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
    'source/crt0.s',
    'source/default_definition.cpp',
]

extra_c_flags = ['-fuse-ld=lld',  '-fno-stack-protector', '-ffreestanding', '-nostdlib', '-nodefaultlibs', '-Wno-unused-command-line-argument', '-Wno-gcc-compat', '-Wall', '-Wextra']
extra_cpp_flags = extra_c_flags + ['-fno-rtti' , '-fomit-frame-pointer',  '-fno-exceptions', '-fno-asynchronous-unwind-tables', '-fno-unwind-tables']

static_library(
    'module-runtime',
    common_sources,
    c_args: extra_c_flags,
    cpp_args: extra_cpp_flags,
    pic: true,
    build_by_default: true,
    install: true
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

