
list(APPEND apps
    matrix_ex.cpp
    matrix_understanding_ex.cpp
    map_ex.c
)

foreach(FILE ${apps})
    get_filename_component(name ${FILE} NAME_WE)
    add_executable(${name} ${FILE})
    target_include_directories(${name} PRIVATE ${PROJECT_SOURCE_DIR}/src/)
    target_link_libraries(${name} ${PROJECT_NAME})
    # target_compile_definitions(${name} INTERFACE KMATH_DEBUG=1)
endforeach()