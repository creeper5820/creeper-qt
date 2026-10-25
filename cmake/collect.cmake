# 收集组件头文件（layout / widget），用于生成 props 聚合头
file(
    GLOB_RECURSE CREEPER_QT_COMPONENT_HEADERS
    CONFIGURE_DEPENDS
    RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "src/${PROJECT_NAME}/layout/*.hh"
    "src/${PROJECT_NAME}/widget/*.hh"
)
list(FILTER CREEPER_QT_COMPONENT_HEADERS EXCLUDE REGEX "\\.impl\\.hh$")
list(SORT CREEPER_QT_COMPONENT_HEADERS)

# 从组件头文件提取 `namespace <component>::pro`（`creeper::` 前缀可选）
set(CREEPER_QT_PROPS_COMPONENTS "")
foreach(header IN LISTS CREEPER_QT_COMPONENT_HEADERS)
    file(READ "${CMAKE_CURRENT_SOURCE_DIR}/src/${header}" _content)
    string(
        REGEX MATCHALL
        "namespace[ \t]+(creeper::)?([A-Za-z_][A-Za-z0-9_]*)::pro[ \t]*\\{"
        _matches "${_content}"
    )
    foreach(_match IN LISTS _matches)
        string(
            REGEX REPLACE
            "namespace[ \t]+(creeper::)?([A-Za-z_][A-Za-z0-9_]*)::pro[ \t]*\\{"
            "\\2" _component "${_match}"
        )
        list(APPEND CREEPER_QT_PROPS_COMPONENTS "${_component}")
    endforeach()
endforeach()
list(REMOVE_DUPLICATES CREEPER_QT_PROPS_COMPONENTS)
list(REMOVE_ITEM CREEPER_QT_PROPS_COMPONENTS nav_host)
list(SORT CREEPER_QT_PROPS_COMPONENTS)

# 计算简称：多词取各词首字母 + p；单词取前若干字母 + p，冲突时逐步加长
set(CREEPER_QT_PROPS_ALIASES "")
set(_used_aliases "")
foreach(_component IN LISTS CREEPER_QT_PROPS_COMPONENTS)
    string(REGEX REPLACE "^_+" "" _clean "${_component}")
    string(REPLACE "_" ";" _words "${_clean}")
    list(LENGTH _words _word_count)
    if(_word_count GREATER 1)
        set(_stem "")
        foreach(_word IN LISTS _words)
            string(SUBSTRING "${_word}" 0 1 _first)
            string(APPEND _stem "${_first}")
        endforeach()
        set(_alias "${_stem}p")
    else()
        set(_alias "")
        string(LENGTH "${_clean}" _name_len)
        set(_len 2)
        while(_len LESS_EQUAL _name_len)
            string(SUBSTRING "${_clean}" 0 ${_len} _stem)
            set(_candidate "${_stem}p")
            list(FIND _used_aliases "${_candidate}" _found)
            if(_found EQUAL -1)
                set(_alias "${_candidate}")
                break()
            endif()
            math(EXPR _len "${_len} + 1")
        endwhile()
    endif()
    list(APPEND CREEPER_QT_PROPS_ALIASES "${_alias}")
    list(APPEND _used_aliases "${_alias}")
endforeach()
list(LENGTH CREEPER_QT_PROPS_COMPONENTS _component_count)
math(EXPR _component_last "${_component_count} - 1")

# 生成 props-alias.hh：各组件 pro 的命名空间别名
set(CREEPER_QT_PROPS_ALIAS_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/src/${PROJECT_NAME}/props-alias.hh")
set(_alias_content
"/**\n\
 * @file props-alias.hh\n\
 * @brief 各组件的 pro 命名空间简称，供调用方按组件消歧后使用属性。\n\
 *\n\
 * 本文件由 CMake 在配置阶段自动生成，请勿手动修改。\n\
 * 新增、删除或重命名组件后，重新运行 cmake 即可自动更新此文件。\n\
 */\n\
\n\
#pragma once\n\
\n")
foreach(header IN LISTS CREEPER_QT_COMPONENT_HEADERS)
    string(APPEND _alias_content "#include <${header}>\n")
endforeach()
# 对齐 `=` 与行尾注释，保持 clang-format 风格
set(_max_alias 0)
foreach(_index RANGE 0 ${_component_last})
    list(GET CREEPER_QT_PROPS_ALIASES ${_index} _alias)
    string(LENGTH "${_alias}" _alias_len)
    if(_alias_len GREATER _max_alias)
        set(_max_alias ${_alias_len})
    endif()
endforeach()

set(_max_lhs 0)
foreach(_index RANGE 0 ${_component_last})
    list(GET CREEPER_QT_PROPS_COMPONENTS ${_index} _component)
    list(GET CREEPER_QT_PROPS_ALIASES ${_index} _alias)
    string(LENGTH "${_alias}" _alias_len)
    math(EXPR _alias_pad "${_max_alias} - ${_alias_len}")
    string(REPEAT " " ${_alias_pad} _alias_spaces)
    string(LENGTH "namespace ${_alias}${_alias_spaces} = ${_component}::pro;" _lhs_len)
    if(_lhs_len GREATER _max_lhs)
        set(_max_lhs ${_lhs_len})
    endif()
endforeach()

string(APPEND _alias_content "\nnamespace creeper::props_alias {\n\n")
foreach(_index RANGE 0 ${_component_last})
    list(GET CREEPER_QT_PROPS_COMPONENTS ${_index} _component)
    list(GET CREEPER_QT_PROPS_ALIASES ${_index} _alias)
    string(LENGTH "${_alias}" _alias_len)
    math(EXPR _alias_pad "${_max_alias} - ${_alias_len}")
    string(REPEAT " " ${_alias_pad} _alias_spaces)
    set(_lhs "namespace ${_alias}${_alias_spaces} = ${_component}::pro;")
    string(LENGTH "${_lhs}" _lhs_len)
    math(EXPR _comment_pad "${_max_lhs} - ${_lhs_len} + 1")
    string(REPEAT " " ${_comment_pad} _comment_spaces)
    string(APPEND _alias_content "${_lhs}${_comment_spaces}// NOLINT(misc-unused-alias-decls)\n")
endforeach()
string(APPEND _alias_content "\n}\n")
file(WRITE "${CREEPER_QT_PROPS_ALIAS_HEADER}" "${_alias_content}")

# 生成 props-scope.hh：聚合全部组件 pro 的命名空间
set(CREEPER_QT_PROPS_SCOPE_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/src/${PROJECT_NAME}/props-scope.hh")
set(_scope_content
"/**\n\
 * @file props-scope.hh\n\
 * @brief 聚合全部组件的 pro 命名空间，供调用方直接使用属性名。\n\
 *\n\
 * 本文件由 CMake 在配置阶段自动生成，请勿手动修改。\n\
 * 新增、删除或重命名组件后，重新运行 cmake 即可自动更新此文件。\n\
 */\n\
\n\
#pragma once\n\
\n")
foreach(header IN LISTS CREEPER_QT_COMPONENT_HEADERS)
    string(APPEND _scope_content "#include <${header}>\n")
endforeach()
string(APPEND _scope_content "\nnamespace creeper::props_scope {\n\n")
foreach(_index RANGE 0 ${_component_last})
    list(GET CREEPER_QT_PROPS_COMPONENTS ${_index} _component)
    string(APPEND _scope_content "using namespace ${_component}::pro;\n")
endforeach()
string(APPEND _scope_content "\n}\n")
file(WRITE "${CREEPER_QT_PROPS_SCOPE_HEADER}" "${_scope_content}")

# 收集公共头文件并生成聚合头文件
file(
    GLOB_RECURSE CREEPER_QT_PUBLIC_HEADERS
    CONFIGURE_DEPENDS
    RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "src/${PROJECT_NAME}/*.hh"
)

# 排除实现头文件与聚合头文件本身
list(FILTER CREEPER_QT_PUBLIC_HEADERS EXCLUDE REGEX "\\.impl\\.hh$")
list(FILTER CREEPER_QT_PUBLIC_HEADERS EXCLUDE REGEX "^${PROJECT_NAME}/${PROJECT_NAME}\\.hh$")
list(SORT CREEPER_QT_PUBLIC_HEADERS)

set(CREEPER_QT_UMBRELLA_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/src/${PROJECT_NAME}/${PROJECT_NAME}.hh")
set(CREEPER_QT_UMBRELLA_CONTENT
"
// 本文件由 CMake 在配置阶段自动生成，请勿手动修改。
// 新增、删除或重命名 src/creeper-qt 下的 .hh 头文件后，
// 重新运行 cmake 即可自动更新此文件。

#pragma once

")

foreach(header IN LISTS CREEPER_QT_PUBLIC_HEADERS)
    string(APPEND CREEPER_QT_UMBRELLA_CONTENT "#include <${header}>\n")
endforeach()

file(WRITE "${CREEPER_QT_UMBRELLA_HEADER}" "${CREEPER_QT_UMBRELLA_CONTENT}")

# 收集 Widgets 模块源文件
file(
    GLOB_RECURSE WIDGETS_SOURCE
    CONFIGURE_DEPENDS
        "src/${PROJECT_NAME}/core/*.cc"
        "src/${PROJECT_NAME}/layout/*.cc"
        "src/${PROJECT_NAME}/utility/*.cc"
        "src/${PROJECT_NAME}/widget/*.cc"
        # 自定义信号头文件
        "src/${PROJECT_NAME}/widget/sliders.hh"
)
