//
// Created by budda on 26/06/16.
//
#include <LEAP/Leap.h>
#include <GRT/GRT.h>

#ifndef INPUT_SAMPLEREADER_H
#define INPUT_SAMPLEREADER_H
class staticSampleReader {
    public:
    virtual GRT::VectorFloat getSample(Leap::Frame frame) = 0;
    void setDimension(int dim);
    int getDimension();
    bool isValid();

protected:
    uint dimension;
    int hand_id = -1;
    bool valid = false;
};
#endif //INPUT_SAMPLEREADER_H
