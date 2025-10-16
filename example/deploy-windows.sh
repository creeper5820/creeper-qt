#!/bin/bash

# Define paths
BIN="./build/widgets.exe"
DLL="./build/libcreeper-qt.dll"
TARGET_DIR="./build/AppDir"

# Create target directory if it doesn't exist
mkdir -p "$TARGET_DIR"

# Step 1: Copy main executable and custom DLL
echo "🚚 Copying main binary and custom DLL..."
cp -u "$BIN" "$TARGET_DIR/"
cp -u "$DLL" "$TARGET_DIR/"

# Step 2: Run windeployqt6 to collect Qt DLLs
echo "🔧 Running windeployqt6..."
windeployqt6 "$BIN" --release --no-translations --no-system-d3d-compiler --no-opengl-sw

# Step 3: Copy remaining DLLs from ldd output
echo "📦 Copying additional DLLs from ldd..."
ldd "$BIN" | awk '/=>/ {print $3}' | while read -r dll_path; do
    if [[ -n "$dll_path" && -f "$dll_path" ]]; then
        echo "Copying: $dll_path"
        cp -u "$dll_path" "$TARGET_DIR/"
    fi
done

echo "✅ All dependencies copied to $TARGET_DIR"
