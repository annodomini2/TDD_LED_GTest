cmake_minimum_required(VERSION 3.0.0)
project(RunTimeErrorStub VERSION 0.1.0)

add_library(RunTimeErrorStub)

target_sources(RunTimeErrorStub 
    PRIVATE RunTimeErrorStub.c 
    PUBLIC FILE_SET HEADERS 
    BASE_DIRS ${PROJECT_SOURCE_DIR}
    FILES RunTimeErrorStub.h
)