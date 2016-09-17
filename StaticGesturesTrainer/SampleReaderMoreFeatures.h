//
// Created by budda on 27/06/16.
//

#ifndef INPUT_SAMPLEREADERMOREFEATURES_H
#define INPUT_SAMPLEREADERMOREFEATURES_H

#include "StaticSampleReader.h"
class SampleReaderMoreFeatures : public staticSampleReader {
    public:
    SampleReaderMoreFeatures();
    virtual GRT::VectorFloat getSample(Leap::Frame frame);
};


#endif //INPUT_SAMPLEREADERMOREFEATURES_H
