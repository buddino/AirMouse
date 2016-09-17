#include <iostream>
#include <ncurses.h>
#include <LEAP/Leap.h>
#include <GRT/GRT.h>
#include <libconfig.h++>
#include "StaticSampleRecorder.h"
#include "StaticSampleRecorderMoreFeatures.h"
#include "ClickDetector.h"
#include "MouseController.h"
#include "Debouncer.h"
#include "SampleRecorder.h"
#include "LeftClickSampleRecorder.h"
#include "RightClickSampleRecorder.h"

using namespace std;
using namespace libconfig;

int kbhit(void) {
    int ch = getch();
    if (ch != ERR) {
        ungetch(ch);
        return 1;
    } else {
        return 0;
    }
}


string className[] = {"INACTIVE", "ACTIVE", "PINCH"};

int main(int argc, char **argv) {
    if (argc < 2){
        std::cerr << "AirMouse <configuration>" << std::endl;
        return -1;
    }
    //Read configuration file passed as parameter
    Config conf;
    try
    {
        conf.readFile(argv[1]);
    }
    catch(const FileIOException &fioex)
    {
        std::cerr << "I/O error while reading file." << std::endl;
        return(EXIT_FAILURE);
    }
    catch(const ParseException &pex)
    {
        std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
        << " - " << pex.getError() << std::endl;
        return(EXIT_FAILURE);
    }

    Leap::Controller controller;
    GRT::GestureRecognitionPipeline pipeline;
    GRT::GestureRecognitionPipeline leftClickPipeline;
    GRT::GestureRecognitionPipeline rightClickPipeline;
    MouseController mouseController;
    //SampleRecorder* middleSampleRecorder = new MiddleSampleRecorder();
    SampleRecorder* indexSampleRecorder = new IndexSampleRecorder();
    Debouncer leftDebouncer((uint) conf.lookup("click-debounce"), 1, 0);
    //Debouncer rightDebouncer((int) conf.lookup("click-debounce"), 1, 0);
    Debouncer wheel((uint) conf.lookup("scroll-debounce"), 0, 1);

    int period = conf.lookup("sampling-period");
    SampleReader *sampleReader = new SampleReaderMoreFeatures();
    initscr();
    mvprintw(0, 0, "Enter pipeline filename");
    move(1, 0);
    //char str[80];
    //getstr(str);


    //Load static gestures pipeline (SVM)
    if (!pipeline.load(conf.lookup("pose-pipeline"))) {
        endwin();
        std::cerr << "Cannot find static gestures pipeline" << std::endl;
        return 1;
    }
    //Load left click pipeline (HMM)
    if (!leftClickPipeline.load(conf.lookup("leftclick-pipeline"))) {
        endwin();
        std::cerr << "Cannot find left click pipeline" << std::endl;
        return 1;
    }
    //Load right click pipeline (HMM)
    if (!rightClickPipeline.load(conf.lookup("rightclick-pipeline"))) {
        endwin();
        std::cerr << "Cannot find right click pipeline" << std::endl;
        return 1;
    }

    controller.enableGesture(Leap::Gesture::TYPE_CIRCLE);
    pipeline.addPostProcessingModule(GRT::ClassLabelFilter(8, 10));
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    scrollok(stdscr, TRUE);
    mvprintw(0, 0, "Testing Gesture Recognition");
    mvprintw(1, 0, "Loaded %d classes (%d dimensions)", pipeline.getNumClasses(),
             pipeline.getInputVectorDimensionsSize());
    mvprintw(2, 0, "o/p to change speed factor q to quit");
    int label = 0;
    int oldLabel = 0;
    Leap::Vector oldPosition(0, 0, 0);
    ClickDetector clickDetector(40, leftClickPipeline);
    int clicks = 0;
    chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();
    chrono::high_resolution_clock::time_point end;
    bool dragging = false;
    float shift_thresh = conf.lookup("shift-threshold");
    float tmp_shift_thresh = shift_thresh;
    float speedFactor = conf.lookup("base-speedfactor");
    float tmp_speedFactor = speedFactor;
    while (1) {
        if (kbhit()) {
            int ch = getch();
            switch (ch) {
                case 'q':
                    endwin();
                    return 0;
                case 'p':
                    speedFactor += 0.5;
                    if (speedFactor > 30) {
                        speedFactor = 30;
                    }
                    break;
                case 'o':
                    speedFactor -= 0.5;
                    if (speedFactor <= 0) {
                        speedFactor = 0.5;
                    }
                    break;
                default:break;
            }
        }
        Leap::Frame frame = controller.frame();
        mvprintw(6, 0, "Scale factor: %.2f", tmp_speedFactor);


        if (frame.hands().count() == 1) {
            GRT::VectorFloat sample = sampleReader->getSample(frame);
            if (sampleReader->isValid()) {
                Leap::Hand hand = frame.hands().frontmost();
                pipeline.predict(sample);
                oldLabel = label;
                label = pipeline.getPredictedClassLabel();
                move(3, 0);
                clrtoeol();
                printw("Label: %s", className[label].c_str());



                //RELEASED
                if (label != 2 && dragging) {
                    mouseController.drop();
                    move(10, 0);
                    clrtoeol();
                    mvprintw(10, 0, "RELEASED", mouseController.getTiming());
                    dragging = false;
                }

                //MOVING
                if (label == 1 & hand.isValid()) {

                    move(10, 0);
                    clrtoeol();
                    mvprintw(10, 0, "TRACKING", mouseController.getTiming());

                    //Left Click detector
                    leftClickPipeline.predict(indexSampleRecorder->recordSample(frame));
                    if (leftDebouncer.getLabel(leftClickPipeline.getPredictedClassLabel()) == 0) {
                        mouseController.click();
                    }

                    //Right Click detector
                    /*
                     * DISABLED FOR USABILITY TEST
                     * rightClickPipeline.predict(middleSampleRecorder->recordSample(frame));
                    if (rightDebouncer.getLabel(rightClickPipeline.getPredictedClassLabel()) == 0) {
                        mouseController.rightClick();
                    }
                     */


                    Leap::Vector newPosition = hand.palmPosition();
                    //Leap::Vector newPosition = hand.stabilizedPalmPosition();
                    if (oldLabel == 0) {
                        oldPosition = newPosition;
                    }
                    Leap::Vector shift = newPosition - oldPosition;
                    int x = round(shift[0] * tmp_speedFactor);
                    int y = round(shift[2] * tmp_speedFactor);

                    //DEADZONE
                    //Deadzone extension
                    //End extension
                    if (abs(shift[0]) > tmp_shift_thresh && abs(shift[2]) > tmp_shift_thresh) {
                        mouseController.move(x, y);
                        if (tmp_speedFactor < speedFactor) {
                            tmp_speedFactor += 0.5;
                        }
                        if (tmp_shift_thresh > shift_thresh) {
                            tmp_shift_thresh -= 0.05;
                        }
                    }
                    else {
                        if (tmp_speedFactor > 0.5) {
                            tmp_speedFactor -= 0.5;
                        }
                        if (tmp_shift_thresh < 0.05) {
                            tmp_shift_thresh += 0.001;
                        }
                        mouseController.move(0, 0);
                    }

                    mvprintw(5, 0, "Threshold: %.3f", tmp_shift_thresh);
                    mvprintw(4, 0, "Shift position: %.1f %.1f", shift[0], shift[2]);
                    mvprintw(7, 0, "Clicks: %d", mouseController.getLeftClicks());
                    //mvprintw(9, 0, "Timing: %d", mouseController.getTiming());
                    oldPosition = newPosition;
                }
                    //Start DRAGGING
                else if (label == 2 && !dragging) {
                    mouseController.drag();
                    dragging = true;
                }

                    //continue DRAGGING
                else if (label == 2 && dragging) {
                    move(10, 0);
                    clrtoeol();
                    mvprintw(10, 0, "DRAGGING", mouseController.getTiming());
                    Leap::Vector newPosition = hand.palmPosition();
                    if (oldLabel == 0) {
                        oldPosition = newPosition;
                    }
                    Leap::Vector shift = newPosition - oldPosition;
                    int x = (int) round(shift[0] * tmp_speedFactor);
                    int y = (int) round(shift[2] * tmp_speedFactor);
                    //Deadzone
                    if (abs(shift[0]) > tmp_shift_thresh && abs(shift[2]) > tmp_shift_thresh) {
                        mouseController.move(x, y);
                        if (tmp_speedFactor < speedFactor) {
                            tmp_speedFactor += 0.5;
                        }
                        if (tmp_shift_thresh > shift_thresh) {
                            tmp_shift_thresh -= 0.05;
                        }
                    }
                    else {
                        if (tmp_speedFactor > 0.5) {
                            tmp_speedFactor -= 0.5;
                        }
                        if (tmp_shift_thresh < 0.05) {
                            tmp_shift_thresh += 0.001;
                        }
                    }
                    mvprintw(5, 0, "Old position: %.1f %.1f", oldPosition[0], oldPosition[2]);
                    mvprintw(4, 0, "Shift position: %.1f %.1f", shift[0], shift[2]);
                    mvprintw(7, 0, "Clicks: %d", mouseController.getLeftClicks());
                    mvprintw(9, 0, "Timing: %d", mouseController.getTiming());
                    oldPosition = newPosition;
                }
                else {

                    if(frame.gestures().count()>0){
                        Leap::GestureList gestures = frame.gestures();
                        for (int i = 0; i < gestures.count(); i++){
                            if(gestures[i].type() == Leap::Gesture::TYPE_CIRCLE){
                                Leap::CircleGesture circle = gestures[i];
                                if(circle.progress() > (float) conf.lookup("circle-progress")) {
                                    float angle = circle.pointable().direction().angleTo(circle.normal());
                                    if (angle <= PI / 2) {
                                        if (wheel.getLabel(1) == 1)
                                            mouseController.scrollDown(circle.radius());
                                    }
                                    else {
                                        if (wheel.getLabel(1) == 1)
                                            mouseController.scrollUp(circle.radius());
                                    }
                                }
                            }
                        }
                    }
                    oldLabel = label;
                    label = 0;
                    move(10, 0);
                    clrtoeol();
                    mvprintw(10, 0, "NOTHING", mouseController.getTiming());
                }
            }
        }
        else {
            mouseController.drop();
            move(10, 0);
            clrtoeol();
            mvprintw(10, 0, "RELEASED");
            dragging = false;
        }
        usleep(period * 1000);
    }
}


