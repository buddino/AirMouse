cmake_minimum_required(VERSION 3.5)
project(MergeTrainingSet)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES main.cpp)
add_executable(MergeTrainingSet ${SOURCE_FILES})
target_link_libraries(MergeTrainingSet grt)