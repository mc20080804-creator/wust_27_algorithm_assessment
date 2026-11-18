cmake_minimum_required(VERSION 3.16)
project(Task6 LANGUAGES CXX)

add_executable(task6 main.cpp fake_serial.cpp protocol.cpp)
target_compile_features(task6 PRIVATE cxx_std_17)