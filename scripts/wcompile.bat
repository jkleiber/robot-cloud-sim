@echo off

:: Change to this script's directory
cd %~dp0

:: Set constants
set COMPILE_WD="../build"
set VCPKG_PATH="../vcpkg"

cmake -B %COMPILE_WD% -S .. -DCMAKE_TOOLCHAIN_FILE=%VCPKG_PATH%/scripts/buildsystems/vcpkg.cmake

cmake --build %COMPILE_WD% 

xcopy /e /k /h /i /Y ..\templates ..\build\apps\Debug\templates
xcopy /e /k /h /i /Y ..\src\boat\*.proto ..\build\apps\Debug\static\boat
xcopy /e /k /h /i /Y ..\server\static\boat\* ..\build\apps\Debug\static\boat
