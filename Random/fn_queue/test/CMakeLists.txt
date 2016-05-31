add_custom_target(tests COMMENT "Build all the unit tests.")
add_dependencies(check tests)
vrm_cmake_generate_unit_tests_glob("${CMAKE_SOURCE_DIR}/test/*.cpp")
