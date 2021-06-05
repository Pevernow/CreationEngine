# CreationEngine

Open source,High performance,Cross platform,Beautiful 3D voxel game.

Copyright (c) 2020-2021 Pevernow <3450354617@qq.com>

This game is not finished
--------------------------
- Don't expect it to work as well as a finished game will.
- Please report any bugs. When doing that, debug.txt is useful.

## Compiling

(Take Ubuntu as an example,support WindowsXP+, Linux, Mac and so on)

#### Dependencies

| Dependency | Version | Commentary |
|------------|---------|------------|
| GCC        | 4.9+    | Can be replaced with Clang 3.4+ |
| CMake      | latest  |            |
| bgfx-cmake | 1.7.3+  | Also can manual build bgfx for cmake|
| LuaJIT     | 2.0+    |            |

#### Download

    $ git clone --depth 1 https://github.com/Pevernow/CreationEngine.git
    $ cd CreationEngine

#### Build

Build a version that runs directly from the source directory:
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make -j 4

Run it:

    $ ./CreationEngine


## Finished

1.window manager

2.world data with a chunk manager

3.sample control

4.biome mapgen


## TODO

1.move without glm (Use bx to replace it)

2.GUI

3.Modding API (with lua)

# Licence
src/ directory of all the code in accordance with the gpl3 open source agreement
