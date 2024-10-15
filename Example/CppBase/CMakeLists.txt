cmake_minimum_required(VERSION 3.10)
project(CppBase)
file(GLOB_RECURSE header_files  ${CMAKE_CURRENT_SOURCE_DIR}/*.h )
source_group("Header Files" FILES ${header_files}) 
aux_source_directory(. SRCS)
add_executable(firstproject ${SRCS} ${header_files})

