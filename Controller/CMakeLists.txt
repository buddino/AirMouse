cmake_minimum_required(VERSION 3.5)
project(AirMouse)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES main.cpp StaticSampleRecorder.h
        StaticSampleRecorder.cpp SampleReaderA.h SampleReaderA.cpp
        StaticSampleRecorderMoreFeatures.cpp StaticSampleRecorderMoreFeatures.h
        ClickDetector.h ClickDetector.cpp MouseController.cpp MouseController.h
        Debouncer.cpp Debouncer.h SampleRecorder.cpp SampleRecorder.h
        LeftClickSampleRecorder.cpp LeftClickSampleRecorder.h RightClickSampleRecorder.cpp RightClickSampleRecorder.h
        )
add_executable(AirMouse ${SOURCE_FILES})
target_link_libraries(AirMouse ncurses pthread Leap grt X11 Xtst config++)