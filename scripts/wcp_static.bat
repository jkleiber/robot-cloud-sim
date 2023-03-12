@echo off

:: Change to this script's directory
cd %~dp0

:: Copy the static files.
xcopy /e /k /h /i /Y ..\server\templates ..\build\apps\Debug\templates

:: Boat
xcopy /e /k /h /i /Y ..\src\boat\*.proto ..\build\apps\Debug\static\boat
xcopy /e /k /h /i /Y ..\server\static\boat\* ..\build\apps\Debug\static\boat

:: AUV
xcopy /e /k /h /i /Y ..\src\auv\*.proto ..\build\apps\Debug\static\auv
xcopy /e /k /h /i /Y ..\server\static\auv\* ..\build\apps\Debug\static\auv
