/*
  ==============================================================================

    DSP.cpp
    Created: 17 Dec 2018 8:53:44pm
    Author:  Stijn de Koning

  ==============================================================================
*/

#include "DSP.h"

double noiseOsc::noiseBrown() {
    // brown noise
    float r1 = rand()/(float)RAND_MAX / 10;
    output = (oldOutput + r1) * 0.9;
    oldOutput = output;
    return(output * 4) - 2;
}
