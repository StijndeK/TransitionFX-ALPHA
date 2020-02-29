/*
  ==============================================================================

    SubVoice.h
    Created: 20 Nov 2018 11:09:30am
    Author:  Stijn de Koning

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "maximilian.h"
#include "Envelopes.h"
#include "DSP.h"


class SubVoice {
    
public:
    
    // osc1
    void getOsc1Parameters (int selection, double pitch, double volume, double pitchoffset, int oscvoicecount, int oscnumber);
    double setOscSelection (int oscSelection, int countvoice, double pitch, maxiOsc (&osc) [5]);
    double setOsc1Parameters ();

    float setOsc1Pitch (double detune, int oscnumb);
    
    // detune
    void getEnv2DetuneParameters (float linktoggle, float onofftoggle, float attack, float release, float delay, float forwardstoggle, float sustaintoggle);
    float setDetunedPitch (double input, int oscnumb);
    
    void getEnv2ParametersMasterDetune (float attack, float release, float dopplertoggle, float onofftoggle, float delay, float forwardstoggle);
    float setDetunedMasterPitch (double input, int oscnumb);
    
    // OVD
    void getEnv2OVDParameters (float linktoggle, float onofftoggle, float attack, float release, float delay, float forwardstoggle, float sustaintoggle);
    float setOVDPitch (float OVDPitch);
    
    // lfo
    void getLfoParameters(float selectlfo, float freqlfo);
    double setLfoParameters(double input);
    
    void getLfoDetuneParameters (float selectlfo, float freqlfo);

    // frequency
    double masterFrequency;
    
    // general/master
    void getGeneralAR (float* attack, float* release);
    void getGeneralTrigger (int trigger);
    
    // output
    double setSubVoiceOutput ();
    
    
    // implement envelopes
    Envelopes env2Detune[4];    // one for every osc
    Envelopes env2DetuneM[4];
    Envelopes env2OVD;

private:

    // env2
    float env2DetuneOnOffToggle;
    float env2DetuneLinkToggle; 
    float env2DetuneAttack;
    float env2DetuneRelease;
    float env2DetuneDelay;
    float env2DetuneForwardsToggle;
    float env2DetuneSustain;
    
    float env2DetuneOnOffToggleM;
    float env2DetuneLinkToggleM;
    float env2DetuneAttackM;
    float env2DetuneReleaseM;
    float env2DetuneDelayM;
    float env2DetuneForwardsToggleM;
    
    float env2OVDOnOffToggle;
    float env2OVDLinkToggle;
    float env2OVDAttack;
    float env2OVDRelease;
    float env2OVDDelay;
    float envOVDForwardsToggle;
    float env2OVDSustain;
    
    // general/master
    double masterAttack;
    double masterRelease;
    int masterTrigger;
    
    // osc1
    int osc1Selection[4];
    double osc1Pitch[4];
    double osc1Volume[4];
    double osc1PitchOffset[4];
    int osc1VoiceCount[4];
    int amountOfOscs = 0;
    
    // lfo
    float lfoSelection[2];
    float lfoPitch[2];
    
    // implement classes
    maxiOsc subVoiceOsc1[5];    // 5 for the oscsubvoices
    maxiOsc subVoiceOsc2[5];
    maxiOsc subVoiceOsc3[5];
    maxiOsc subVoiceOsc4[5];
    
    maxiOsc lfo[4];             // one for every osc
    
    noiseOsc nOsc1;
    PinkNumber pn1;
};
