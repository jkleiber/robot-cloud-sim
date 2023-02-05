

:: Find clean script working directory
cd %~dp0
set COMPILE_WD="..\build"
set APPS_WD="..\build\apps"

:: Change to the build directory
cd %COMPILE_WD%

:: Remove all files ignored by git (compile scripts, etc.)
git clean -dfX

:: Go one layer deeper, into apps/
cd %APPS_WD%

:: Remove all files ignored by git (compile scripts, etc.)
git clean -dfX

echo "Done cleaning!"
cd %~dp0