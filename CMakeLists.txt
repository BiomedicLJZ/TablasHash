cmake_minimum_required(VERSION 3.27)
project(TablasHash)

set(CMAKE_CXX_STANDARD 17)

add_executable(TablasHash main2.cpp
        HashTable.h)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
target_link_libraries(TablasHash ${GTEST_LIBRARIES})
