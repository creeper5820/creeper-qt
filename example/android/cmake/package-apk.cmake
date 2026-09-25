# 归集 APK 到 build/（由 package target 调用）

file(GLOB_RECURSE apks "${APP_BUILD_DIR}/*.apk")
if(NOT apks)
    message(FATAL_ERROR "未找到 APK：请先构建 app（cmake --build --preset apk）")
endif()

list(SORT apks)
list(POP_BACK apks apk)

file(MAKE_DIRECTORY "${OUTPUT_DIR}")
set(target "${OUTPUT_DIR}/widgets-android-${ANDROID_ABI}.apk")
file(COPY_FILE "${apk}" "${target}" ONLY_IF_DIFFERENT)

message(STATUS "输出于 ${target}")
