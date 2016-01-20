cmake_minimum_required(VERSION 3.0)

# Include SSVCmake.
list(APPEND CMAKE_MODULE_PATH
    "~/OHWorkspace/SSVCMake/cmake/modules/"
    "${CMAKE_SOURCE_DIR}/../SSVCMake/cmake/modules/"
    "${CMAKE_SOURCE_DIR}/extlibs/SSVCMake/cmake/modules/")

include(SSVCMake)

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

SSVCMake_setDefaults()
vrm_cmake_add_common_compiler_flags()

include_directories("./vrm/")
add_executable(${PROJECT_NAME} ${SRC_LIST})
SSVCMake_linkSFML()
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${SDL2_LIBRARY} ${SDL2_TTF_LIBRARY} ${SDL2_IMAGE_LIBRARY})

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_SOURCE_DIR})
