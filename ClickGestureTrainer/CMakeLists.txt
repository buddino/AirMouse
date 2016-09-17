cmake_minimum_required(VERSION 3.5)
project(ClickGestureTrainer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES main.cpp IndexSampleRecorder.h IndexSampleRecorder.cpp MiddleSampleRecorder.h MiddleSampleRecorder.cpp Debouncer.cpp Debouncer.h SampleRecorder.cpp SampleRecorder.h)
add_executable(ClickGestureTrainer ${SOURCE_FILES})
target_link_libraries(ClickGestureTrainer ncurses Leap grt)