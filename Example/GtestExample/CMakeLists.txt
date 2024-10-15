cmake_minimum_required(VERSION 3.10)

project(GtestExample)

add_executable(${PROJECT_NAME})

find_package(GTest CONFIG REQUIRED)

#向工程添加源码
target_sources(${PROJECT_NAME} PRIVATE
    test.cpp
)

#向工程添加头文件路径 -I
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../PersonCpp
)

# 链接 Google Test 到测试目标
target_link_libraries(${PROJECT_NAME} 
    GTest::gmock 
	GTest::gtest 
	GTest::gmock_main 
	GTest::gtest_main
    PersonCpp
  )
