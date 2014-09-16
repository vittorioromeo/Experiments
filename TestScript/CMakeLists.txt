cmake_minimum_required(VERSION 2.8)

set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type.")
project(TestScript)

set(CMAKE_CXX_FLAGS "-std=c++1y -Wall -Wextra -W -pedantic -pthread" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -O3" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_DEBUG "-fno-omit-frame-pointer -g3 -gdwarf-2 -O0" CACHE STRING "" FORCE)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../cmake/modules/;${CMAKE_MODULE_PATH}")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules/;${CMAKE_MODULE_PATH}")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/extlibs/SSVUtils/cmake/modules/;${CMAKE_MODULE_PATH}")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/extlibs/SSVUtilsJson/cmake/modules/;${CMAKE_MODULE_PATH}")

find_package(SSVUtils REQUIRED)
find_package(SSVUtilsJson REQUIRED)

set(SRC_DIR "src")
set(INC_DIR "include")
file(GLOB_RECURSE SRC_LIST "${INC_DIR}/*" "${SRC_DIR}/*")

include_directories("./include/")
include_directories(${SSVUTILS_INCLUDE_DIR})
include_directories(${SSVUTILSJSON_INCLUDE_DIR})
add_executable(${PROJECT_NAME} ${SRC_LIST})

if(UNIX)
	install(TARGETS TestScript RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/games/TestScript/)
	install(TARGETS TestScript RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/_RELEASE/)
	install(DIRECTORY ${CMAKE_SOURCE_DIR}/_RELEASE/ DESTINATION ${CMAKE_INSTALL_PREFIX}/games/TestScript/)
	install(PROGRAMS ${CMAKE_SOURCE_DIR}/_RELEASE/TestScript DESTINATION ${CMAKE_INSTALL_PREFIX}/games/TestScript/ PERMISSIONS WORLD_EXECUTE WORLD_READ)
else(WIN32)
	install(TARGETS TestScript RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/_RELEASE/)
endif()
