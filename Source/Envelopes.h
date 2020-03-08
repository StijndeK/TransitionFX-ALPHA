/*
  ==============================================================================

    Envelopes.h
    Created: 31 Oct 2018 1:40:53pm
    Author:  Stijn de Koning

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"


class Envelopes {
    
public:

    // arlin envelope states
    enum ArLinState {ATTACK, HOLD, RELEASE, STOP};
    // initial state
    ArLinState currentEnvState = STOP;
    
    double dLength = 0;
    double Alength = 0;
    
    // Envelopes
    double arGemiddelde(double input, int trigger);     // ar gemiddelde tussen exp en lin
    double arExp(double input, int trigger);    // ar exponentieel
    double arLin(double input, int trigger);    // ar Linear
    double arLin12Steps(double input, int trigger);  // ar Linear 8 stappen attack en release
    double dars(double input, int trigger);      // dasr Linear with choice for release or no release

    // Delay
    double delay(double input, int trigger);    // sends a delayed trigger
    double delayInput;
    
    // DARS
    bool darsDelay = false; // set delay on or off
    bool darsRelease = true; // choose between staying on sustain or goig release
    bool darsReverseRelease = false; // choose between normal and reverse release
    
    // Attack en Release voor linear en exponentieel
    double attackExp;
    double attackLin;
    double releaseExp;
    double releaseLin;
    double attackLin1, attackLin2, attackLin3; // 4 steps
    double attackLinSteps[7]; // 12 steps
    double releaseLinSteps[7]; // 12 steps
    
    // punten voor step envelope
    double point1Y;
    double point2Y;
    double points[7];
    double releasePoints[7];
    
    // gemiddelde tussen exp en lin
    double verhoudingGemiddelde;

    // phases in de envelope
    int attackphase,decayphase,sustainphase,holdphase,releasephase;
    int attack1phase, attack2phase, attack3phase;
    int attackPhase[7] = {0,0,0,0,0,0,0};
    int releasePhase[7] = {0,0,0,0,0,0,0};
    int waitPhase;
    
    // Functies voor het instellen van de waardes van de attack en releases
    void setAttackLin4Steps(double length, double point1, double point2, double point1length, double point2length);
    void setAttackLin8Steps(double length, double point1, double point2, double point3, double point4, double point5, double point6);
    void setReleaseLin8Steps(double length, double point1, double point2, double point3, double point4, double point5, double point6);

    void setAttack(double attackMS);
    void setRelease(double releaseMS);
    void setAttackLin(double attackMS);
    void setReleaseLin(double releaseMS);
    void setAttackLinDelay(double attackMS, double delayMS);
    void setAttackExp(double attackMS);
    void setReleaseExp(double releaseMS);
    
    // samplerate
    double samplerate = 44100.0;
    double setSampleRate (double rate)
    {
        if (rate == 0){
            return 44100.0;
        }
        else {
            return rate;
        }
    }
    
    // Length
    double attackLength;
    double releaseLength;
    double setLength(double attackMS)
    {
        return (attackMS / 1000) * samplerate; 
    }
    double setTotalLength(double attackMS, double releaseMS)
    {
        return ((attackMS / 1000) * samplerate) + ((releaseMS / 1000) * samplerate); 
    }
    double delayLength = 0;
    
    // verder
    double output;
    double amplitude = 0.001;
    double amplitudeStartValue = 0.001;     // define the startvalue for the exponential curve
    double amplitudeExp, amplitudeLin;
    int trigger;
    long holdtime=1;
    long holdcount;
    int currentsample = 0;
    
    // smoothing
    double oldOutput;
    double oldOutputsArray[1000];
    int currentOutput = 0;
    double newOutput;
    bool first50check = false;
    bool smooth = false;
    float smoothAmount = 1;
};
