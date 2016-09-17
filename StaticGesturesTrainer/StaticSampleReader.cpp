#include "StaticSampleReader.h"

int staticSampleReader::getDimension() {
    return dimension;
}

void staticSampleReader::setDimension(int dim) {
    dimension = dim;
}

bool staticSampleReader::isValid() {
    return valid;
}




