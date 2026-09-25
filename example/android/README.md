# Android 示例

自包含工程：以本目录为项目根，依赖经 FetchContent 获取，不引用上级目录。
版本与开关统一在 `CMakePresets.json`。

## 工具链（首次执行）

```bash
cmake --workflow --preset download-sdk
cmake --workflow --preset download-dep
cmake --workflow --preset download-emu
```

`download-dep` 同时下载 Material Icons（Round / Outlined）字体到 `build/fonts/`，构建时编入应用资源。

## 构建 APK

```bash
cmake --workflow --preset apk
```

## 打包产物

```bash
cmake --workflow --preset package
```

输出：`example/android/build/dist/widgets-android-arm64-v8a.apk`

## 在容器中执行

宿主缺少依赖时，用容器执行同一条指令（容器内路径与宿主一致）：

```bash
docker compose run --rm --user "$(id -u):$(id -g)" build \
    cmake --workflow --preset apk
```

## 模拟器

```bash
cmake --workflow --preset download-emu
cmake --build build/tools --target emulator         # 无头启动（emulator-5554）并等待开机
cmake --build build/tools --target emulator-window  # 带窗口启动
cmake --build build/tools --target emulator-stop
```

## 安装到模拟器

x86_64 模拟器镜像自带 ARM64 转译，可直接安装 arm64-v8a 包。

发布包（nightly）由 CI 使用 GitHub Secrets 中的 release keystore 签名，下载后可直接安装：

```bash
SDK=build/android-sdk
"$SDK/platform-tools/adb" -s emulator-5554 install -r build/dist/widgets-android-arm64-v8a-signed.apk
"$SDK/platform-tools/adb" -s emulator-5554 shell am start \
    -n com.creeper5820.widgets/org.qtproject.qt.android.bindings.QtActivity
```

本地 `cmake --workflow --preset package` 产出的 `build/dist/widgets-android-arm64-v8a.apk` 是未签名的，需用你自己的 keystore 签名。先新建一把（放在仓库外，勿入库），再用 `apksigner` 签名：

```bash
SDK=build/android-sdk
keytool -genkeypair -v \
    -keystore ~/.android-widgets/release.keystore \
    -alias widgets -keyalg RSA -keysize 2048 -validity 10000

"$SDK/build-tools/37.0.0/apksigner" sign \
    --ks ~/.android-widgets/release.keystore \
    --out build/dist/widgets-android-arm64-v8a-signed.apk \
    build/dist/widgets-android-arm64-v8a.apk
```
