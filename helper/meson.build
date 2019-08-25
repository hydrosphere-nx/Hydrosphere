# Copyright (c) 2019 Hydrosphère Developers
#
# Licensed under the Apache License, Version 2.0 <LICENSE-APACHE or
# http://www.apache.org/licenses/LICENSE-2.0> or the MIT license
# <LICENSE-MIT or http://opensource.org/licenses/MIT>, at your
# option. This file may not be copied, modified, or distributed
# except according to those terms.

project('helper')

hydrosphere_sysroot = get_option('hydrosphere_sysroot')
hydrosphere_prefix = join_paths(hydrosphere_sysroot, 'usr')
hydrosphere_datadir = join_paths(hydrosphere_prefix, 'share')

cdata = configuration_data()
cdata.set('SYSROOT', get_option('hydrosphere_sysroot'))

datadir = join_paths(hydrosphere_datadir, 'meson', 'cross')


configure_file(input: 'meson/cross/aarch64-nx.in',
               output: 'aarch64-nx',
               configuration: cdata,
               install: true,
               install_dir: datadir)

configure_file(input: 'meson/cross/aarch32-nx.in',
               output: 'aarch32-nx',
               configuration: cdata,
               install: true,
               install_dir: datadir)

install_subdir('linker-scripts', install_dir: hydrosphere_datadir, strip_directory : false)
