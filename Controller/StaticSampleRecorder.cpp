#include "StaticSampleRecorder.h"

int SampleReader::getDimension() {
    return dimension;
}

void SampleReader::setDimension(int dim) {
    dimension = dim;
}

bool SampleReader::isValid() {
    return valid;
}




