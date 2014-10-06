cmake_minimum_required(VERSION 2.8)
project(SSVVM)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/extlibs/SSVCMake/cmake/modules/;${CMAKE_MODULE_PATH}")
include(SSVCMake)

SSVCMake_setDefaults()
SSVCMake_findExtlib(SSVUtils)

add_executable(${PROJECT_NAME} ${SRC_LIST})
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/_RELEASE/)
