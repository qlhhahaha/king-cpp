cmake_minimum_required(VERSION 3.10)  #cmake版本最低3.10

project(lab1_json)  #添加当前项目名称

#设置包含当前目录
SET(CMAKE_INCLUDE_CURRENT_DIR ON) 

add_executable(${PROJECT_NAME})

#向工程添加源码
target_sources(${PROJECT_NAME} PRIVATE
    main.cpp
    kjson.cpp
    kjson.h
)


#向工程添加多个特定头文件的搜索路径——   -I参数
target_include_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}
)
