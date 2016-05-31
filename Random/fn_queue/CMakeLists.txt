cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)


list(APPEND CMAKE_MODULE_PATH
    "/home/vittorioromeo/OHWorkspace/SSVCMake/cmake/modules/")

include(SSVCMake)
vrm_cmake_init_project(fn_queue)

include_directories("/home/vittorioromeo/OHWorkspace/vrm_pp/include")
include_directories("/home/vittorioromeo/OHWorkspace/vrm_core/include")

vrm_check_target()

add_subdirectory(test)

# add_executable(main "./main.cpp")

