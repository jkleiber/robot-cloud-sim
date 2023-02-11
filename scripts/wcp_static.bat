@echo off

:: Change to this script's directory
cd %~dp0

:: Copy the static files.
xcopy /e /k /h /i /Y ..\templates ..\build\apps\Debug\templates
xcopy /e /k /h /i /Y ..\src\msg\*.proto ..\build\apps\Debug\static\msg
