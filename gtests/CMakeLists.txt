
include(FetchContent)
FetchContent_Declare(
    gtest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)
FetchContent_MakeAvailable(gtest)

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