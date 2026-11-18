cmake_minimum_required(VERSION 3.15)
project(test)

file(GLOB SRC ${PROJECT_SOURCE_DIR}/src/*.cpp)

set(EXECUTABLE_OUTPUT_PATH ..)
add_executable(app ${SRC})
