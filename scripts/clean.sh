
# Find clean script working directory
cd "$(dirname "$0")"
CMAKE_DIR="$(pwd)/../build/CMakeFiles"
SRC_DIR="$(pwd)/../build/src"
APPS_DIR="$(pwd)/../build/apps"

# Clean temporary CMakeFiles
cd $CMAKE_DIR

# Remove all files ignored by git (compile scripts, etc.)
git clean -dfX

# Clean the sources and generated files
cd $SRC_DIR

# Remove all files ignored by git (compile scripts, etc.)
git clean -dfX

# Clean the apps
cd $APPS_DIR

# Remove all files ignored by git (compile scripts, etc.)
git clean -dfX

echo "Done cleaning!"
