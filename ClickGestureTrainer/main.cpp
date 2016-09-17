#include <iostream>
#include <LEAP/Leap.h>
#include <GRT/GRT.h>
#include <ncurses.h>
#include "IndexSampleRecorder.h"
#include "MiddleSampleRecorder.h"
#include "Debouncer.h"

int kbhit(void) {
    int ch = getch();
    if (ch != ERR) {
        ungetch(ch);
        return 1;
    } else {
        return 0;
    }
}


int main(int argc, const char *argv[]) {
    if (argc < 3) {
        std::cout << "Click n_samples period" << std::endl;
        return 1;
    }
    GRT::GestureRecognitionPipeline pipeline;
    GRT::TimeSeriesClassificationData trainingData;
    trainingData.setNumDimensions(DIMENSION);
    trainingData.setAllowNullGestureClass(true);
    GRT::MatrixFloat timeSeries;
    timeSeries.reserve(100);
    Leap::Controller controller;
    SampleRecorder* indexSampleRecorder = new IndexSampleRecorder();
    SampleRecorder* middleSampleRecorder = new MiddleSampleRecorder();
    SampleRecorder* sampleRecorder = indexSampleRecorder;
    int n_samples = atoi(argv[1]);
    uint period = atoi(argv[2]);
    GRT::HMM hmm;
    uint label = 3;
    std::string fingerName = "Index";
    hmm.setHMMType(HMM_CONTINUOUS);
    hmm.setDownsampleFactor(5);
    hmm.setCommitteeSize(6);
    hmm.setAutoEstimateSigma(true);
    hmm.setSigma(20.0);
    hmm.setModelType(HMM_LEFTRIGHT);
    hmm.setDelta(1);
    hmm.enableNullRejection(true);
    pipeline.setClassifier(hmm);

    initscr();
    nodelay(stdscr, true);
    noecho();

    mvprintw(1, 0, "Press n to record a new sample (%s)",fingerName.c_str());
    mvprintw(0, 0, "0: Left click | 1: Right click",fingerName.c_str());
    while (1) {

        if (kbhit()) {
            switch (getch()) {
                case 'n': {
                    timeSeries.clear();
                    mvprintw(2, 0, "Recording for %1.1fs", period * n_samples * 0.001);
                    for (int i = 0; i < n_samples; i++) {
                        Leap::Frame frame = controller.frame();
                        if (frame.hands().count() > 0) {
                            GRT::VectorFloat sample = sampleRecorder->recordSample(frame);
                            timeSeries.push_back(sample);
                            mvprintw(3, 0, "Sampling: %d/%d", timeSeries.getNumRows(), n_samples);
                            refresh();
                        }
                        GRT::Util::sleep(period);
                    }
                    if (timeSeries.getNumRows() == n_samples) {
                        trainingData.addSample(label, timeSeries);
                        mvprintw(1, 0, "Samples: %d", trainingData.getNumSamples());
                    }
                    else {
                        mvprintw(7, 0, "Sample recording error, retry");
                        refresh();
                        sleep(1);
                    }

                    break;
                }
                case 't': {
                    erase();
                    endwin();
                    GRT::TimeSeriesClassificationData testData = trainingData.partition(80);
                    pipeline.train(trainingData);
                    sleep(1);
                    if (pipeline.getTrained()) {
                        pipeline.test(testData);
                        initscr();
                        nodelay(stdscr, true);
                        noecho();
                        mvprintw(0,0,"Trained in %2.2f",pipeline.getTrainingTime());
                        mvprintw(1,0,"Accuracy: %2.2f",pipeline.getTestAccuracy());
                        while(!kbhit()){}
                        getch();
                        pipeline.save("Pipeline");
                    }
                    else {
                        erase();
                        initscr();
                        nodelay(stdscr, true);
                        noecho();
                        mvprintw(0,0,"ERROR");
                    }
                    break;
                }
                case 'i' :
                    endwin();
                    trainingData.printStats();
                    sleep(3);
                    initscr();
                    break;
                case 'q' :
                    endwin();
                    return 0;
                case 's': {
                    std::string filename = fingerName + "TrainingData";
                    trainingData.save(filename);
                    break;
                }
                case 'l':{
                    std::string filename = fingerName+"TrainingData";
                    trainingData.load(filename);
                    break;
                }
                case '0':
                    fingerName = "Index";
                    sampleRecorder = indexSampleRecorder;
                    break;
                case '1':
                    fingerName = "Middle";
                    sampleRecorder = middleSampleRecorder;
                    break;
                case 'z': {
                    erase();
                    if (pipeline.load("Pipeline")){
                        mvprintw(0,0,"Loaded pipeline");
                    }
                    else {
                        mvprintw(0,0,"Error loading pipeline");
                    }
                    refresh();
                    uint counter = 0;
                    uint clickDebounced = 0;
                    uint clicks = 0;
                    Debouncer debouncer(500,3,0);
                    while(!kbhit()) {
                        Leap::Frame frame = controller.frame();
                        if (frame.hands().count() > 0) {
                            mvprintw(4,0,"Counter: %08d",++counter);
                            GRT::VectorFloat sample = sampleRecorder->recordSample(frame);
                            pipeline.predict(sample);
                            label = pipeline.getPredictedClassLabel();
                            mvprintw(1,0,"Unprocessed: %d",label);
                            uint debounced = debouncer.getLabel(label);
                            mvprintw(3,0,"Debouncer: %d",debounced);
                            if( debounced == 0 ){
                                mvprintw(10,0,"Debounced: %d",++clickDebounced);
                            }
                            if( label == 0 ){
                                mvprintw(11,0,"Clicks: %d",++clicks);
                            }
                            mvprintw(5,0,"Elapsed: %d",debouncer.getCurrElapsed());
                        }
                        usleep(period*1000);
                    }
                    break;
                }
            }
            erase();
            if(!pipeline.getTrained()) {
                mvprintw(0, 0, "Press n to record a new sample (%s)", fingerName.c_str());
                mvprintw(1, 0, "Samples: %d", trainingData.getNumSamples());
            }
            else {
                mvprintw(0, 0, "Press z to test", label);
            }
        }
        usleep(1000);
    }

}
