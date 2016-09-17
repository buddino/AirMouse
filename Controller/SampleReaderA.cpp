#include <LEAP/Leap.h>
#include <GRT/GRT.h>
#include "SampleReaderA.h"

SampleReaderA::SampleReaderA(){
    dimension = 6;
}
GRT::VectorFloat SampleReaderA::getSample(Leap::Frame frame) {

    GRT::VectorFloat sample(dimension);
    if(frame.hands().count() > 0){
        //Check hands are present
        Leap::Hand hand = frame.hand(hand_id);
        if( !hand.isValid() ){
            Leap::Hand hand = frame.hands().frontmost();
            hand_id = hand.id();
        }
        for (int i = 0; i < 5; i++) {
            sample[i] = hand.fingers()[i].direction().angleTo(hand.direction());
        }
        sample[5] = hand.grabStrength();
        valid = true;
    }
    else {
        valid = false;
    }
    return sample;
}