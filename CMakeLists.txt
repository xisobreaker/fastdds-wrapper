cmake_minimum_required (VERSION 3.0)

project(simplest-fast-dds)

# 设定编译选项
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置输出路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

find_package(fastcdr REQUIRED)
find_package(fastrtps REQUIRED)

add_subdirectory(example)
add_subdirectory(src)
