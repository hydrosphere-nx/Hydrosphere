# Copyright (c) 2019 Hydrosphère Developers
#
# Licensed under the Apache License, Version 2.0 <LICENSE-APACHE or
# http://www.apache.org/licenses/LICENSE-2.0> or the MIT license
# <LICENSE-MIT or http://opensource.org/licenses/MIT>, at your
# option. This file may not be copied, modified, or distributed
# except according to those terms.

# TODO(Kaenbyō): rewrite using meson to remvoe dependency on make
ifeq ($(strip $(HYDROSPHERE_SYSROOT)),)
$(error "Please set HYDROSPHERE_SYSROOT in your environment. export HYDROSPHERE_SYSROOT=<path to>/hydrosphere")
endif

all: deploy

install-helpers:
	meson setup helper build/helper -Dhydrosphere_sysroot=$(HYDROSPHERE_SYSROOT)
	ninja -C build/helper install

build-hydrosphere-aarch64: install-helpers install-builtins install-module-runtime
	meson setup libhydrosphere build/aarch64/libhydrosphere --cross-file $(HYDROSPHERE_SYSROOT)/usr/share/meson/cross/aarch64-nx --buildtype=debugoptimized
	ninja -C build/aarch64/libhydrosphere

build-hydrosphere-aarch32: install-helpers install-builtins install-module-runtime
	meson setup libhydrosphere build/aarch32/libhydrosphere --cross-file $(HYDROSPHERE_SYSROOT)/usr/share/meson/cross/aarch32-nx --buildtype=debugoptimized
	ninja -C build/aarch32/libhydrosphere

build-builtins-aarch64: install-helpers
	meson setup rt-builtins build/aarch64/rt-builtins --cross-file $(HYDROSPHERE_SYSROOT)/usr/share/meson/cross/aarch64-nx
	ninja -C build/aarch64/rt-builtins

build-builtins-aarch32: install-helpers
	meson setup rt-builtins build/aarch32/rt-builtins --cross-file $(HYDROSPHERE_SYSROOT)/usr/share/meson/cross/aarch32-nx
	ninja -C build/aarch32/rt-builtins

build-module-runtime-aarch64: install-helpers
	meson setup module-runtime build/aarch64/module-runtime --cross-file $(HYDROSPHERE_SYSROOT)/usr/share/meson/cross/aarch64-nx
	ninja -C build/aarch64/module-runtime

build-module-runtime-aarch32: install-helpers
	meson setup module-runtime build/aarch32/module-runtime --cross-file $(HYDROSPHERE_SYSROOT)/usr/share/meson/cross/aarch32-nx
	ninja -C build/aarch32/module-runtime

build-rtld-aarch64: install-helpers build-builtins-aarch64
	meson setup rtld build/aarch64/rtld --cross-file $(HYDROSPHERE_SYSROOT)/usr/share/meson/cross/aarch64-nx --buildtype=release
	ninja -C build/aarch64/rtld

build-rtld-aarch32: install-helpers build-builtins-aarch32
	meson setup rtld build/aarch32/rtld --cross-file $(HYDROSPHERE_SYSROOT)/usr/share/meson/cross/aarch32-nx --buildtype=release
	ninja -C build/aarch32/rtld

build-application-template-aarch64: install-helpers
	meson setup application-template build/aarch64/application-template --cross-file $(HYDROSPHERE_SYSROOT)/usr/share/meson/cross/aarch64-nx
	ninja -C build/aarch64/application-template

build-application-template-aarch32: install-helpers
	meson setup application-template build/aarch32/application-template --cross-file $(HYDROSPHERE_SYSROOT)/usr/share/meson/cross/aarch32-nx
	ninja -C build/aarch32/application-template

install-hydrosphere: install-builtins install-rtld build-hydrosphere-aarch64 build-hydrosphere-aarch32
	ninja -C build/aarch64/libhydrosphere install
	ninja -C build/aarch32/libhydrosphere install

install-builtins: build-builtins-aarch64 build-builtins-aarch32
	ninja -C build/aarch64/rt-builtins install
	ninja -C build/aarch32/rt-builtins install

install-module-runtime: build-module-runtime-aarch64 build-module-runtime-aarch32
	ninja -C build/aarch64/module-runtime install
	ninja -C build/aarch32/module-runtime install

install-rtld: build-rtld-aarch64 build-rtld-aarch32
	ninja -C build/aarch64/rtld install
	ninja -C build/aarch32/rtld install

clean-rtld:
	rm -rf build/aarch64/rtld
	rm -rf build/aarch32/rtld

clean-helpers:
	rm -rf build/helper

clean-builtins:
	rm -rf build/aarch64/rt-builtins
	rm -rf build/aarch32/rt-builtins

clean-module-runtime:
	rm -rf build/aarch64/module-runtime
	rm -rf build/aarch32/module-runtime

clean-hydrosphere:
	rm -rf build/aarch64/libhydrosphere
	rm -rf build/aarch32/libhydrosphere

clean-application-template:
	rm -rf build/aarch64/application-template
	rm -rf build/aarch32/application-template

clean: clean-helpers clean-builtins clean-module-runtime clean-hydrosphere clean-rtld

deploy: install-hydrosphere install-rtld install-builtins

prepare-launch-aarch64: deploy build-application-template-aarch64
	mkdir -p output/aarch64
	cp $(HYDROSPHERE_SYSROOT)/usr/lib/aarch64/rtld.nso output/aarch64/rtld
	cp $(HYDROSPHERE_SYSROOT)/usr/lib/aarch64/libhydrosphere.nso output/aarch64/sdk
	cp build/aarch64/application-template/application.nso output/aarch64/main

prepare-launch-aarch32: deploy build-application-template-aarch32
	mkdir -p output/aarch32
	cp $(HYDROSPHERE_SYSROOT)/usr/lib/aarch32/rtld.nso output/aarch32/rtld
	cp $(HYDROSPHERE_SYSROOT)/usr/lib/aarch32/libhydrosphere.nso output/aarch32/sdk
	cp build/aarch32/application-template/application.nso output/aarch32/main

prepare-launch: prepare-launch-aarch32 prepare-launch-aarch64

