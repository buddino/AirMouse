//
// Created by budda on 30/06/16.
//
#include <LEAP/Leap.h>
#include <GRT/GRT.h>
#include "SampleRecorder.h"

#define THUMB 0
#define INDEX 1
#define MIDDLE 2
#define RING 3
#define PINKY 4
#define DIMENSION 4

#ifndef HMMTEST_INDEXSAMPLERECORDER_H
#define HMMTEST_INDEXSAMPLERECORDER_H

class IndexSampleRecorder : public SampleRecorder {
    GRT::VectorFloat recordSample(Leap::Frame frame);
};
#endif
