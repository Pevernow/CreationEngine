#!/bin/bash

# compile shaders

# simple shader
shaderc  \
-f shader/block/v_simple.sc -o shader/block/v_simple.bin  \
--platform linux --type vertex --verbose -i --profile 120 ./shader/block

shaderc \
-f shader/block/f_simple.sc -o shader/block/f_simple.bin \
--platform linux --type fragment --verbose -i --profile 120 ./shader/block
