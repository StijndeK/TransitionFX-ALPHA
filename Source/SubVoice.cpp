/*
  ==============================================================================

    SubVoice.cpp
    Created: 20 Nov 2018 11:09:30am
    Author:  Stijn de Koning

  ==============================================================================
*/

#include "SubVoice.h"


//==============================================================================
//                               Oscillators
//==============================================================================
void SubVoice::getOsc1Parameters(int selection, double pitch, double volume, double pitchoffset, int oscvoicecount, int oscnumber)
{
    // set parameters based on which osc is selected
    for (int i = 0; i < 4; i++){        // every osc
        if (oscnumber == i) {
            osc1Selection[i]    = selection;
            osc1Pitch[i]        = pitch;
            osc1Volume[i]       = volume;
            osc1PitchOffset[i]  = pitchoffset;
            osc1VoiceCount[i]   = oscvoicecount;
            break;
        }
    }
    
    // set value for how many oscs are turned on (to check how much the volume should be reduced)
    amountOfOscs = 0;
    for (int i = 0; i < 4; i++) {
        if (osc1Selection[i] != 0){
            amountOfOscs = amountOfOscs + 1;
        }
    }
}

// select osc and return its output
double SubVoice::setOscSelection (int oscSelection, int countvoice, double pitch, maxiOsc (&osc) [5])
{
    if (oscSelection == 0){
        return 0.f;
    }
    else if (oscSelection == 1){        // white noise
        return osc[countvoice].noise2();
    }
    else if (oscSelection == 2){        // brown noise
        return nOsc1.noiseBrown();
    }
    else if (oscSelection == 3){        // pink noise
        float out = pn1.GetNextValue();
        return ((out / 100) * 2) - 1;
    }
    else if (oscSelection == 4){        // saw
        return osc[countvoice].saw(pitch);
    }
    else if (oscSelection == 5){        // sine
        return osc[countvoice].sinewave(pitch);
    }
    else if (oscSelection == 6){        // square
        return osc[countvoice].square(pitch);
    }
    else if (oscSelection == 7){        // triangle
        return osc[countvoice].triangle(pitch);
    }
    else {
        std::cout << "oscselection out of bounds. Set to square." << std::endl;
        return osc[countvoice].square(pitch);
    }
}

// set the osc parameters
double SubVoice::setOsc1Parameters ()
{
    double oscSelectionOutput[4] = {0,0,0,0};
    
    // set OVDpitchoffset, check if env is set on OVDpitchoffset
    double calculatedPitchOffset;
    if (env2OVDOnOffToggle == 0) {
        calculatedPitchOffset = osc1PitchOffset[0]; // TODO: this shouldnt be an array
    }
    else {
        calculatedPitchOffset = setOVDPitch(osc1PitchOffset[0]);
    }
    
    // for every osc
    for (int i = 0; i < 4; i++){
        
        // check if osc is on
        if (osc1Selection[i] != 0 ){
            
            double oscSelection = 0;

            // set pitch
            double pitch = setOsc1Pitch(osc1Pitch[i], i);
            
            // for every osc voice
            for (int ii = 0; ii < osc1VoiceCount[i]; ii++) {
                
                // check which osc is on
                if (i == 0){
                    oscSelection = oscSelection + setOscSelection(osc1Selection[i], ii, pitch, subVoiceOsc1);
                }
                else if (i == 1) {
                    oscSelection = oscSelection + setOscSelection(osc1Selection[i], ii, pitch, subVoiceOsc2);
                }
                else if (i == 2) {
                    oscSelection = oscSelection + setOscSelection(osc1Selection[i], ii, pitch, subVoiceOsc3);
                }
                else if (i == 3){
                    oscSelection = oscSelection + setOscSelection(osc1Selection[i], ii, pitch, subVoiceOsc4);
                }
                
                // calculate new pitch with offset
                pitch = pitch * pow(2.0 , calculatedPitchOffset);
            }
            
            // set reduction volume and add volume
            oscSelectionOutput[i] = (oscSelection * pow(10, ((osc1VoiceCount[i] - 1) * (-3 - ((osc1VoiceCount[i] - 1) / 5))  / 20) ))  * osc1Volume[i];
        }
    }
    
    // add all oscs together and set reduction volume
    return (oscSelectionOutput[0] + oscSelectionOutput[1] + oscSelectionOutput[2] + oscSelectionOutput[3]) * pow(10, ((amountOfOscs - 1) * -3)  / 20);
}

// set logoritmic pitch (go through setdetunedpitch to check if there's a detune modulation)
float SubVoice::setOsc1Pitch (double detune, int oscnumb)
{
    double out;
    
    // check if pitch modulation is on
    if (lfoSelection[1] != 0 || env2DetuneOnOffToggle != 0 || env2DetuneOnOffToggleM != 0) {
        // check if master pitch modulation is on
        if (env2DetuneOnOffToggleM == 1) {
            out = masterFrequency * pow(2.0 , setDetunedMasterPitch(setDetunedPitch(detune, oscnumb), oscnumb));
        }
        else {
            out = masterFrequency * pow(2.0 , setDetunedPitch(detune, oscnumb));
        }
    }
    else {
        out = masterFrequency * pow(2.0 , detune);
    }
    
    return out;
}


//==============================================================================
//                               Modulation
//==============================================================================
//------------------------------- Detune --------------------------------------
void SubVoice::getEnv2ParametersMasterDetune(float attack, float release, float dopplertoggle, float onofftoggle, float delay, float forwardstoggle)
{
    env2DetuneOnOffToggleM = onofftoggle;
    env2DetuneLinkToggleM = dopplertoggle;
    env2DetuneAttackM = attack;
    env2DetuneReleaseM = release;
    env2DetuneDelayM = delay;
    env2DetuneForwardsToggleM = forwardstoggle;
}


void SubVoice::getEnv2DetuneParameters (float linktoggle, float onofftoggle, float attack, float release, float delay, float forwardstoggle, float sustaintoggle)
{
    env2DetuneOnOffToggle = onofftoggle;
    env2DetuneLinkToggle = linktoggle;
    env2DetuneAttack = attack;
    env2DetuneRelease = release;
    env2DetuneDelay = delay;
    env2DetuneForwardsToggle = forwardstoggle;
    env2DetuneSustain = sustaintoggle;
}

void SubVoice::getLfoDetuneParameters(float selectlfo, float freqlfo)
{
    lfoSelection[1] = selectlfo;
    lfoPitch[1] = freqlfo;
}

float SubVoice::setDetunedPitch(double input, int oscnumb)
{
    double attack, release, delay, out;
    
    // check if link toggle is on
    if (env2DetuneLinkToggle == 0){
        attack = env2DetuneAttack * masterAttack;
        release = env2DetuneRelease * masterRelease;
        delay = env2DetuneDelay * masterAttack;
    }
    else {
        attack = masterAttack;
        release = masterRelease;
        delay = 0;
    }
    
    // set envelope parameters
    env2Detune[oscnumb].setAttackLinDelay(attack - delay, delay);
    env2Detune[oscnumb].setReleaseLin(release);
    env2Detune[oscnumb].attackLength = masterAttack;
    env2Detune[oscnumb].releaseLength = masterRelease;
    env2Detune[oscnumb].darsRelease = (env2DetuneSustain == 1) ? false : true;
    
    // check modulations statuses and set output
    if (env2DetuneOnOffToggle == 1 && lfoSelection[1] != 0) { // both are on
        // check and set normal or reverse release
        if (env2DetuneForwardsToggle == 1) {
            out = env2Detune[oscnumb].dars((input * lfo[oscnumb].sinewave(lfoPitch[1])), masterTrigger);
        }
        else {
            out = (input - env2Detune[oscnumb].dars((input * lfo[oscnumb].sinewave(lfoPitch[1])), masterTrigger));
        }
    }
    else if (env2DetuneOnOffToggle == 1 && lfoSelection[1] == 0) { // only env
        // check and set normal or reverse release
        if (env2DetuneForwardsToggle == 1) {
            out = env2Detune[oscnumb].dars(input, masterTrigger);
        }
        else {
            out = input - env2Detune[oscnumb].dars(input, masterTrigger);;
        }
    }
    else if (env2DetuneOnOffToggle != 1 && lfoSelection[1] != 0) { // only lfo
        out = input * lfo[oscnumb].sinewave(lfoPitch[1]);
    }
    else {  // error check
        out = input;
    }
    
    return out;
}

float SubVoice::setDetunedMasterPitch(double input, int oscnumb)
{
    double attack, release, delay, out;
    
    // check if link toggle is on
    if (env2DetuneLinkToggleM == 0){
        attack = env2DetuneAttackM * masterAttack;
        release = env2DetuneReleaseM * masterRelease;
        delay = env2DetuneDelayM * masterAttack;
    }
    else {
        attack = masterAttack;
        release = masterRelease;
        delay = 0;
    }
    
    // set envelope parameters
    env2DetuneM[oscnumb].setAttackLinDelay(attack - delay, delay);
    env2DetuneM[oscnumb].setReleaseLin(release);
    env2DetuneM[oscnumb].attackLength = masterAttack;
    env2DetuneM[oscnumb].releaseLength = masterRelease;
    env2DetuneM[oscnumb].darsRelease = true;
    
    // check and set normal or reverse release
    if (env2DetuneForwardsToggleM == 1) {
        out = env2DetuneM[oscnumb].dars(input, masterTrigger);
    }
    else {
        out = input - env2DetuneM[oscnumb].dars(input, masterTrigger);;
    }

    return out;
}

//------------------------------- OVD --------------------------------------
void SubVoice::getEnv2OVDParameters(float linktoggle, float onofftoggle, float attack, float release, float delay, float forwardstoggle, float sustaintoggle)
{
    env2OVDOnOffToggle = onofftoggle;
    env2OVDLinkToggle = linktoggle;
    env2OVDAttack = attack;
    env2OVDRelease = release;
    env2OVDDelay = delay;
    envOVDForwardsToggle = forwardstoggle;
    env2OVDSustain = sustaintoggle;
}

float SubVoice::setOVDPitch(float OVDPitch)
{
    double attack, release, delay, out;
    
    // check if link toggle is on
    if (env2OVDLinkToggle == 0){
        attack = env2OVDAttack * masterAttack;
        release = env2OVDRelease * masterRelease;
        delay = env2OVDDelay * masterAttack;
    }
    else {
        attack = masterAttack;
        release = masterRelease;
        delay = 0;
    }
    
    // set envelope parameters
    env2OVD.setAttackLinDelay(attack - delay, delay);
    env2OVD.setReleaseLin(release);
    env2OVD.attackLength = masterAttack;
    env2OVD.releaseLength = masterRelease;
    env2OVD.darsRelease = (env2OVDSustain == 1) ? false : true;
    
    // check and set normal or reverse release
    if (envOVDForwardsToggle == 1) {
        out = env2OVD.dars(OVDPitch, masterTrigger);
    }
    else {
        out = OVDPitch - env2OVD.dars(OVDPitch, masterTrigger);
    }
    
    return out;
}

//------------------------------- Gain LFO --------------------------------------
void SubVoice::getLfoParameters(float selectlfo, float freqlfo)
{
    lfoSelection[0] = selectlfo;
    lfoPitch[0] = freqlfo;
}

// set gain lfo
double SubVoice::setLfoParameters(double input)
{
    return input * (1 + lfo[0].sinewave(lfoPitch[0]) / 2);
}


//==============================================================================
//                                  Master
//==============================================================================

// get the general/master attack and release
void SubVoice::getGeneralAR(float* attack, float* release)
{
    masterAttack = *attack;
    masterRelease = *release;
}

// get the trigger
void SubVoice::getGeneralTrigger(int trigger)
{
    masterTrigger = trigger;
}

// set the output
double SubVoice::setSubVoiceOutput ()
{
    double out;
    
    // check if lfo is on
    if (lfoSelection[0] == 0){
        out = setOsc1Parameters();
    }
    else {
        out = setLfoParameters(setOsc1Parameters());
    }
    
    return out;
}




