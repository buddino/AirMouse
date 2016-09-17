//
// Created by budda on 29/06/16.
//
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include <chrono>

#ifndef TESTGESTURE_MOUSECONTROLLER_H
#define TESTGESTURE_MOUSECONTROLLER_H


class MouseController {
public:
    MouseController();
    void click();
    void rightClick();
    void move(int x, int y);
    void drag();
    void drop();
    void scrollDown(float radius);
    void scrollUp(float radius);
    uint getLeftClicks();
    uint getRightClicks();
    uint getTiming();
private:
    Display* commonDisplay;
    XEvent event;
    uint left_clicks = 0;
    uint right_clicks = 0;
    std::chrono::high_resolution_clock::time_point start, end;
    void _click();
};


#endif //TESTGESTURE_MOUSECONTROLLER_H
