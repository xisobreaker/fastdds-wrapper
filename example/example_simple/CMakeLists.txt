project(example_simple)

include_directories(${CMAKE_SOURCE_DIR}/include)

aux_source_directory(. SRC_FILES)
add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} glog)
target_link_libraries(${PROJECT_NAME} fastddscpp)
