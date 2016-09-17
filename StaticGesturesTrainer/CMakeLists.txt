cmake_minimum_required(VERSION 3.5)
project(INPUT)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES main.cpp FrameReader.h FrameReader.cpp FeatureExtractor.h FeatureExtractor.cpp StaticSampleReader.h StaticSampleReader.cpp SampleReaderMoreFeatures.cpp SampleReaderMoreFeatures.h)
add_executable(INPUT ${SOURCE_FILES} FrameReader.h FrameReader.cpp)
target_link_libraries(INPUT ncurses pthread Leap grt)