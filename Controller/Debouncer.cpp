//
// Created by budda on 30/06/16.
//

#include "Debouncer.h"
using namespace std::chrono;
Debouncer::Debouncer(uint t,uint nl, uint cl) {
    start = high_resolution_clock::now();
    click_label = cl;
    null_label = nl;
    timeout = t;
}

uint Debouncer::getTimeout() {
    return timeout;
}

void Debouncer::setTimeout(uint t) {
    timeout = t;
}

uint Debouncer::getCurrElapsed() {
    return duration_cast<milliseconds>(end-start).count();
}

uint Debouncer::getLabel(uint predicted) {
    end = high_resolution_clock::now();
    if (predicted == click_label ) {
        if (getCurrElapsed() > timeout ) {
            start = end;
            return click_label;
        }
    }
        return null_label;
}

void Debouncer::clear() {
    start = high_resolution_clock::now();
}











