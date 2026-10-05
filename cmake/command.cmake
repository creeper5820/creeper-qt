# 构建命令目标：clang-format / clang-tidy

# 在 PATH 中查找 program 以及 program-* 形式的候选，运行 --version 解析版本号，
# 返回版本号最高的可执行文件，避免硬编码具体版本名
function(creeper_pick_latest out_exe out_version program)
    file(TO_CMAKE_PATH "$ENV{PATH}" path_list)
    set(candidates)
    foreach(dir IN LISTS path_list)
        if(NOT dir)
            continue()
        endif()
        file(GLOB found "${dir}/${program}" "${dir}/${program}-*")
        list(APPEND candidates ${found})
    endforeach()
    list(REMOVE_DUPLICATES candidates)

    set(best_exe "")
    set(best_version "0")
    foreach(candidate IN LISTS candidates)
        if(IS_DIRECTORY "${candidate}")
            continue()
        endif()
        execute_process(
            COMMAND "${candidate}" --version
            OUTPUT_VARIABLE version_output
            ERROR_QUIET
            RESULT_VARIABLE version_result
        )
        if(NOT version_result EQUAL 0)
            continue()
        endif()
        string(REGEX MATCH "[0-9]+\\.[0-9]+\\.[0-9]+" version "${version_output}")
        if(version AND version VERSION_GREATER best_version)
            set(best_version "${version}")
            set(best_exe "${candidate}")
        endif()
    endforeach()

    set(${out_exe} "${best_exe}" PARENT_SCOPE)
    set(${out_version} "${best_version}" PARENT_SCOPE)
endfunction()

creeper_pick_latest(CLANG_FORMAT_EXECUTABLE CLANG_FORMAT_VERSION clang-format)
creeper_pick_latest(CLANG_TIDY_EXECUTABLE CLANG_TIDY_VERSION clang-tidy)

# run-clang-tidy 没有版本化命名，普通查找即可
find_program(RUN_CLANG_TIDY_EXECUTABLE NAMES run-clang-tidy run-clang-tidy.py)

# 排除生成目录（如 example/android/build），避免把第三方/生成代码纳入
set(CREEPER_QT_GENERATED_DIRS "/build/" "/cache/" "/.cache/")

# 格式化范围：仓库内全部 .cc / .hh（含独立构建的 test/）
file(
    GLOB_RECURSE CREEPER_QT_FORMAT_SOURCES
    CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hh"
        "${CMAKE_CURRENT_SOURCE_DIR}/example/*.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/example/*.hh"
        "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/test/*.hh"
)
foreach(pattern IN LISTS CREEPER_QT_GENERATED_DIRS)
    list(FILTER CREEPER_QT_FORMAT_SOURCES EXCLUDE REGEX "${pattern}")
endforeach()
# 聚合头文件由 CMake 自动生成，无需格式化
list(FILTER CREEPER_QT_FORMAT_SOURCES EXCLUDE REGEX "/${PROJECT_NAME}/${PROJECT_NAME}\\.hh$")

# 检查范围：编译数据库中的 .cc 翻译单元，与实际构建范围一致
set(CREEPER_QT_TIDY_SOURCES)
set(CREEPER_QT_COMPILE_COMMANDS "${CMAKE_BINARY_DIR}/compile_commands.json")
if(EXISTS "${CREEPER_QT_COMPILE_COMMANDS}")
    file(READ "${CREEPER_QT_COMPILE_COMMANDS}" _commands)
    if(NOT _commands STREQUAL "")
        string(JSON _count LENGTH "${_commands}")
        if(_count GREATER 0)
            math(EXPR _last "${_count} - 1")
            foreach(_i RANGE 0 ${_last})
                string(JSON _entry GET "${_commands}" ${_i})
                string(JSON _file GET "${_entry}" file)
                list(APPEND CREEPER_QT_TIDY_SOURCES "${_file}")
            endforeach()
        endif()
        list(REMOVE_DUPLICATES CREEPER_QT_TIDY_SOURCES)
        list(FILTER CREEPER_QT_TIDY_SOURCES EXCLUDE REGEX "_autogen")
    endif()
endif()

if(CLANG_FORMAT_EXECUTABLE)
    add_custom_target(
        clang-format
        COMMAND "${CLANG_FORMAT_EXECUTABLE}" -i ${CREEPER_QT_FORMAT_SOURCES}
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        COMMENT "Running clang-format ${CLANG_FORMAT_VERSION}"
        USES_TERMINAL
        VERBATIM
    )
else()
    message(WARNING "未找到 clang-format，clang-format 目标将直接报错")
    add_custom_target(
        clang-format
        COMMAND "${CMAKE_COMMAND}" -E echo "clang-format 未找到，请先安装"
        COMMAND "${CMAKE_COMMAND}" -E false
    )
endif()

if(CLANG_TIDY_EXECUTABLE AND CREEPER_QT_TIDY_SOURCES)
    if(RUN_CLANG_TIDY_EXECUTABLE)
        # 优先并行
        add_custom_target(
            clang-tidy
            COMMAND "${RUN_CLANG_TIDY_EXECUTABLE}" -p "${CMAKE_BINARY_DIR}" -quiet
                    ${CREEPER_QT_TIDY_SOURCES}
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
            COMMENT "Running run-clang-tidy ${CLANG_TIDY_VERSION}"
            USES_TERMINAL
            VERBATIM
        )
    else()
        message(WARNING "未找到 run-clang-tidy，clang-tidy 改为串行运行")
        add_custom_target(
            clang-tidy
            COMMAND "${CLANG_TIDY_EXECUTABLE}" -p "${CMAKE_BINARY_DIR}"
                    ${CREEPER_QT_TIDY_SOURCES}
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
            COMMENT "Running clang-tidy ${CLANG_TIDY_VERSION}"
            USES_TERMINAL
            VERBATIM
        )
    endif()
else()
    message(WARNING "clang-tidy 或编译数据库不可用，clang-tidy 目标将直接报错")
    add_custom_target(
        clang-tidy
        COMMAND "${CMAKE_COMMAND}" -E echo "clang-tidy 或编译数据库不可用"
        COMMAND "${CMAKE_COMMAND}" -E false
    )
endif()
