cmake_minimum_required(VERSION 3.10)

project(fastddscpp)

include_directories(${CMAKE_SOURCE_DIR}/include)

aux_source_directory(. DIR_SRCS)
add_library(${PROJECT_NAME} ${DIR_SRCS})

target_link_libraries(${PROJECT_NAME} glog)
target_link_libraries(${PROJECT_NAME} fastrtps)
