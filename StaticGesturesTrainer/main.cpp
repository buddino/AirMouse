#include <ncurses.h>
#include <unistd.h>  /* only for sleep() */
#include <functional>
#include <chrono>
#include <future>
#include <iostream>
#include "FrameReader.h"
#include "StaticSampleReader.h"
#include "SampleReaderMoreFeatures.h"
#include "FeatureExtractor.h"
using namespace std::chrono;


int kbhit(void) {
    int ch = getch();
    if (ch != ERR) {
        ungetch(ch);
        return 1;
    } else {
        return 0;
    }
}

/**
 * If no parameter default sampling value of 10ms is used, otherwise the value (in ms) passed as parameter is used
 */

int main(int argc, char* argv[]) {

    initscr();
    int mx, my;
    getmaxyx(stdscr, my, mx);
    staticSampleReader* sampleReader = new SampleReaderMoreFeatures();
    FeatureExtractor* featureExtractor = new FeatureExtractor(sampleReader);
    GRT::GestureRecognitionPipeline pipeline;
    if(argc >1){
        int period = atoi(argv[1]);
        if(period > 0){
            featureExtractor->setSamplingPeriod(period);
        }
    }

    int row, col;
    getmaxyx(stdscr,row,col);

    //Ask for trainer name
    erase();
    mvprintw(0,0,"AirMouse training");
    mvprintw(1,0,"Insert trainer name: ");
    nodelay(stdscr, FALSE);
    echo();
    char name[80];
    wgetnstr(stdscr,name,80);

    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    scrollok(stdscr, TRUE);

    featureExtractor->start();


    while (1) {
        if (kbhit()) {
            switch (getch()) {
                case '1':
                    featureExtractor->setLabel(TRACKING);
                    break;
                case '0':
                    featureExtractor->setLabel(NOTHING);
                    break;
                case '2':
                    featureExtractor->setLabel(DRAGGING);
                    break;
                case 'r':
                    featureExtractor->resume();
                    break;
                case 'p':
                    featureExtractor->pause();
                    break;
                case 'e':
                    featureExtractor->stop();
                    endwin();
                    return 0;
                case 't': {
                    featureExtractor->stop();
                    GRT::ClassificationData trainingData = featureExtractor->getTrainingData();
                    mvprintw(0, 0, "Total samples: %d", trainingData.getNumSamples());
                    mvprintw(1, 0, "Training", trainingData.getNumSamples());
                    GRT::SVM svm;
                    svm.enableNullRejection(true);
                    svm.setKernelType(GRT::RBF);
                    pipeline.setClassifier(svm);
                    endwin();
                    pipeline.train(trainingData);
                    initscr();
                    erase();
                    if ( pipeline.getTrained() ){
                        mvprintw(0,0,"Training time: %.2f seconds",pipeline.getTrainingTime());
                        mvprintw(1,0,"Test accuracy: %.2f",pipeline.getCrossValidationAccuracy());
                        mvprintw(3,0,"Press j to test");
                    }
                    else {
                        mvprintw(0,0,"Error during training");
                    }
                    nodelay(stdscr, TRUE);
                    noecho();
                    break;
                }

                case 'j':{
                    featureExtractor->stop();
                    erase();
                    if(!pipeline.getTrained()){
                        mvprintw(0,0,"Insert pipeline filename to load");
                        move(1,0);
                        nodelay(stdscr, FALSE);
                        echo();
                        char str[80];
                        wgetnstr(stdscr,str,80);
                        pipeline.load(str);
                        noecho();
                        nodelay(stdscr,TRUE);
                    }


                    Leap::Controller controller;
                    erase();
                    mvprintw(0,0,"Press s to save training data or e to exit");
                    std::string status;
                    while(!kbhit()) {
                        GRT::VectorFloat sample = sampleReader->getSample(controller.frame());
                        pipeline.predict(sample);
                        if(pipeline.getPredictedClassLabel()==1){
                            status = "MOVING";
                        }
                        else if (pipeline.getPredictedClassLabel()==2){
                            status = "DRAGGING";
                        }
                        else {
                            status = "NOTHING";
                        }
                        move(row/2-1,0); clrtoeol();
                        move(row/2,0); clrtoeol();
                        mvprintw(row/2-1,(col-strlen("GESTURE STATUS:"))/2,"GESTURE STATUS:");
                        attron(A_BOLD);
                        mvprintw(row/2,(col-strlen(status.c_str()))/2,"%s",status.c_str());
                        attroff(A_BOLD);
                        usleep(1000*10);
                    }
                    char key = getch();
                    if(key=='s'){
                        featureExtractor->getTrainingData().save(name);
                        erase();
                        mvprintw(0,0,"Saved to %s",name);
                        mvprintw(1,0,"Exiting",name);
                        refresh();
                        sleep(1);
                        endwin();
                        return 0;
                    }
                }
            }
            refresh();
        }
        usleep(10000);
    }

}