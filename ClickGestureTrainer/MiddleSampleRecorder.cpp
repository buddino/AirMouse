//
// Created by budda on 30/06/16.
//

#include "SampleRecorder.h"
#include "MiddleSampleRecorder.h"

GRT::VectorFloat MiddleSampleRecorder::recordSample(Leap::Frame frame)  {
    GRT::VectorFloat sample(DIMENSION);
    if (frame.hands().count() > 0) {
        Leap::Hand hand = frame.hands().frontmost();

        sample[0] = hand.fingers()[MIDDLE].tipVelocity()[1] - hand.fingers()[THUMB].tipVelocity()[1];
        sample[1] = hand.fingers()[MIDDLE].tipVelocity()[1] - hand.fingers()[INDEX].tipVelocity()[1];
        sample[2] = hand.fingers()[MIDDLE].tipVelocity()[1] - hand.fingers()[RING].tipVelocity()[1];
        sample[3] = hand.fingers()[MIDDLE].tipVelocity()[1] - hand.fingers()[PINKY].tipVelocity()[1];
        /*
        Leap::Vector d_pollice = hand.fingers()[0].tipPosition() - hand.fingers()[1].tipPosition();
        Leap::Vector d_medio = hand.fingers()[2].tipPosition() - hand.fingers()[1].tipPosition();
        Leap::Vector d_palmo = hand.stabilizedPalmPosition() - hand.fingers()[1].tipPosition();
        sample[12] = (hand.fingers()[0].tipPosition() - hand.fingers()[1].tipPosition())[0];
        sample[13] = (hand.fingers()[0].tipPosition() - hand.fingers()[1].tipPosition())[1];
        sample[14] = (hand.fingers()[0].tipPosition() - hand.fingers()[1].tipPosition())[2];

        sample[15] = (hand.fingers()[2].tipPosition() - hand.fingers()[1].tipPosition())[0];
        sample[16] = (hand.fingers()[2].tipPosition() - hand.fingers()[1].tipPosition())[1];
        sample[17] = (hand.fingers()[2].tipPosition() - hand.fingers()[1].tipPosition())[2];

        sample[18] = (hand.fingers()[3].tipPosition() - hand.fingers()[1].tipPosition())[0];
        sample[19] = (hand.fingers()[3].tipPosition() - hand.fingers()[1].tipPosition())[1];
        sample[20] = (hand.fingers()[3].tipPosition() - hand.fingers()[1].tipPosition())[2];

        sample[21] = (hand.fingers()[4].tipPosition() - hand.fingers()[1].tipPosition())[0];
        sample[22] = (hand.fingers()[4].tipPosition() - hand.fingers()[1].tipPosition())[1];
        sample[23] = (hand.fingers()[4].tipPosition() - hand.fingers()[1].tipPosition())[2];
        */
        return sample;
    }
}

