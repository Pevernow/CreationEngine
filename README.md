# CreationEngine

[![GPLv3 license](https://img.shields.io/badge/License-GPLv3-blue.svg)](http://perso.crans.org/besson/LICENSE.html)
[![Open Source? Yes!](https://badgen.net/badge/Open%20Source%20%3F/Yes%21/blue?icon=github)](https://github.com/Naereen/badges/)
[![ForTheBadge built-by-developers](http://ForTheBadge.com/images/badges/built-by-developers.svg)](https://github.com/Pevernow/)
[![GitHub stars](https://img.shields.io/github/stars/Naereen/StrapDown.js.svg?style=social&label=Star&maxAge=2592000)](https://github.com/Pevernow/CreationEngine/stargazers/)

Open source,High performance,Cross platform,Beautiful 3D voxel sandbox metaverse

Copyright (c) 2020-2021 Pevernow <3450354617@qq.com>

## ScreenShot

![creation](https://user-images.githubusercontent.com/29888010/131216383-692b30e0-f77d-4a02-b0b0-18692d79cb32.png)


2021/8/28

This game is not finished
--------------------------
- Don't expect it to work as well as a finished game will.
- Please join in the project if you want it will become better.

## Compiling

(Take Ubuntu as an example,support WindowsXP+, Linux, Mac and so on)

#### Dependencies

| Dependency | Version | Commentary |
|------------|---------|------------|
| GCC        | 4.9+    | Can be replaced with Clang 3.4+ |
| CMake      | latest  |            |
| bgfx-cmake | latest  | Also can manual build bgfx for cmake|
| LuaJIT     | 2.0+    |            |
| SDL2       | 2.0+    |            |

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


# Feature

Fast Cross Platform renderer -- BGFX

Fastest RUDP -- ASIO_KCP

Fastest and Minimal net packet -- Flatbuffers

Simple but Useful GUI -- IMGUI

Fast but Lightly Logger -- Spdlog

Cross Platform and Powerful Library -- SDL2


## Finished

1.window manager

2.world data with a chunk manager

3.sample control

4.biome mapgen

5.Network manager


## TODO

1.move without glm (Use bx to replace it)

2.GUI

3.Modding API (with lua)

4.Biome mapgen v2

5.Entity

6.Shaders

# Licence
src/ directory of all the code in accordance use LICENCE.md

other parts of the project use with their own licences
