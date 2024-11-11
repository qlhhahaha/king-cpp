cmake_minimum_required(VERSION 3.10)  #cmake版本最低3.10

project(circle)  #添加当前项目名称

#设置包含当前目录
SET(CMAKE_INCLUDE_CURRENT_DIR ON) 

add_executable(${PROJECT_NAME})

#向工程添加源码
target_sources(${PROJECT_NAME} PRIVATE
    main.cpp
    circle.cpp
    circle.h
)
