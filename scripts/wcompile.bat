@echo off

:: Change to this script's directory
cd %~dp0

:: Set constants
set COMPILE_WD="../build"
set VCPKG_PATH="../vcpkg"

cmake -B %COMPILE_WD% -S .. -DCMAKE_TOOLCHAIN_FILE=%VCPKG_PATH%/scripts/buildsystems/vcpkg.cmake

cmake --build %COMPILE_WD% 

:: Copy the static files.
call wcp_static.bat
