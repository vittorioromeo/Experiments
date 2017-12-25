cmake_minimum_required(VERSION 3.0)

list(APPEND CMAKE_MODULE_PATH
    "/home/vittorioromeo/OHWorkspace/vrm_cmake/cmake/"
    "${CMAKE_SOURCE_DIR}/../vrm_cmake/cmake/"
    "${CMAKE_SOURCE_DIR}/extlibs/vrm_cmake/cmake/")

include(vrm_cmake)

# Basic project setup.
vrm_cmake_init_project(emscripten_tests)
vrm_cmake_find_extlib_in(vrm_pp "../..")
vrm_cmake_find_extlib_in(vrm_core "../..")
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLUT REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
vrm_cmake_add_common_compiler_flags()

file(GLOB_RECURSE SRC_LIST "./src/*")
add_executable(${PROJECT_NAME} ${SRC_LIST})
include_directories("./include")
include_directories(${VRM_PP_INCLUDE_DIR})
include_directories(${VRM_CORE_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${SDL2_LIBRARY} ${SDL2_TTF_LIBRARY} ${SDL2_IMAGE_LIBRARY})

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_SOURCE_DIR})
