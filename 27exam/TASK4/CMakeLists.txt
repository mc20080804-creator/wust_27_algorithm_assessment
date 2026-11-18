cmake_minimum_required(VERSION 3.16)
project(Task4 LANGUAGES CXX)

add_executable(task4 main.cpp)
target_compile_features(task4 PRIVATE cxx_std_17)