cmake_minimum_required(VERSION 3.16)
project(Task5 LANGUAGES CXX)

add_executable(task5 main.cpp simulator.cpp my_scheduler.cpp)

target_compile_features(task5 PRIVATE cxx_std_17)