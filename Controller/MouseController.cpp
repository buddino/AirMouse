//
// Created by budda on 29/06/16.
//

#include <cstdlib>
#include <cstring>
#include <zconf.h>
#include <cmath>
#include "MouseController.h"

void MouseController::move(int x, int y) {
    if (commonDisplay == NULL) {
        exit(EXIT_FAILURE);
    }
    XWarpPointer(commonDisplay, None, None, 0, 0, 0, 0, x, y);
    XFlush(commonDisplay);
}

MouseController::MouseController() {
    commonDisplay = XOpenDisplay(NULL);
    start = std::chrono::high_resolution_clock::now();
}

void MouseController::click() {
    left_clicks++;
    if (commonDisplay == NULL) {
        exit(EXIT_FAILURE);
    }
    XTestFakeButtonEvent(commonDisplay, 1, True, CurrentTime);
    usleep(1000);
    XTestFakeButtonEvent(commonDisplay, 1, False, CurrentTime);
    XFlush(commonDisplay);
}

void MouseController::rightClick() {
    right_clicks++;
    if (commonDisplay == NULL) {
        exit(EXIT_FAILURE);
    }
    XTestFakeButtonEvent(commonDisplay, 3, True, CurrentTime);
    usleep(1000);
    XTestFakeButtonEvent(commonDisplay, 3, False, CurrentTime);
    XFlush(commonDisplay);
}

void MouseController::drag() {
    if (commonDisplay == NULL) {
        exit(EXIT_FAILURE);
    }
    XTestFakeButtonEvent(commonDisplay, 1, True, CurrentTime);
    XFlush(commonDisplay);

}

void MouseController::drop() {
    if (commonDisplay == NULL) {
        exit(EXIT_FAILURE);
    }
    XTestFakeButtonEvent(commonDisplay, 1, False, CurrentTime);
    XFlush(commonDisplay);

}

uint MouseController::getLeftClicks() {
    return left_clicks;
}
uint MouseController::getRightClicks() {
    return right_clicks;
}

uint MouseController::getTiming(){
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

void MouseController::scrollDown(float radius) {
    int steps = round(radius/20);
    for(int s = 0; s < steps; s++) {
        XTestFakeButtonEvent(commonDisplay, 5, True, CurrentTime);
        XTestFakeButtonEvent(commonDisplay, 5, False, CurrentTime);
    }
    XFlush(commonDisplay);
}

void MouseController::scrollUp(float radius) {
    int steps = round(radius/20);
    for(int s = 0; s < steps; s++) {
        XTestFakeButtonEvent(commonDisplay, 4, True, CurrentTime);
        XTestFakeButtonEvent(commonDisplay, 4, False, CurrentTime);
    }
    XFlush(commonDisplay);
}



















