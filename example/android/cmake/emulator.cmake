# 启动模拟器（由 emulator / emulator-window target 调用）：创建 AVD 后启动，等待开机

include("${CMAKE_CURRENT_LIST_DIR}/tool-env.cmake")

set(avd_dir "${HOME_ROOT}/avd/${ANDROID_EMU_NAME}.avd")
file(MAKE_DIRECTORY "${avd_dir}")

file(WRITE "${HOME_ROOT}/avd/${ANDROID_EMU_NAME}.ini"
"avd.ini.encoding=UTF-8
path=${avd_dir}
path.rel=avd/${ANDROID_EMU_NAME}.avd
target=android-36
")

string(REPLACE ";" "/" _sysdir "${ANDROID_EMU_IMAGE}")
file(WRITE "${avd_dir}/config.ini"
"AvdId=${ANDROID_EMU_NAME}
PlayStore.enabled=false
abi.type=${ANDROID_EMU_ABI}
avd.ini.displayname=${ANDROID_EMU_NAME}
avd.ini.encoding=UTF-8
disk.dataPartition.size=6G
fastboot.forceColdBoot=yes
hw.audioInput=no
hw.audioOutput=no
hw.camera.back=none
hw.camera.front=none
hw.cpu.arch=${ANDROID_EMU_ABI}
hw.cpu.ncore=4
hw.gpu.enabled=yes
hw.gpu.mode=swiftshader_indirect
hw.keyboard=yes
hw.lcd.density=480
hw.lcd.height=2400
hw.lcd.width=1080
hw.mainKeys=no
hw.ramSize=2048
hw.sdCard=no
image.sysdir.1=${_sysdir}/
tag.display=Google APIs
tag.id=google_apis
vm.heapSize=256
")

set(emulator "${SDK_ROOT}/emulator/emulator")
set(adb "${SDK_ROOT}/platform-tools/adb" -s emulator-5554)
set(log "${HOME_ROOT}/emulator.log")

set(window_args -no-window)
if(EMU_WINDOW)
    set(window_args "")
    # 模拟器自带的 Qt 不含 wayland 插件，经 XWayland 显示
    set(ENV{QT_QPA_PLATFORM} xcb)
endif()

# 模拟器以后台进程运行，固定端口使 adb 可按序列号定位
execute_process(
    COMMAND sh -c "nohup \"$0\" \"$@\" > \"${log}\" 2>&1 &"
        "${emulator}" -avd "${ANDROID_EMU_NAME}" -port 5554
        ${window_args} -no-audio -no-boot-anim -no-snapshot -no-metrics
        -gpu swiftshader_indirect -accel on
    COMMAND_ERROR_IS_FATAL ANY)

execute_process(COMMAND ${adb} wait-for-device TIMEOUT 120 RESULT_VARIABLE _wait)
if(NOT _wait EQUAL 0)
    message(FATAL_ERROR "模拟器未能连接（${_wait}），日志：${log}")
endif()

foreach(_i RANGE 90)
    execute_process(COMMAND ${adb} shell getprop sys.boot_completed TIMEOUT 10
        OUTPUT_VARIABLE _boot OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)
    if(_boot STREQUAL "1")
        message(STATUS "模拟器已就绪")
        return()
    endif()
    execute_process(COMMAND "${CMAKE_COMMAND}" -E sleep 2)
endforeach()

message(FATAL_ERROR "模拟器启动超时")
