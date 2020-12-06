@echo off

REM compile shaders

REM simple shader
C:\Program Files\bgfx\bin\shaderc.exe ^
-f shader\v_simple.sc -o shader\v_simple.bin ^
--platform windows --type vertex --verbose -i ./ -p vs_5_0

C:\Program Files\bgfx\bin\shaderc.exe ^
-f shader\f_simple.sc -o shader\f_simple.bin ^
--platform windows --type fragment --verbose -i ./ -p ps_5_0
