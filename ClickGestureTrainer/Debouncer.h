//
// Created by budda on 30/06/16.
//

#ifndef HMMTEST_CLICKDETECTOR_H
#define HMMTEST_CLICKDETECTOR_H

#include <LEAP/Leap.h>
#include <GRT/GRT.h>
#include <chrono>

class Debouncer {
public:
    Debouncer(uint t, uint nl, uint cl); //Timeout in ms
    uint getTimeout();
    void setTimeout(uint t);
    uint getLabel(uint predicted);
    void clear();
    uint getCurrElapsed();

private:
    std::chrono::high_resolution_clock::time_point start,end;
    uint timeout;
    uint null_label, click_label;

};


#endif //HMMTEST_CLICKDETECTOR_H
