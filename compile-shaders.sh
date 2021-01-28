#!/bin/bash

# compile shaders

# simple shader
shaderc  \
-f shader/v_simple.sc -o shader/v_simple.bin  \
--platform linux --type vertex --verbose -i ./shader

shaderc \
-f shader/f_simple.sc -o shader/f_simple.bin \
--platform linux --type fragment --verbose -i ./shader
