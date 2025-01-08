cmake_minimum_required(VERSION 3.25.0)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# PROJECT_VERSION_MAJOR year
# PROJECT_VERSION_MINOR month
# PROJECT_VERSION_PATCH day
# PROJECT_VERSION_TWEAK tweak
project(kmath VERSION 2025.01.07.0)

if(PROJECT_IS_TOP_LEVEL)
    cmake_host_system_information(RESULT HOST QUERY HOSTNAME)
    cmake_host_system_information(RESULT OSN QUERY OS_NAME)
    cmake_host_system_information(RESULT OS_VERSION QUERY OS_RELEASE)
    cmake_host_system_information(RESULT PROC QUERY PROCESSOR_DESCRIPTION)

    message(STATUS "-------------------------------------")
    message(STATUS "  Project: ${PROJECT_NAME}")
    message(STATUS "  Version: ${CMAKE_PROJECT_VERSION}")
    message(STATUS "  C ${CMAKE_CXX_STANDARD}")
    message(STATUS "-------------------------------------")
    message(STATUS " ${HOST}")
    message(STATUS " ${OSN}: ${OS_VERSION}")
    message(STATUS " ${PROC}")
    message(STATUS "-------------------------------------")

    set(BUILD_EXAMPLES ON)
    set(BUILD_GTESTS ON)
else()
    message(STATUS "-> ${PROJECT_NAME} is submodule")
    set(BUILD_EXAMPLES OFF)
    set(BUILD_GTESTS OFF)
endif()

# Library ---------------------------------------------------------
add_library(${PROJECT_NAME} INTERFACE src)
target_compile_definitions(${PROJECT_NAME} INTERFACE TYPE=float)
# target_compile_definitions(${PROJECT_NAME} INTERFACE KMATH_DEBUG=1)

# Examples -------------------------------------------------------
if(${BUILD_EXAMPLES})
    add_subdirectory(examples)
endif()

# Tests --------------------------------------------------------------
if(${BUILD_GTESTS})
    add_subdirectory(gtests)
endif()
