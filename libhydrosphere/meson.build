# Copyright (c) 2019 Hydrosphère Developers
#
# Licensed under the Apache License, Version 2.0 <LICENSE-APACHE or
# http://www.apache.org/licenses/LICENSE-2.0> or the MIT license
# <LICENSE-MIT or http://opensource.org/licenses/MIT>, at your
# option. This file may not be copied, modified, or distributed
# except according to those terms.

project('libhydrosphere', 
        ['c', 'cpp'],
        license: ['Apache 2', 'MIT'],
        default_options: ['c_std=c11', 'cpp_std=c++17', 'b_asneeded=false', 'b_lundef=false'],
        version: '0.0.0'
    )

# Check we are using clang
cc = meson.get_compiler('cpp')
assert(cc.get_id() == 'clang', 'This project only support the Clang compiler.')
assert(meson.is_cross_build(), 'This project is supposed to be cross compiled.')

common_sources = [
    'source/common/compiler/allocator.cpp',
    'source/common/compiler/memcpy.cpp',
    'source/common/compiler/shared_ptr.cpp',
    'source/common/compiler/unique_ptr.cpp',
    'source/common/diag/diag_api.cpp',
    'source/common/init/initialization.cpp',
    'source/common/init/module_requirements.cpp',
    'source/common/util/util_string_api.cpp',
    'source/common/os/detail/os_threadlist.cpp',
    'source/common/os/detail/os_virtualmemory_allocator.cpp',
    'source/common/os/os_barrier_api.cpp',
    'source/common/os/os_condition_variable_impl.cpp',
    'source/common/os/os_condition_variable_api.cpp',
    'source/common/os/os_critical_section.cpp',
    'source/common/os/os_kernelevent_api.cpp',
    'source/common/os/os_mutex_api.cpp',
    'source/common/os/os_thread_api.cpp',
    'source/common/os/os_tls.cpp',
    'source/common/os/os_userevent_api.cpp',
]

external_sources = [
    'external/source/snprintf.c'
]

sysroot_arg = ['--sysroot=@0@'.format(meson.get_cross_property('sys_root'))]
extra_c_flags = ['-fuse-ld=lld',  '-fno-stack-protector', '-ffreestanding', '-nostdlib', '-nodefaultlibs', '-Wno-unused-command-line-argument', '-Wno-gcc-compat', '-Wall', '-Wextra']
extra_cpp_flags = extra_c_flags + ['-fno-rtti' , '-fomit-frame-pointer',  '-fno-exceptions', '-fno-asynchronous-unwind-tables', '-fno-unwind-tables']
target_cpu_familly = target_machine.cpu_family()

if target_cpu_familly == 'aarch64'
    arch_sources = ['source/aarch64/svc.s']
elif target_cpu_familly == 'arm'
    arch_sources = ['source/aarch32/svc.s']
else
    error('Unsuported target cpu_familly')
endif

link_args = meson.get_cross_property('link_args')
mapfile = 'exported.txt'
vflag = '-Wl,--version-script,@0@/@1@'.format(meson.current_source_dir(), mapfile)
tflags = '-Wl,-T,@0@/share/linker-scripts/@1@/module.ld'.format(get_option('prefix'), meson.get_cross_property('arch'))
hydrosphere_standalone_link_args = meson.get_cross_property('hydrosphere_standalone_link_args')
hydrosphere_default_link_args = meson.get_cross_property('hydrosphere_default_link_args')
extra_ld_flags = sysroot_arg + hydrosphere_standalone_link_args + hydrosphere_default_link_args + [vflag, tflags]

library_directory = '@0@/@1@'.format(get_option('prefix'), get_option('libdir'))
compiler_rt_builtins_dependency = cc.find_library('clang_rt.builtins', dirs: [library_directory], required: true)
module_runtime_dependency = cc.find_library('module-runtime', dirs: [library_directory], required: true)

both_libs = both_libraries(
    'hydrosphere',
    common_sources + arch_sources + external_sources,
    include_directories: include_directories('include', 'external/include', 'source/common'),
    link_args: link_args + extra_ld_flags,
    c_args: extra_c_flags,
    cpp_args: extra_cpp_flags,
    dependencies: [compiler_rt_builtins_dependency, module_runtime_dependency],
    link_depends: mapfile,
    pic: true,
    install: true
)

# NSO generation part
linkle_tool = find_program('linkle')

custom_target('libhydrosphere.nso',
    build_by_default: true,
    install: true,
    input: both_libs.get_shared_lib(),
    output: 'libhydrosphere.nso',
    command: [linkle_tool, 'nso', '@INPUT@', '@OUTPUT@'],
    install_dir: get_option('libdir')
)

# Linter
cpplint = find_program('cpplint', required: false)
if cpplint.found()
    run_target('lint',
        command: [cpplint, '--recursive', meson.current_source_dir()]
    )
else
    warning('cpplint not found, linter task disabled')
endif

# Doxygen


doxygen = find_program('doxygen', required : false)
if not doxygen.found()
  warning('doxygen not found. Documentation task disabled')
else
    cdata = configuration_data()
    cdata.set('VERSION', meson.project_version())

    if find_program('dot', required : false).found()
    # In the real world this would set the variable
    # to YES. However we set it to NO so that the
    # list of generated files is always the same
    # so tests always pass.
    cdata.set('HAVE_DOT', 'NO')
    else
    cdata.set('HAVE_DOT', 'NO')
    endif

    subdir('doc')
endif

# Extra install

# Headers install
# TODO(Kaenbyō): find a better way of doing this
find_program = find_program('find') 
include_path = '@0@/include'.format(meson.current_source_dir())
find_includes_output = run_command(find_program.path(), include_path, '-name', '*.hpp', '-printf', '%P\n')
includes_list = find_includes_output.stdout().strip().split('\n')
all_includes = []
foreach include : includes_list
    include_file = files('@0@/@1@'.format(include_path, include))
    all_includes += include_file
    include_components = include.split('/')
    subdir_components = []

    foreach path_component : include_components
      if subdir_components.length() < include_components.length() - 1
        subdir_components += path_component
      endif
    endforeach

    include_subdir = '/'.join(subdir_components)

    install_headers(include_file, subdir : include_subdir)
endforeach
