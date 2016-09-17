//
// Created by budda on 27/06/16.
//

#include "SampleReaderMoreFeatures.h"

SampleReaderMoreFeatures::SampleReaderMoreFeatures(){
    dimension = 20;
}
GRT::VectorFloat SampleReaderMoreFeatures::getSample(Leap::Frame frame) {
    GRT::VectorFloat sample(dimension);
    if(frame.hands().count() > 0){
        //Check hands are present
        Leap::Hand hand = frame.hand(hand_id);
        if( !hand.isValid() ){
            Leap::Hand hand = frame.hands().frontmost();
            hand_id = hand.id();
        }
        int n = 4;
        Leap::Vector palmPosition = hand.stabilizedPalmPosition();
        for (int i = 0; i < 5; i++) {
            sample[i*n] = hand.fingers()[i].direction().angleTo(hand.direction());
            Leap::Vector relativePosition = hand.fingers()[i].stabilizedTipPosition() - palmPosition;
            sample[i*n+1] = relativePosition[0];
            sample[i*n+2] = relativePosition[1];
            sample[i*n+3] = relativePosition[2];
        }
        sample[20] = hand.grabStrength();
        valid = true;
    }
    else {
        valid = false;
    }
    return sample;
}