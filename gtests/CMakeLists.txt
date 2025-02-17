
find_package(GTest REQUIRED)
if(GTest_FOUND)
    message(STATUS "=> Found gtest")
else()
    message(STATUS "*** Didn't find gtest")
endif()

enable_testing()

list(APPEND gtests
    matrix_test.cpp
    # map_test.cpp
)

add_executable(test-all ${gtests})
target_link_libraries(test-all GTest::gtest_main ${PROJECT_NAME})
target_include_directories(test-all PRIVATE ${PROJECT_SOURCE_DIR}/src/)
include(GoogleTest)
gtest_discover_tests(test-all)