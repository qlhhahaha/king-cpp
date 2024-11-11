cmake_minimum_required(VERSION 3.10)  #cmake版本最低3.10



if(WIN32)

    set(CMAKE_SYSTEM_VERSION "10.0.18362.0")

endif()



SET(CMAKE_TOOLCHAIN_FILE  "${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty_install/vcpkg/scripts/buildsystems/vcpkg.cmake")



project(qlh-project)  #添加当前项目名称



set(CMAKE_CXX_STANDARD 11) #设置C++标准



# 设置输出目录为顶层 bin 文件夹

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")


set(EXCLUDE_DIRS ".git")


# 获取当前路径

set(CURRENT_DIR ${CMAKE_SOURCE_DIR})

# 将CURRENT_DIR下的所有文件夹和文件都放到ALL_ITEMS中

# GLOB：获取当前目录下的所有文件和文件夹

# RELATIVE：获取相对路径

# GLOB_RECURSE：获取当前目录下的所有文件和文件夹，包括子文件夹

file(GLOB ALL_ITEMS RELATIVE ${CURRENT_DIR} ${CURRENT_DIR}/*)

message(STATUS "ALL_ITEMS: ${ALL_ITEMS}")



# 遍历所有的文件

# 通过IS_DIRECTORY判断是否为目录

set(DIRECTORIES "")

foreach(ITEM ${ALL_ITEMS})

    # 判断是否为目录

    if(IS_DIRECTORY ${CMAKE_SOURCE_DIR}/${ITEM})

        # 判断当前目录是否在排除的文件夹列表中

        set(EXCLUDE_FLAG OFF)

        foreach(EXCLUDE_DIR ${EXCLUDE_DIRS})

            # STREQUAL 操作符，它用于比较两个字符串是否完全相同。

            if(${ITEM} STREQUAL ${EXCLUDE_DIR})

                set(EXCLUDE_FLAG ON)

                break()

            endif()

        endforeach()

        # 如果当前目录不在排除的文件夹列表中

        if(NOT EXCLUDE_FLAG)

            list(APPEND DIRECTORIES ${ITEM})

            # 添加子目录

            add_subdirectory(${CURRENT_DIR}/${ITEM})

        endif()

    endif()

endforeach()


set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT QtDemo)  #设置启动项