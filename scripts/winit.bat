@echo off

:: Change to this script's directory
cd %~dp0

:: Set up vcpkg
..\vcpkg\bootstrap-vcpkg.bat
