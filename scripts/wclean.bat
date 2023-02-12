

:: Find clean script working directory
cd %~dp0
set SRC_DIR="..\build\src"
set CMAKE_DIR="..\build\CMakeFiles"
set APPS_DIR="..\build\apps"

:: Clean temporary CMakeFiles
cd %CMAKE_DIR%

:: Remove all files ignored by git (compile scripts, etc.)
git clean -dfX

:: Clean the sources
cd %SRC_DIR%

:: Remove all files ignored by git (compile scripts, etc.)
git clean -dfX

:: Clean apps
cd %APPS_DIR%

:: Remove all files ignored by git (compile scripts, etc.)
git clean -dfX

echo "Done cleaning!"
cd %~dp0