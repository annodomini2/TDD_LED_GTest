cmake_minimum_required(VERSION 3.25)
project(LedDriver_test VERSION 0.1.0)

include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.12.0
)

FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

target_sources(LedDriver_test PRIVATE test_led.cpp)

add_subdirectory(mocks)

target_link_libraries(LedDriver_test
    GTest::GTest
    RunTimeErrorStub
)