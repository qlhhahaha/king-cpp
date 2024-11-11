cmake_minimum_required(VERSION 3.10)

project(circletest)

add_executable(${PROJECT_NAME})

find_package(GTest CONFIG REQUIRED)

#向工程添加源码
target_sources(${PROJECT_NAME} PRIVATE
	circle.h
	circle.cpp
	test.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/../circle	
)

target_link_libraries(${PROJECT_NAME} PRIVATE
	circleLib
	GTest::gmock
	GTest::gtest
)