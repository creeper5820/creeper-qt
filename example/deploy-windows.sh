#!/bin/bash

# Define paths
BIN="example/widgets/widgets.exe"
DLL="libcreeper-qt-widgets.dll"
TARGET_DIR="./build/AppDir"

# Create target directory if it doesn't exist
mkdir -p "$TARGET_DIR"

BIN_NAME=$(basename "$BIN")

# Step 1: Copy main executable and custom DLL
echo "🚚 Copying main binary and custom DLL..."
cp -u "build/$BIN" "$TARGET_DIR/"
cp -u "build/$DLL" "$TARGET_DIR/"

# Step 2: Run windeployqt6 to collect Qt DLLs
echo "🔧 Running windeployqt6..."
cd "$TARGET_DIR"
windeployqt6 "$BIN_NAME" --release --no-translations --no-system-d3d-compiler --no-opengl-sw
cd -

# Step 3: Copy remaining DLLs from ldd output, excluding unwanted ones
echo "📦 Copying additional DLLs from ldd..."
EXCLUDE_DLLS=("UxTheme.dll" "mmpiclient.dll" "dwmapi.dll" "version.dll" "winmm.dll")

ldd "build/$BIN" | awk '/=>/ {print $3}' | while read -r dll_path; do
    dll_name=$(basename "$dll_path")
    skip=false
    for exclude in "${EXCLUDE_DLLS[@]}"; do
        if [[ "$dll_name" == "$exclude" ]]; then
            echo "❌ Skipping: $dll_name"
            skip=true
            break
        fi
    done
    if [[ "$skip" == false && -n "$dll_path" && -f "$dll_path" ]]; then
        echo "✅ Copying: $dll_name"
        cp -u "$dll_path" "$TARGET_DIR/"
    fi
done

echo "🎉 All filtered dependencies copied to $TARGET_DIR"
