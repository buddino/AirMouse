//
// Created by budda on 23/06/16.
//
#include <LEAP/Leap.h>
#include <GRT/GRT.h>
#include <thread>
#include "StaticSampleReader.h"

#ifndef INPUT_FEATUREEXTRACTOR_H
#define INPUT_FEATUREEXTRACTOR_H

class FeatureExtractor {
public:
    FeatureExtractor(staticSampleReader * sampleReader);
    void start();
    void pause();
    void resume();
    void stop();
    void setLabel(int label);
    GRT::ClassificationData getTrainingData();
    void setSamplingPeriod(int period);
private:
    int label = 0;
    int period = 10;
    staticSampleReader* sampleReader;
    std::thread t;
    GRT::ClassificationData trainingData;
    Leap::Controller controller;
    bool pause_thread = true;
    bool stop_thread = false;
    void RecordData();
    GRT::VectorFloat frame2sample(Leap::Frame frame);
    const char *LABEL[3] = {"NOTHING","MOVING","DRAGGING"};
};



#endif //INPUT_FEATUREEXTRACTOR_H
