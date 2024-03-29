# Plume
## An application designed to replace INKK for INSA Lyon
---
### Table of contents
 - [Download](#download)
 - [Build and Run](#build-and-run)
 - [Contact](#contact)
 - [Coding conventions](#coding-conventions)
 - [License](#license)

## Download
- [Windows](https://nightly.link/EnyoYoen/Plume/workflows/build-windows/master/Plume%20-%20Windows%20x64.zip)
- [MacOs](https://nightly.link/EnyoYoen/Plume/workflows/build-macos/master/Plume%20-%20MacOs%20x64.zip)
- [Linux](https://nightly.link/EnyoYoen/Plume/workflows/build-linux/master/Plume%20-%20Linux%20x64.zip) 

For Linux and MacOs, you need to install some dependencies :  
- On Ubuntu 22.04 or later
```shell
$ sudo apt-get install qt6-base-dev libpthread-stubs0-dev
```
- On MacOS (with [Homebrew](https://brew.sh/index)):
```shell
$ brew install qt6 libpthread-stubs
```

Plume is available for Windows (x64), Linux (x64) and MacOs (x64) for now.  
You can compile Plume if it is not available for your operating system or your arch : [Build and Run](#build-and-run).

## Build and Run
### Dependencies
- Qt6 (Widgets)
- Pthread

### Install the dependencies
**On Ubuntu 22.04 or later**
```shell
$ sudo apt-get install qt6-base-dev libpthread-stubs0-dev
```

**On Windows (with [vcpkg](https://github.com/microsoft/vcpkg/#quick-start-windows)):**
```shell
> vcpkg install qt6 pthreads
```

**On MacOS (with [Homebrew](https://brew.sh/index)):**
```shell
$ brew install qt6 libpthread-stubs
```

You will also need [CMake](https://cmake.org/download/) in your system PATH (Minimum version: 3.1.0)
### Build
In 'Plume' (top) folder:  
On Windows: (needs MinGW)
```shell
git submodule update --init --recursive
cd libs/libical
mkdir build
cmake -B build --warn-uninitialized -G "MinGW Makefiles" -DCMAKE_INSTALL_PREFIX=. -DCMAKE_BUILD_TYPE=Release -DWITH_CXX_BINDINGS=False -DLIBICAL_BUILD_TESTING=False -DENABLE_GTK_DOC=False -DICAL_GLIB=False -DGOBJECT_INTROSPECTION=False -DICAL_GLIB_VAPI=False
cmake --build build
cd build
mingw32-make
mingw32-make install

cd build\
cmake -DCMAKE_BUILD_TYPE=Release .. -DCMAKE_TOOLCHAIN_FILE=(path/to/)vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build . -j 8 --config Release
```
Otherwise:
```shell
git submodule update --init --recursive
cd libs/libical
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=. -DCMAKE_BUILD_TYPE=Release -DWITH_CXX_BINDINGS=False -DLIBICAL_BUILD_TESTING=False -DENABLE_GTK_DOC=False -DICAL_GLIB=False -DGOBJECT_INTROSPECTION=False -DICAL_GLIB_VAPI=False
make
sudo make install

cd ../build
cmake -DCMAKE_BUILD_TYPE=Release .. 
cmake --build . -j 8
```
### Run
The executable is in the 'Plume/build' folder ;)  

## Contact
You can reach me by e-mail at this address: enyotoyrep@gmail.com or on Discord: 'yoen.'

## Coding Conventions
If you want to contribute to this project, you must respect the coding conventions from Qt (https://wiki.qt.io/Coding_Conventions).

## License
[MIT License](https://en.wikipedia.org/wiki/MIT_License)

Copyright (c) 2024 EnyoYoen

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

