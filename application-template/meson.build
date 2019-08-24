# Copyright (c) 2019 Hydrosphère Developers
#
# Licensed under the Apache License, Version 2.0 <LICENSE-APACHE or
# http://www.apache.org/licenses/LICENSE-2.0> or the MIT license
# <LICENSE-MIT or http://opensource.org/licenses/MIT>, at your
# option. This file may not be copied, modified, or distributed
# except according to those terms.

project('application', 
        ['c', 'cpp'],
        license: ['Apache 2', 'MIT'],
        default_options: ['c_std=c11', 'cpp_std=c++17', 'b_asneeded=false', 'b_lundef=false'],
        version: '0.0.0'
    )

# Check we are using clang
cc = meson.get_compiler('cpp')
assert(cc.get_id() == 'clang', 'This project only support the Clang compiler.')
assert(meson.is_cross_build(), 'This project is supposed to be cross compiled.')

sources = [
    'source/main.cpp',
]

sysroot_arg = ['--sysroot=@0@'.format(meson.get_cross_property('sys_root'))]
extra_c_flags = sysroot_arg + ['-fuse-ld=lld',  '-fno-stack-protector', '-ffreestanding', '-nostdlib', '-nodefaultlibs', '-Wno-unused-command-line-argument', '-Wno-gcc-compat', '-Wall', '-Wextra']
extra_cpp_flags = extra_c_flags + ['-fno-rtti' , '-fomit-frame-pointer',  '-fno-exceptions', '-fno-asynchronous-unwind-tables', '-fno-unwind-tables']
target_cpu_familly = target_machine.cpu_family()

link_args = meson.get_cross_property('link_args')
mapfile = 'exported.txt'
vflag = '-Wl,--version-script,@0@/@1@'.format(meson.current_source_dir(), mapfile)
tflags = '-Wl,-T,@0@/share/linker-scripts/@1@/module.ld'.format(get_option('prefix'), meson.get_cross_property('arch'))
hydrosphere_standalone_link_args = meson.get_cross_property('hydrosphere_standalone_link_args')
hydrosphere_default_link_args = meson.get_cross_property('hydrosphere_default_link_args')
extra_ld_flags = sysroot_arg + hydrosphere_standalone_link_args + hydrosphere_default_link_args + [vflag, tflags]

library_directory = '@0@/@1@'.format(get_option('prefix'), get_option('libdir'))
module_runtime_dependency = cc.find_library('module-runtime', dirs: [library_directory], required: true)

application_shared_library = shared_library(
    'application',
    sources,
    name_prefix: '',
    include_directories: include_directories('include'),
    c_args: extra_c_flags,
    cpp_args: extra_cpp_flags,
    link_args: link_args + extra_ld_flags,
    dependencies: [module_runtime_dependency]
)

# NSO generation part
linkle_tool = find_program('linkle')

custom_target('application.nso',
    build_by_default: true,
    input: application_shared_library,
    output: 'application.nso',
    command: [linkle_tool, 'nso', '@INPUT@', '@OUTPUT@'],
)
