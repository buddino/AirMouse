//
// Created by budda on 21/06/16.
//
#include <LEAP/Leap.h>
#include <thread>
#include <GRT/GRT.h>
#include <ncurses.h>
#ifndef INPUT_FRAMEREADER_H
#define INPUT_FRAMEREADER_H
#define TRACKING 1
#define NOTHING 0
#define DRAGGING 2
#define DIMENSION 6
class FrameReader{
public:
    FrameReader();
    void start();
    void pause();
    void resume();
    void stop();
    void setLabel(int label);
    int getLabel();
    void save();
    GRT::VectorFloat getSample();
    GRT::ClassificationData getTrainingData();
private:
    int hand_id = -1;
    GRT::ClassificationData trainingData;
    Leap::Controller controller;

    int label = NOTHING;
    bool pause_thread = true;
    bool stop_thread = false;
    std::thread t;
    void ThreadMain();
    int samples = 0;
    int ms = 10;
};

#endif //INPUT_FRAMEREADER_H
