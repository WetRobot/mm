cmake_minimum_required(VERSION 3.14)
project(my_project)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/master.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
    tests
    tests/tests.cpp
)

find_package(OpenMP REQUIRED)

target_include_directories(
    tests PRIVATE ${CMAKE_SOURCE_DIR}/include/
)

target_link_libraries(
    tests
    GTest::gtest_main
    OpenMP::OpenMP_CXX
)

include(GoogleTest)
gtest_discover_tests(tests)
