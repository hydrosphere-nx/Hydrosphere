<p align="center">
    <img
      alt="Hydrosphère"
      src="https://raw.githubusercontent.com/hydrosphere-nx/Hydrosphere/master/.github/res/hydrosphere_800.png"
      width="200"
    />
</p>

# Hydrosphère [![Travis Build](https://img.shields.io/travis/com/hydrosphere-nx/Hydrosphere.svg?logo=travis)](https://travis-ci.com/hydrosphere-nx/Hydrosphere)
"Ocean beyond the Horizon"

Hydrosphère is a work in progress C++ toolchain targeting the Nintendo Switch userland.

## Install from sources

### Building on *nix
1. Make sure you have installed the dependencies:

   * `llvm` 8.x or later
   * `clang` 8.x or later
   * `meson` 0.51 or later
   * `linkle` 0.2.7 or later (Releases: [here](https://github.com/MegatonHammer/linkle/releases/latest))
   * `ninja`
   * `make`
   * `git`

2. Clone the [source] with `git`:

   ```sh
   $ git clone --recursive https://github.com/hydrosphere-nx/Hydrosphere.git
   $ cd Hydrosphere
   ```

[source]: https://github.com/hydrosphere-nx/Hydrosphere

3. Choose Hydrosphère sysroot and export it:

    ```sh
    $ export HYDROSPHERE_SYSROOT=<path to your sysroot>
    ```

4. Build and deploy Hydrosphère to ``$HYDROSPHERE_SYSROOT``:

    ```sh
    $ make deploy
    ```

### Building on Windows (MINGW64)
1. Install [MSYS2](https://www.msys2.org/)

2. Make sure you have installed the dependencies and **that you are running under MINGW64 shell**:

    ```sh
    # Install dependencies
    $ pacman -S mingw-w64-x86_64-llvm mingw-w64-x86_64-clang mingw-w64-x86_64-lld mingw-w64-x86_64-meson mingw-w64-x86_64-ninja make wget gunzip git

    # Install linkle manually
    $ mkdir -p $HOME/bin
    $ wget -nc -O - https://github.com/MegatonHammer/linkle/releases/download/v0.2.7/linkle-v0.2.7-x86_64-pc-windows-msvc.zip | gunzip > $HOME/bin/linkle.exe

    # Add ~/bin to PATH for new shell instances
    $ echo "export PATH=\"\$HOME/bin:\$PATH\"" >> $HOME/.bashrc

    # Add ~/bin to PATH for this shell instance
    $ export PATH="$HOME/bin:$PATH"
    ```
2. Clone the [source] with `git`:

   ```sh
   $ git clone --recursive https://github.com/hydrosphere-nx/Hydrosphere.git
   $ cd Hydrosphere
   ```

[source]: https://github.com/hydrosphere-nx/Hydrosphere

3. Choose Hydrosphère sysroot and export it:

    ```sh
    $ export HYDROSPHERE_SYSROOT=<path to your sysroot>
    ```

4. Build and deploy Hydrosphère to ``$HYDROSPHERE_SYSROOT``:

    ```sh
    $ make deploy
    ```

## Documentation

Documentation is hosted on our [github pages](https://hydrosphere-nx.github.io/Hydrosphere).

## License

Hydrosphère is distributed under the terms of either the MIT license or the Apache
License (Version 2.0), at the user's choice.

See [LICENSE-APACHE](LICENSE-APACHE) and [LICENSE-MIT](LICENSE-MIT).
