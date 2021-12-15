cmake_minimum_required(VERSION 3.16)
project(reference_generator VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)

find_package(yaml-cpp CONFIG REQUIRED)

add_subdirectory(apps)
add_subdirectory(lib)