#!/usr/bin/env bash
set -euo pipefail
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
ROOT_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
BUILD_DIR="$ROOT_DIR/build"

# 確保 build 目錄存在
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# ---- 0. 找二进制（只在 build/ 下找） ----
BIN=${1:-widgets}
[[ -x $BIN ]] || {
    echo "❌ $BUILD_DIR/$BIN 不存在"
    exit 1
}
echo "✅ 二进制：$BUILD_DIR/$BIN"

# ---- 1. 下載官方 appimagetool 到 build/ ----
TOOL=./appimagetool-official
if [[ ! -x $TOOL ]]; then
    echo "⬇️  下載 go-appimage appimagetool ..."
    wget --progress=bar:force -c https://github.com/$(wget -q https://github.com/probonopd/go-appimage/releases/expanded_assets/continuous -O - | grep "appimagetool-.*-x86_64.AppImage" | head -n 1 | cut -d '"' -f 2)
    mv appimagetool-*.AppImage $TOOL
    chmod +x $TOOL
fi

# ---- 2. 在 build/ 裡準備 AppDir ----
APPDIR=AppDir
rm -rf "$APPDIR"
mkdir -p "$APPDIR/usr/bin"
cp "$BIN" "$APPDIR/usr/bin/"

# desktop & icon
mkdir -p "$APPDIR/usr/share/applications" \
    "$APPDIR/usr/share/icons/hicolor/256x256/apps"
cat >"$APPDIR/usr/share/applications/widgets.desktop" <<EOF
[Desktop Entry]
Name=Widgets
Exec=widgets
Icon=widgets
Type=Application
Categories=Utility;
EOF
touch $APPDIR/usr/share/icons/hicolor/256x256/apps/widgets.png

# ---- 3. deploy ----
echo "📦 软件 deploy 中 ..."
export PATH="/usr/lib/qt6/bin:$PATH"
if [[ -d /usr/lib/x86_64-linux-gnu/qt6/plugins/platforms ]]; then
    export QTDIR=/usr/lib/x86_64-linux-gnu/qt6
elif [[ -d /usr/lib/qt6/plugins/platforms ]]; then
    export QTDIR=/usr/lib/qt6
else
    echo "❌ 未找到 Qt6 platforms 插件" >&2
    exit 1
fi
$TOOL deploy "$APPDIR/usr/share/applications/widgets.desktop"

# To fix tls plugin unupport of tool
TLSDIR="${QTDIR}/plugins/tls"
if [[ -d "$TLSDIR" ]]; then
    mkdir -p AppDir/${QTDIR}/plugins
    cp -Lr "$TLSDIR" AppDir/${QTDIR}/plugins
fi

# 添加插件依赖的 libssl.so.3 / libcrypto.so.3
for so in "$TLSDIR/libqopensslbackend.so"; do
    ldd "$so" | grep -oE '/[^ ]*(libssl|libcrypto)\.so\.[0-9]' |
        while read -r lib; do cp -L "$lib" AppDir/usr/lib; done
done

# AppRun
cat >"$APPDIR/AppRun" <<'EOF'
#!/bin/sh
SELF=$(readlink -f "$0")
APPDIR="${SELF%/*}"

export QT_PLUGIN_PATH="$APPDIR/usr/lib/x86_64-linux-gnu/qt6/plugins:$QT_PLUGIN_PATH"
export QT_PLUGIN_PATH="$APPDIR/usr/lib/qt6/plugins:$QT_PLUGIN_PATH"

export LD_LIBRARY_PATH="$APPDIR/usr/lib64:$LD_LIBRARY_PATH"
export LD_LIBRARY_PATH="$APPDIR/usr/lib:$LD_LIBRARY_PATH"
export LD_LIBRARY_PATH="$APPDIR/usr/lib/x86_64-linux-gnu:$LD_LIBRARY_PATH"

exec "$APPDIR/usr/bin/widgets" "$@"
EOF
chmod +x "$APPDIR/AppRun"

# ---- 4. 打包 ----
echo "🔧 生成 AppImage ..."
VERSION=example $TOOL "$APPDIR"
mv Widget*.AppImage widgets-x86_64.AppImage

# ---- 5. 导出 ----
tar -czf widgets-linux-x86_64.tar.gz -C "$APPDIR" .
echo "✅ 輸出於 $BUILD_DIR："
ls -lh widgets-x86_64.AppImage widgets-linux-x86_64.tar.gz
