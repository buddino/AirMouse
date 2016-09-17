//
// Created by budda on 26/06/16.
//

#ifndef INPUT_SAMPLEREADERA_H
#define INPUT_SAMPLEREADERA_H

#include "StaticSampleRecorder.h"

class SampleReaderA : public SampleReader {
public:
    SampleReaderA();
    virtual GRT::VectorFloat getSample(Leap::Frame frame);
};


#endif //INPUT_SAMPLEREADERA_H
