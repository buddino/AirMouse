#include "FrameReader.h"

std::string fingerNames[] = {"Pollice", "Indice", "Medio", "Anulare", "Mignolo"};

FrameReader::FrameReader() {
    mvprintw(0, 0, "Label: ?");
    trainingData.setNumDimensions(DIMENSION);
}

void FrameReader::start() { t = std::thread(&FrameReader::ThreadMain, this); }

void FrameReader::pause() { pause_thread = true; }

void FrameReader::resume() { pause_thread = false; }

void FrameReader::stop() { stop_thread = false; }

void FrameReader::ThreadMain() {
    while (!stop_thread) {
        Leap::Frame frame = controller.frame();
        mvprintw(2, 0, "Hands: %d (Hand id: %d)", frame.hands().count(), hand_id);
        if (!pause_thread) {
            if (frame.hands().count() > 0) {
                GRT::VectorFloat sample(DIMENSION);
                if (hand_id == -1) {
                    hand_id = frame.hands().frontmost().id();
                }
                Leap::Hand hand = frame.hand(hand_id);
                if (hand.isValid()) {

                /*
                    Leap::Vector palmPosition = hand.stabilizedPalmPosition();
                    Leap::FingerList fingerList = hand.fingers();
                    //For each finger compute palm center distance using stabilized positions
                    for (int f = 0; f < 5; f++) {
                        /*Leap::Finger finger = fingerList[f];
                        Leap::Vector tipPosition = finger.stabilizedTipPosition();
                        Leap::Vector tipPositionRelToPalm = tipPosition - palmPosition;
                        std::string s = fingerNames[f] + std::string(": %.2f\t%.2f\t%.2f");
                        mvprintw(5 + f, 0, s.c_str(), tipPositionRelToPalm[0], tipPositionRelToPalm[1],
                                 tipPositionRelToPalm[2]);
                        sample[3 * f] = tipPositionRelToPalm[0];
                        sample[3 * f + 1] = tipPositionRelToPalm[0];
                        sample[3 * f + 1] = tipPositionRelToPalm[0];
                    }
                 */
                    for (int i = 0; i < 5; i++) {
                        sample[i] = hand.fingers()[i].direction().angleTo(hand.direction());
                        mvprintw(i+3, 0, "%2.2f", sample[i]);
                    }
                    sample[6] = hand.grabStrength();
                    trainingData.addSample(label, sample);
                    mvprintw(10, 0, "Samples: %d", ++samples);
                }
                else { hand_id = -1; }
            }
            else {
                hand_id = -1;
            }
        }
        usleep(ms * 10000);

    }
}

int FrameReader::getLabel() {
    return label;
}

void FrameReader::setLabel(int label) {
    if(label == 0)
        mvprintw(0, 0, "Label: NOTHING");
    else
    mvprintw(0, 0, "Label: TRACKING");

    this->label = label;
}

void FrameReader::save() {
    trainingData.saveDatasetToFile("ClassificationData.grt");
}

GRT::ClassificationData FrameReader::getTrainingData(){
    return trainingData;
}

GRT::VectorFloat FrameReader::getSample() {
    Leap::Frame frame = controller.frame();
    GRT::VectorFloat sample(DIMENSION);
    if (frame.hands().count() > 0) {
        Leap::Hand hand = frame.hands().frontmost();
        for (int i = 0; i < 5; i++) {
            sample[i] = hand.fingers()[i].direction().angleTo(hand.direction());
        }
        sample[6] = hand.grabStrength();
    }
    return sample;
}
