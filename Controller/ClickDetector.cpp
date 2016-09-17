#include "ClickDetector.h"

#define THUMB 0
#define INDEX 1
#define MIDDLE 2
#define RING 3
#define PINKY 4

ClickDetector::ClickDetector(uint window_size, GRT::GestureRecognitionPipeline pipeline) {
    clickPipeline = pipeline;
    win_size = window_size;
}

uint ClickDetector::add(GRT::VectorFloat sample) {
    timeSeries.push_back(sample);
    if (timeSeries.getNumRows() == win_size+1) {
        GRT::MatrixFloat ts = timeSeries;
        timeSeries.clear();
        for (int i = 0; i < win_size; i++) {
            timeSeries.push_back(ts.getRowVector(i + 1));
        }
        if (!clickPipeline.predict(timeSeries)) {
            //TODO Errore
        }
        uint predictedClass = clickPipeline.getPredictedClassLabel();
        return predictedClass;
    }
    return null_class;
}

GRT::VectorFloat ClickDetector::getSample(Leap::Hand hand) {
    GRT::VectorFloat sample(dimension);
    sample[0] = hand.fingers()[INDEX].tipVelocity()[1] - hand.fingers()[THUMB].tipVelocity()[1];
    sample[1] = hand.fingers()[INDEX].tipVelocity()[1] - hand.fingers()[MIDDLE].tipVelocity()[1];
    sample[2] = hand.fingers()[INDEX].tipVelocity()[1] - hand.fingers()[RING].tipVelocity()[1];
    sample[3] = hand.fingers()[INDEX].tipVelocity()[1] - hand.fingers()[PINKY].tipVelocity()[1];
    sample[4] = hand.fingers()[INDEX].tipVelocity()[1] - hand.palmVelocity()[1];
    return sample;
}