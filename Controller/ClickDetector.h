//
// Created by budda on 29/06/16.
//
#include <LEAP/Leap.h>
#include <GRT/GRT.h>

#ifndef TESTGESTURE_CLICKDETECTOR_H
#define TESTGESTURE_CLICKDETECTOR_H
class ClickDetector {
    private:
        GRT::MatrixFloat timeSeries;
        GRT::GestureRecognitionPipeline clickPipeline;
        uint win_size;
        uint null_class = 0;
        uint dimension = 5;
    public:
        ClickDetector(uint window_size,GRT::GestureRecognitionPipeline pipeline);
        uint add(GRT::VectorFloat sample);
        GRT::VectorFloat getSample(Leap::Hand hand);
};
#endif //TESTGESTURE_CLICKDETECTOR_H
