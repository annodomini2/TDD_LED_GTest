cmake_minimum_required(VERSION 3.25)
project(LedDriver VERSION 0.1.0)

# Add -O0 to remove optimizations when using gcc
IF(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -Wall -fprofile-arcs -ftest-coverage")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -Wall -fprofile-arcs -ftest-coverage")
ENDIF(CMAKE_COMPILER_IS_GNUCC)

add_library(${PROJECT_NAME})

target_sources(${PROJECT_NAME} 
    PRIVATE ${PROJECT_NAME}.c 
    PUBLIC FILE_SET HEADERS 
    BASE_DIRS ${PROJECT_SOURCE_DIR}
    FILES ${PROJECT_NAME}.h
)

target_link_libraries(${PROJECT_NAME} util)