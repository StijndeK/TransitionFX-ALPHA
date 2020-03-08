/*
  ==============================================================================

    Envelopes.cpp
    Created: 31 Oct 2018 1:40:53pm
    Author:  Stijn de Koning

  ==============================================================================
*/

#include "Envelopes.h"

//==============================================================================

// TODO: clean up header
// TODO: use switch for delay and dars

//==============================================================================
double Envelopes::arLin12Steps(double input, int trigger)
{
    switch(currentEnvState) {
        case ATTACK:
            for (int i = 0; i < 7; i++){
                if (attackPhase[i] == 1) {
                    amplitude += (1 * attackLinSteps[i]);
                    
                    // smoothing
                    if (smooth == true){
                        newOutput = amplitude;
                        oldOutput = 0;
        
                        if (first50check == true) {
                            for (int i = 0; i < smoothAmount; i++){
                                oldOutput = oldOutput + oldOutputsArray[i];
                            }
                            newOutput = (((newOutput * 3) + ((oldOutput / smoothAmount) * 7)) / 10); //
                        }
                        output = newOutput * input;;
                        currentOutput = currentOutput + 1;
                        if (currentOutput > smoothAmount - 1){
                            first50check = true;
                            currentOutput = 0;
                        }
        
                        // fill array
                        oldOutputsArray[currentOutput] = newOutput;
                    }
                    // no smoothing
                    else {
                        newOutput = amplitude;
                        output    = newOutput * input;
                    }
                }
                if (amplitude >= points[i] && attackPhase[i] == 1) {
                    if (attackPhase[6] != 1){
                        amplitude = points[i];
                        attackPhase[i] = 0;
                        attackPhase[i+1] = 1;
                    }
                    else if (attackPhase[6] == 1){
                        attackPhase[6] = 0;
                        amplitude = 1;
                        currentEnvState = HOLD;
                    }
                }
            }
            break;
        case HOLD:
            output = input;
            amplitude = 1.;
            if (trigger!=1) {
                currentEnvState = RELEASE;
                releasePhase[0] = 1;
            }
            break;
        case RELEASE:
            for (int i = 0; i < 7; i++){
                if (releasePhase[i] == 1) {
                    amplitude -= releaseLinSteps[i];
                    
                    // smoothing
                    if (smooth == true){
                        newOutput = amplitude;
                        oldOutput = 0;
        
                        for (int i = 0; i < smoothAmount; i++){
                            oldOutput = oldOutput + oldOutputsArray[i];
                        }
                        newOutput = (((newOutput * 3) + ((oldOutput / smoothAmount) * 7)) / 10); //
                        output = newOutput * input;;
                        currentOutput = currentOutput + 1;
                        if (currentOutput > smoothAmount - 1){
                            currentOutput = 0;
                        }
        
                        // fill array
                        oldOutputsArray[currentOutput] = newOutput;
                    }
                    // no smoothing
                    else {
                        newOutput = amplitude;
                        output = newOutput * input;
                    }
                }
                if (amplitude <= releasePoints[i] && releasePhase[i] == 1) {
                    if (releasePhase[6] != 1) {
                        amplitude = releasePoints[i];
                        releasePhase[i] = 0;
                        releasePhase[i + 1] = 1;
                    }
                    else if (releasePhase[6] == 1) {
                        releasePhase[6] = 0;
                        amplitude = 0;
                        currentEnvState = STOP;
                    }
                }
            }
            break;
        case STOP:
            amplitude = 0.0;
            if (trigger == 1) {
                newOutput       = 0.0;
                oldOutput       = 0; // not sure if necessary
                first50check    = false; // for smoothing
                attackPhase[0] = 1;
                currentOutput = -1; // what? for smoothing?
                for (int i = 0; i < 1000; i++) { // what? for smoothing?
                    oldOutputsArray[i] = 0.0;
                }
                currentEnvState = ATTACK;
            }
            break;
    }
    
    return output;
}


//==============================================================================
double Envelopes::arGemiddelde(double input, int trigger)
{
    switch(currentEnvState) {
        case ATTACK:
            amplitudeExp*= attackExp;
            amplitudeLin+=(1*attackLin);
            amplitude = ((amplitudeExp * verhoudingGemiddelde) + (amplitudeLin * (100.0 - verhoudingGemiddelde))) / 100.0;
            if (amplitude>=1) {
                amplitude=1;
                currentEnvState = HOLD;
            }
            break;
        case HOLD:
            amplitude = 1;
            if (trigger!=1) {
                currentEnvState = RELEASE;
            }
            break;
        case RELEASE:
            amplitudeExp*= releaseExp;
            amplitudeLin-=(1*releaseLin);
            amplitude = ((amplitudeExp * verhoudingGemiddelde) + (amplitudeLin * (100.0 - verhoudingGemiddelde))) / 100.0;
            if (amplitude<=amplitudeStartValue) {
                amplitude = 0;
                currentEnvState = STOP;
            }
            break;
        case STOP:
            amplitude = 0.0;
            if (trigger == 1) {
                amplitudeExp = amplitudeStartValue;
                amplitudeLin = 0.0;
                currentEnvState = ATTACK;
            }
            break;
    }
    
    output = input*amplitude;
    return output;
}

//==============================================================================
double Envelopes::arExp(double input, int trigger)
{
    switch(currentEnvState) {
        case ATTACK:
            amplitude*= attackExp;
            if (amplitude>=1) {
                amplitude=1;
                currentEnvState = HOLD;
            }
            break;
        case HOLD:
            amplitude = 1;
            if (trigger!=1) {
                currentEnvState = RELEASE;
            }
            break;
        case RELEASE:
            amplitude*= releaseExp;
            if (amplitude<=amplitudeStartValue) {
                amplitude = 0;
                currentEnvState = STOP;
            }
            break;
        case STOP:
            amplitude = 0.0;
            if (trigger == 1) {
                amplitude = amplitudeStartValue;
                currentEnvState = ATTACK;
            }
            break;
    }

    output = input*amplitude;
    return output;
}


//==============================================================================
double Envelopes::arLin(double input, int trigger)
{
    switch(currentEnvState) {
        case ATTACK:
            amplitude+=(1*attackLin);
            if (amplitude>=1) {
                amplitude=1;
                currentEnvState = HOLD;
            }
            break;
        case HOLD:
            amplitude = 1;
            if (trigger!=1) {
                currentEnvState = RELEASE;
            }
            break;
        case RELEASE:
            amplitude-=(1*releaseLin);
            if (amplitude<=0) {
                amplitude = 0;
                currentEnvState = STOP;
            }
            break;
        case STOP:
            amplitude = 0.0;
            if (trigger == 1) {
                currentEnvState = ATTACK;
            }
            break;
    }
    
    output = input*amplitude;
    return output;
}

//==============================================================================
double Envelopes::dars(double input, int trigger)
{
    //initialise
    if (trigger == 1 && attackphase != 1 && holdphase != 1 && sustainphase != 1 && waitPhase != 1){
        holdcount       = 0;
        releasephase    = 0;
        attackphase     = 0;
        sustainphase    = 0;
        currentsample   = 0;
        amplitude       = 0.0;
        waitPhase       = 1;
        
        std::cout << "darstrigger" << std::endl;
        dLength = setLength(delayLength);
        Alength = setLength(attackLength);
    }
    
    // delay
    if (waitPhase == 1) {
        output        = 0.0;
        currentsample = currentsample + 1;
    }
    if (currentsample >= dLength && waitPhase == 1) {
        waitPhase   = 0;
        amplitude   = 0.0;
        attackphase = 1;
    }
    
    // go through attacks
    if (attackphase == 1) {
        amplitude       += (1 * attackLin);
        output          =  input * amplitude;
        currentsample   =  currentsample + 1;
    }
    if (amplitude >= 1 && attackphase == 1) {
        amplitude       = 1;
        attackphase     = 0;
        sustainphase    = 1;
    }
    if (sustainphase == 1) {
        amplitude       = 1;
        currentsample   = currentsample + 1;
    }
    if (sustainphase == 1 && currentsample >= Alength){
        sustainphase = 0;
        holdphase    = 1;
    }
    // if note is held down
    if (holdcount < holdtime && holdphase == 1) {
        output  = input;
        holdcount++;
    }
    if (holdcount == holdtime && trigger == 1) {
        output = input;
    }
    if (holdcount == holdtime && trigger != 1) {
        holdphase       = 0;
        releasephase    = 1;
    }
    
    // check if release is on or off
    if (darsRelease == true) {
        // check for normal or reverse release
        if (darsReverseRelease == false) {
            if (releasephase == 1 && amplitude > 0.) {
                amplitude -=    (1 * releaseLin);
                output    =     input * amplitude;
            }
        }
        else {
            if (releasephase == 1 && amplitude <= 2) {
                amplitude += (1 * releaseLin);
                output    = input * (amplitude);
            }
        }
    }

    return output;
}

//==============================================================================
// env that sends a trigger when the attack is finished
double Envelopes::delay(double input, int trigger)
{
    // initialise
    if (trigger == 1 && waitPhase != 1){
        waitPhase = 1;
        currentsample = 0;
        releasephase = 0;
        holdcount = 0;
        delayInput = input;
    }
    // wait for attack to finish
    if (waitPhase == 1) {
        output = 0.0;
        currentsample = currentsample + 1;
    }
    // output 1, then set it 0 again
    double Alength = setLength(attackLength);
    if (currentsample >= Alength) {
        waitPhase = 0;
        output = delayInput; // set input as output
        delayInput = 0;
        holdphase = 1;
    }
    // holdphase (make sure no trigger is send out)
    if (holdcount < holdtime && holdphase == 1 && output != 1) {
        output = 0;
        holdcount++;
    }
    if (holdcount == holdtime && trigger == 1 ) {
        output = 0;
    }
    if (holdcount == holdtime && trigger != 1) {
        holdphase = 0;
        releasephase = 1;
    }
    // releasephase (make sure no trigger is send out)
    if (releasephase == 1){
        output = 0;
    }
    
    return output;
}

//==============================================================================
// set length and point values
//==============================================================================
void Envelopes::setAttackLin8Steps(double length, double point1, double point2, double point3, double point4, double point5, double point6)
{
    double sampleValue = 1.0/samplerate;
    double sampleWorth = (sampleValue * (1.0 / (((1.0/7.0) * length) / 1000.0)));
    attackLinSteps[0] = sampleWorth * point1;
    attackLinSteps[1] = sampleWorth * (point2 - point1);
    attackLinSteps[2] = sampleWorth * (point3 - point2);
    attackLinSteps[3] = sampleWorth * (point4 - point3);
    attackLinSteps[4] = sampleWorth * (point5 - point4);
    attackLinSteps[5] = sampleWorth * (point6 - point5);
    attackLinSteps[6] = sampleWorth * (1.0 - point6);
    
    points[0] = point1;
    points[1] = point2;
    points[2] = point3;
    points[3] = point4;
    points[4] = point5;
    points[5] = point6;
    points[6] = 1.0;
}

void Envelopes::setReleaseLin8Steps(double length, double point1, double point2, double point3, double point4, double point5, double point6)
{
    double sampleValue = 1.0/samplerate;
    double sampleWorth = (sampleValue * (1.0 / (((1.0/7.0) * length) / 1000.0)));
    
    releaseLinSteps[0] = sampleWorth * (1.0 - point1);
    releaseLinSteps[1] = sampleWorth * (point1 - point2);
    releaseLinSteps[2] = sampleWorth * (point2 - point3);
    releaseLinSteps[3] = sampleWorth * (point3 - point4);
    releaseLinSteps[4] = sampleWorth * (point4 - point5);
    releaseLinSteps[5] = sampleWorth * (point5 - point6);
    releaseLinSteps[6] = sampleWorth * point6;

    releasePoints[0] = point1;
    releasePoints[1] = point2;
    releasePoints[2] = point3;
    releasePoints[3] = point4;
    releasePoints[4] = point5;
    releasePoints[5] = point6;
    releasePoints[6] = 0.0;
}

void Envelopes::setAttackLin(double attackMS)
{
    attackLin = ((1.0/samplerate) * (1.0 / (attackMS / 1000.0)));
}

void Envelopes::setAttackLinDelay(double attackMS, double delayMS)
{
    delayLength = delayMS;
    attackLin = ((1.0/samplerate) * (1.0 / (attackMS / 1000.0)));
}

void Envelopes::setReleaseLin(double releaseMS)
{
    releaseLin = (1.0/samplerate) * (1.0 / (releaseMS / 1000.0));
}

void Envelopes::setAttackExp(double attackMS)
{
    attackExp = pow((1.0/amplitudeStartValue), 1.0 / (samplerate * (attackMS / 1000.0)));
}

void Envelopes::setReleaseExp(double releaseMS)
{
    releaseExp = pow((amplitudeStartValue/1), 1.0/ (samplerate * (releaseMS / 1000.0)));
}

void Envelopes::setAttackLin4Steps(double length, double point1, double point2, double point1length, double point2length)
{
    double sampleValue = 1.0/samplerate;
    
    attackLin1 = (sampleValue * (1.0 / ((point1length * length) / 1000.0))) * point1;
    attackLin2 = (sampleValue * (1.0 / (((point2length - point1length) * length) / 1000.0))) * (point2-point1);
    attackLin3 = (sampleValue * (1.0 / (((1.0 - point2length) * length) / 1000.0))) * (1.0-point2);
    
    point1Y = point1;
    point2Y = point2;
}
