#include <ncurses.h>
#include "FeatureExtractor.h"

FeatureExtractor::FeatureExtractor(staticSampleReader *sampleReader) {
    trainingData.setNumDimensions(sampleReader->getDimension());
    this->sampleReader = sampleReader;
    mvprintw(0,0,"Dataset dim: %d (%d)",trainingData.getNumDimensions(),sampleReader->getDimension());
}

void FeatureExtractor::start() { t = std::thread(&FeatureExtractor::RecordData, this); }

void FeatureExtractor::pause() { pause_thread = true; }

void FeatureExtractor::resume() { pause_thread = false; }

void FeatureExtractor::stop() { stop_thread = true; }

GRT::ClassificationData FeatureExtractor::getTrainingData() { return trainingData; }

void FeatureExtractor::setLabel(int label) { this->label = label; }

void FeatureExtractor::RecordData() {
    while (!stop_thread) {
        erase();
        mvprintw(0, 0, "Select label by pressing the correspondent number:");
        mvprintw(1, 2, "0: %s", LABEL[0]);
        mvprintw(2, 2, "1: %s", LABEL[1]);
        mvprintw(3, 2, "2: %s", LABEL[2]);
        mvprintw(4, 2, "Selected label: %d (%s)",label,LABEL[label]);
        mvprintw(5, 0, "Put your hand on the leap motion controller");
        mvprintw(6, 0, "Press r to start recording and p to pause recording, press t when finished");

        mvprintw(8, 0, "Samples: %d", trainingData.getNumSamples());
        pause_thread ? mvprintw(9,0, "Status: PAUSED") : mvprintw(9,0, "Status: RECORDING");

        if (!pause_thread) {
            Leap::Frame frame = controller.frame();
            GRT::VectorFloat sample = sampleReader->getSample(frame);
            if (sampleReader->isValid()) {
                trainingData.addSample(label, sample);
            }
            else {
                mvprintw(9,0, "Status: NO HAND DETECTED ABOVE THE CONTROLLER");
            }

        }
        usleep(period*1000);
    }
}

void FeatureExtractor::setSamplingPeriod(int period) {
    this->period = period;
}





