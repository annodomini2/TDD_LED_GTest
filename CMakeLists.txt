cmake_minimum_required(VERSION 3.25)
project(LedDriver VERSION 0.1.0)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME) # Builds test suite when local
    # GoogleTest requires at least C++14
    set(CMAKE_CXX_STANDARD 14)

    # Add -O0 to remove optimizations when using gcc
    IF(CMAKE_COMPILER_IS_GNUCC)
       set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -Wall -fprofile-arcs -ftest-coverage")
       set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -Wall -fprofile-arcs -ftest-coverage")
    ENDIF(CMAKE_COMPILER_IS_GNUCC)

    include(CTest)
    enable_testing()

    add_executable(${PROJECT_NAME}_test)
    add_subdirectory(util)
    add_subdirectory(LedDriver)
    add_subdirectory(test)

    target_include_directories(${PROJECT_NAME}_test PRIVATE "${PROJECT_SOURCE_DIR}")

    target_link_libraries(${PROJECT_NAME}_test LedDriver util)

    add_test(LedDriver ${PROJECT_NAME}_test)

else() # Builds as a library when integrated in another project
    add_subdirectory(${PROJECT_NAME})

    set(CPACK_PROJECT_NAME ${PROJECT_NAME})
    set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
    include(CPack)
endif()