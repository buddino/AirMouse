//
// Created by budda on 06/07/16.
//

#ifndef HMMTEST_SAMPLERECORDER_H
#define HMMTEST_SAMPLERECORDER_H


#include <LEAP/Leap.h>
#include <GRT/GRT.h>

class SampleRecorder {
public:
    virtual GRT::VectorFloat recordSample(Leap::Frame frame) = 0;
};


#endif //HMMTEST_SAMPLERECORDER_H
