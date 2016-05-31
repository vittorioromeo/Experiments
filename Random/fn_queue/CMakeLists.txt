cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)

include_directories("/home/vittorioromeo/OHWorkspace/vrm_pp/include")
include_directories("/home/vittorioromeo/OHWorkspace/vrm_core/include")

add_executable(main "./main.cpp")

