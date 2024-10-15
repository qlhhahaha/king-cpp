cmake_minimum_required(VERSION 3.10)

project(GMockExample)

add_executable(${PROJECT_NAME})

find_package(GTest CONFIG REQUIRED)

#向工程添加源码
target_sources(${PROJECT_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/foo.h
)

#向工程添加多个特定头文件的搜索路径——   -I参数
target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/person
)

# 链接 Google Test 到测试目标
target_link_libraries(${PROJECT_NAME} PUBLIC
    GTest::gmock 
	GTest::gtest 
	GTest::gmock_main 
	GTest::gtest_main
  )

