/*
  ==============================================================================

    SynthVoiceSource.cpp
    Created: 19 Nov 2018 1:19:31pm
    Author:  Stijn de Koning

  ==============================================================================
*/

class SynthVoice;
#include "ModulationEditor.h"


//==============================================================================
//                              Routing Systeem
//==============================================================================

// select which signal should be processed by giving and imput number (number of where in the chain)
double* SynthVoice::setSigStereo(int inputNumb)
{
    if (inputNumb == 1) {           // subvoices sound (just source sound)
        return setSubVoiceParametersStereo();
    }
    else if (inputNumb == 2){       // distorted sound
        return setDistortionStereo();
    }
    else if (inputNumb == 3){       // gain lfo-modulated sound
        return setLfo1ParametersStereo();
    }
    else {                          // check for out of bounds
        std::cout << "signal number out of bounds. Set to subVoiceParameters" << std::endl;
        return setSubVoiceParametersStereo();
    }
}


//==============================================================================
//                                  Hits
//==============================================================================

void SynthVoice::getHitsParameters (float* kickPitch, float* kickRelease, float* onofftoggle, float* gain, float* offset, float* distortiontoggle)
{
    mKickPitch = *kickPitch;
    mKickRelease = *kickRelease;
    mKickOnOff = *onofftoggle;
    mKickGain = *gain;
    hitTimingOffset = *offset;
    mKickDistortionOnOff = *distortiontoggle;
}

double SynthVoice::setHitsParameters ()
{
    // set parameters
    mKick.setPitch(mKickPitch);
    mKick.setRelease(mKickRelease);
    mKick.gain = mKickGain;
    mKick.trigger(envKick.delay(1., trigger));
    envKick.attackLength = env1Attack + (hitTimingOffset * env1Release);
        
        // check if distortion is on and return output
        if (mKickDistortionOnOff == 0){
            return mKick.play();
        }
        else {
            return maxiDist.atanDist(mKick.play(), 1000) / 2 * mKickGain;
        }
}

//==============================================================================
//                               Oscillators
//==============================================================================

void SynthVoice::getAllOsc1Parameters(float* (&selections)[4], float* (&detunes)[4], float* (&gains)[4])
{
    // set parameters based on which voice is selected
    for (int i = 0; i < constAmountOfSubvoices; i ++) {     // every synth voice
        if (currentSynthVoice == i+1) {     // check which voice is being edited
            for (int ii = 0; ii < 4; ii++) {     // every osc
                oscAllVolumeArrays[ii][i] = pow(10,  *gains[ii] / 20);
                oscAllSelectionArrays[ii][i] = *selections[ii];
                oscAllDetuneArrays[ii][i] = *detunes[ii];
            }
        }
    }
    
    // give parameters to subvoices
    for (int i = 0; i < amountOfSynthVoices; i ++) {    // every synth voice being used
        for (int ii = 0; ii < 4; ii++) {    // every osc
            subVoiceArray[i].getOsc1Parameters(oscAllSelectionArrays[ii][i], oscAllDetuneArrays[ii][i], oscAllVolumeArrays[ii][i], oscOscVoiceOffsetArray[i], oscAmountOfOscVoicesArray[i], ii);
        }
    }
}

void SynthVoice::getOscPitchOffsetVoices(float *oscvoices, float *oscvoicespitchoffset)
{
    // set parameters based on which voice is selected
    for (int i = 0; i < constAmountOfSubvoices; i++){
        if (currentSynthVoice == i+1){
            oscAmountOfOscVoicesArray[i] = *oscvoices;
            oscOscVoiceOffsetArray[i] = *oscvoicespitchoffset;
            break;
        }
    }
}


//==============================================================================
//                                   LFO 1
//==============================================================================

//------------------------------- general lfos -----------------------------------
void SynthVoice::getLfo1Parameters(float* selection, float* frequency)
{
    lfo1Freq = *frequency;
    lfo1Selection = *selection;
}

// Master gain LFO
double* SynthVoice::setLfo1ParametersStereo()
{
    // check which input should be used and set it
    int signalValue = (distortionOnOffToggle == 1 ? 2 : 1);
    double * cleanSig = setSigStereo(signalValue);
    
    // check for env on fq and set fq
    double lfoParamsFQ = (env2LFOOnOffButton == 1 ? setEnvLFORateParameters() : lfo1Freq);
    
    // check menu seletion and set outputs
    double* out = new double [2];
    if (lfo1Selection == 1) {
        out[0] = cleanSig[0] * lfoL.sinewave(lfoParamsFQ / 2.0);
        out[1] = cleanSig[1] * lfoR.sinewave(lfoParamsFQ / 2.0);
    }
    else if (lfo1Selection == 2) {
        out[0] = cleanSig[0] * lfoL.saw(lfoParamsFQ / 2.0);
        out[1] = cleanSig[1] * lfoR.saw(lfoParamsFQ / 2.0);
    }
    else if (lfo1Selection == 3) {
        out[0] = cleanSig[0] * lfoL.square(lfoParamsFQ / 2.0);
        out[1] = cleanSig[1] * lfoR.square(lfoParamsFQ / 2.0);
    }
    else if (lfo1Selection == 4) {
        out[0] = cleanSig[0] * lfoL.triangle(lfoParamsFQ / 2.0);
        out[1] = cleanSig[1] * lfoR.triangle(lfoParamsFQ / 2.0);
    }
    else {
        out[0] = cleanSig[0];
        out[1] = cleanSig[1];
    }
    
    return out;
}

//------------------------------- subvoice lfos -----------------------------------
void SynthVoice::getLfo1VoicesParameters(float selection, float frequency)
{
    // set parameters based on which voice is selected
    for (int i = 0; i < constAmountOfSubvoices; i++){
        if (currentSynthVoice == i + 1){
            lfoVoicesMenusArray[i] = selection;
            lfoVoicesFreqSlidersArray[i] = frequency;
            break;
        }
    }
    
    // give parameters to subvoices
    for (int i = 0; i < constAmountOfSubvoices; i++){
        subVoiceArray[i].getLfoParameters(lfoVoicesMenusArray[i], lfoVoicesFreqSlidersArray[i]);
    }
}

void SynthVoice::getLfo1VoicesParametersDetune (float selection, float frequency)
{
    // set parameters based on which voice is selected
    for (int i = 0; i < constAmountOfSubvoices; i++){
        if (currentSynthVoice == i + 1){
            lfoVoicesMenusDetuneArray[i] = selection;
            lfoVoicesFreqSlidersDetuneArray[i] = frequency;
            break;
        }
    }
    
    // give parameters to subvoices
    for (int i = 0; i < amountOfSynthVoices; i++){
        subVoiceArray[i].getLfoDetuneParameters(lfoVoicesMenusDetuneArray[i], lfoVoicesFreqSlidersDetuneArray[i]);
    }
}

//==============================================================================
//                                  Effects
//==============================================================================

//------------------------------- Distortion1 -----------------------------------
void SynthVoice::getDistortionParameters (float* toggle, float* drive, float* range, float* blendDist, float* gain, float* selection)
{
    distortionOnOffToggle = *toggle;
    distortionDrive = *drive;
    distortionRange = *range;
    distortionBlend = *blendDist;
    distortionGain = *gain;
    distortionMenuChoice = *selection;
}

double* SynthVoice::setDistortionStereo ()
{
    float volume = distortionGain;
    float drive = distortionDrive;
    float range = distortionRange;
    float blend = distortionBlend;
    
    //save clean signal for blend
    double * cleanSig = setSigStereo(1);

    // set drive and range values
    double driveandrange[2];
    driveandrange[0] = cleanSig[0] * (drive * range);
    driveandrange[1] = cleanSig[1] * (drive * range);

    // check which distortion should play
    if (distortionMenuChoice == 0){
        // add distortion and set outvalues
        double* out = new double[2];
        out[0] = ( ( ((2.f / float_Pi) * atan(driveandrange[0]))  * blend) + (cleanSig[0] * (1.f - blend))) * volume;
        out[1] = ( ( ((2.f / float_Pi) * atan(driveandrange[1]))  * blend) + (cleanSig[1] * (1.f - blend))) * volume;
        return out;
    }
    else {
        // return clean signal because there's no second distortion yet
        return cleanSig;
    }
}

//------------------------------- Chorus -----------------------------------------
// template to create chorus
void SynthVoice::getChorus (float* toggle)
{
    chorusOnOffToggle = *toggle;
}

double SynthVoice::setChorus ()
{
    return 0;
}


//==============================================================================
//                              Envelope 2
//==============================================================================

//------------------------------- Detune Voices --------------------------------------
void SynthVoice::getEnv2ParametersDetuneV (float* attack, float* release, float* dopplertoggle, float* onofftoggle, float* delay, float* forwardstoggle, float* sustaintoggle)
{
    // set parameters based on which voice is selected
    for (int i = 0; i < constAmountOfSubvoices; i++){
        if (currentSynthVoice == i + 1){
            env2DetuneAttacksArray[i] = *attack;
            env2DetuneReleasesArray[i] = *release;
            env2DetuneDopplerButtonsArray[i] = *dopplertoggle;
            env2DetuneOnOffButtonsArray[i] = *onofftoggle;
            env2DetuneDelaysArray[i] = *delay;
            env2DetuneForwardsButtonsArray[i] = *forwardstoggle;
            env2DetuneSustainsArray[i] = *sustaintoggle;
            break;
        }
    }
    
    // give parameters to subvoices
    for (int i = 0; i < amountOfSynthVoices; i++){
        subVoiceArray[i].getEnv2DetuneParameters(env2DetuneDopplerButtonsArray[i], env2DetuneOnOffButtonsArray[i], env2DetuneAttacksArray[i], env2DetuneReleasesArray[i], env2DetuneDelaysArray[i], env2DetuneForwardsButtonsArray[i], env2DetuneSustainsArray[i]);
    }
}


//------------------------------- OVD (osc voice detunes) --------------------------------------
void SynthVoice::getEnv2ParametersOVDV(float *attack, float *release, float *dopplertoggle, float *onofftoggle, float* delay, float* forwardstoggle, float* sustaintoggle)
{
    // set parameters based on which voice is selected
    for (int i = 0; i < constAmountOfSubvoices; i++){
        if (currentSynthVoice == i + 1){
            env2OVDAttacksArray[i] = *attack;
            env2OVDReleasesArray[i] = *release;
            env2OVDDopplerButtonsArray[i] = *dopplertoggle;
            env2OVDOnOffButtonsArray[i] = *onofftoggle;
            env2OVDDelaysArray[i] = *delay;
            env2OVDForwardsButtonsArray[i] = *forwardstoggle;
            env2OVDSustainsArray[i] = *sustaintoggle;
            break;
        }
    }
    
    for (int i = 0; i < amountOfSynthVoices; i++){
        subVoiceArray[i].getEnv2OVDParameters(env2OVDDopplerButtonsArray[i], env2OVDOnOffButtonsArray[i], env2OVDAttacksArray[i], env2OVDReleasesArray[i], env2OVDDelaysArray[i], env2OVDForwardsButtonsArray[i], env2OVDSustainsArray[i]);
    }
}

//------------------------------- Voices Pan --------------------------------------
void SynthVoice::getEnv2ParametersPanV(float *attack, float *release, float *dopplertoggle, float *onofftoggle, float *delay, float *forwardstoggle, float* sustaintoggle)
{
    // set parameters based on which voice is selected
    for (int i = 0; i < constAmountOfSubvoices; i++){
        if (currentSynthVoice == i + 1){
            env2PanAttacksArray[i] = *attack;
            env2PanReleasesArray[i] = *release;
            env2PanDopplerButtonsArray[i] = *dopplertoggle;
            env2PanOnOffButtonsArray[i] = *onofftoggle;
            env2PanDelaysArray[i] = *delay;
            env2PanForwardsButtonsArray[i] = *forwardstoggle;
            env2PanSustainsArray[i] = *sustaintoggle;
            break;
        }
    }
}

//------------------------------- Voices Gain --------------------------------------
void SynthVoice::getEnv2ParametersGainV(float *attack, float *release, float *dopplertoggle, float *onofftoggle, float *delay, float *forwardstoggle)
{
    // set parameters based on which voice is selected
    for (int i = 0; i < constAmountOfSubvoices; i++){
        if (currentSynthVoice == i + 1){
            env2GainAttacksArray[i] = *attack;
            env2GainReleasesArray[i] = *release;
            env2GainDopplerButtonsArray[i] = *dopplertoggle;
            env2GainOnOffButtonsArray[i] = *onofftoggle;
            env2GainDelaysArray[i] = *delay;
            env2GainForwardsButtonsArray[i] = *forwardstoggle;
            break;
        }
    }
}

//------------------------------- Detune Master --------------------------------------
void SynthVoice::getEnv2ParametersDetune(float *attack, float *release, float *dopplertoggle, float *onofftoggle, float *delay, float *forwardstoggle, float *sustaintoggle)
{
    env2DetuneOnOffButton = *onofftoggle;
    env2DetuneDopplerButton = *dopplertoggle;
    env2DetuneAttack = *attack;
    env2DetuneRelease = *release;
    env2DetuneDelay = *delay;
    env2DetuneForwards = *forwardstoggle;
    env2DetuneSustainButton = *sustaintoggle;
    
    for (int i = 0; i < amountOfSynthVoices; i++){
        subVoiceArray[i].getEnv2ParametersMasterDetune(env2DetuneAttack, env2DetuneRelease, env2DetuneDopplerButton, env2DetuneOnOffButton, env2DetuneDelay, env2DetuneForwards);
    }
}

//------------------------------- gain LFO --------------------------------------
void SynthVoice::getEnv2ParametersLFO (float* attack, float* release, float* dopplertoggle, float* onofftoggle, float* delay, float* forwardstoggle, float (&range)[2], float* sustaintoggle)
{
    env2LFOOnOffButton = *onofftoggle;
    env2LFODopplerButton = *dopplertoggle;
    env2LFOAttack = *attack;
    env2LFORelease = *release;
    env2LFODelay = *delay;
    env2LFOForwards = *forwardstoggle;
    env2LFORange[0] = range[0];
    env2LFORange[1] = range[1];
    env2LFOSustainButton = *sustaintoggle;
}

double SynthVoice::setEnvLFORateParameters ()
{
    double attack, release, delay, out, rangedinput;
    
    rangedinput = env2LFORange[1] - env2LFORange[0];
    
    if (env2LFODopplerButton == 0){
        attack = env2LFOAttack * env1Attack;
        release = env2LFOAttack * env1Release;
        delay = env2LFODelay * env1Attack;
    }
    else {
        attack = env1Attack;
        release = env1Release;
        delay = 0;
    }
    env2s[0].setAttackLinDelay(attack - delay, delay);
    env2s[0].setReleaseLin(release);
    
    env2s[0].darsRelease = (env2LFOSustainButton == 1) ? false : true;
    
    
    // check and set normal or reverse release
    if (env2LFOForwards == 1) {
        out = env2LFORange[0] + env2s[0].dars(rangedinput, trigger);
//        out = env2s[0].dars(lfo1Freq, trigger);
    }
    else {
        out = lfo1Freq - env2s[0].dars(lfo1Freq, trigger);
    }
    
    return out;
}

//------------------------------- Default --------------------------------------
double SynthVoice::setEnv23Paramers (float envinput, int envnumber, bool reverserelease, float envattack, float envrelease, float envdopplertoggle, float envonofftoggle, float envdelay, float envforwardstoggle, float sustaintoggle)
{
    double attack, release, delay, out;
    
    if (envdopplertoggle == 0) {
        attack = envattack * env1Attack;
        release = envrelease * env1Release;
        delay = envdelay * env1Attack;
    }
    else {
        attack = env1Attack;
        release = env1Release;
        delay = 0;
    }
    
    env2s[envnumber].setAttackLinDelay(attack - delay, delay);
    env2s[envnumber].setReleaseLin(release);
    env2s[envnumber].darsRelease = (sustaintoggle == 1) ? false : true; // set release on / off
    env2s[envnumber].darsReverseRelease = reverserelease; // set release to keep going or to go back
    
    // check and set normal or reverse release
    if (envforwardstoggle == 1) {
        out = env2s[envnumber].dars(envinput, trigger);
    }
    else {
        // figure out what needs to be here FORMULE
        out = 1 - env2s[envnumber].dars(envinput, trigger);
    }
    
    return out;
}

//------------------------------- Pan --------------------------------------
void SynthVoice::getEnv2ParametersPan (float* attack, float* release, float* dopplertoggle, float* onofftoggle, float* delay, float* forwardstoggle, float* sustaintoggle)
{
    env2PanOnOffButton = *onofftoggle;
    env2PanDopplerButton = *dopplertoggle;
    env2PanAttack = *attack;
    env2PanRelease = *release;
    env2PanDelay = *delay;
    env2PanForwards = *forwardstoggle;
    env2PanSustainButton = *sustaintoggle;
}

double SynthVoice::setEnvPanParameters()
{
    double attack, release, delay, out;
    
    if (env2PanDopplerButton == 0){
        attack = env2PanAttack * env1Attack;
        release = env2PanRelease * env1Release;
        delay = env2PanDelay * env1Attack;
    }
    else {
        attack = env1Attack;
        release = env1Release;
        delay = 0;
    }
    
    env2s[1].setAttackLinDelay(attack - delay, delay);
    env2s[1].setReleaseLin(release);
    env2s[1].darsRelease = true; // set release on / off
//    env2s[1].darsReverseRelease = true; // set release to keep going or to go back
    
    // check and set normal or reverse release
    if (env2PanForwards == 1) {
        out = env2s[1].dars(masterPan, trigger);
    }
    else {
        out = 1 - env2s[1].dars(masterPan, trigger);
    }
    
    return out;
}

//==============================================================================
//                               Envelope 1
//==============================================================================

//------------------------------- Getters --------------------------------------
void SynthVoice::getEnv1Parameters (float* selection, float* attack, float* release,  float* smooth, float* smoothamount)
{
    env1Selection = *selection;
    env1Attack = *attack;
    env1Release = *release;
    
    // TODO find better place to do this
    // set parameters for ALL envelopes
    for (int i = 0; i < amountOfEnvelopesLoop; i++) {
        masterGainEnv[i].attackLength = env1Attack;
        masterGainEnv[i].releaseLength = env1Release;
        masterGainEnv[i].smooth = *smooth;
        masterGainEnv[i].smoothAmount = *smoothamount;
        env2s[i].attackLength = env1Attack;
        env2s[i].releaseLength = env1Release;
    }
    
    // give parameters to subvoices
    for (int i = 0; i < constAmountOfSubvoices; i++) {
        subVoiceArray[i].getGeneralAR(&env1Attack, &env1Release);
    }
}

// ar steps for 15 step envelope
void SynthVoice::getEnv1AR8StepsParameters(float* (&attackPoints)[6], float* (&releasePoints)[6])
{
    for (int i = 0; i < 6; i++) {   // every step
        env1Points[i] = *attackPoints[i];
        env1ReleasePoints[i] = *releasePoints[i];
    }
}

//------------------------------- Setters --------------------------------------

double* SynthVoice::setEnv1ParametersStereo()
{
    masterGainEnv[0].setAttackLin(env1Attack);
    masterGainEnv[0].setReleaseLin(env1Release);
    masterGainEnv[1].setAttackLin(env1Attack);
    masterGainEnv[1].setReleaseLin(env1Release);

    // check which input should be used and set it
    int signalValue;
    if (lfo1Selection == 0 && distortionOnOffToggle == 0){
        signalValue = 1;
    }
    else if (lfo1Selection == 0 && distortionOnOffToggle == 1) {
        signalValue = 2;
    }
    else {
        signalValue = 3;
    }

    // give values to envelopes
    masterGainEnv[0].setReleaseLin8Steps(env1Release, env1ReleasePoints[0], env1ReleasePoints[1], env1ReleasePoints[2], env1ReleasePoints[3], env1ReleasePoints[4], env1ReleasePoints[5]);
    masterGainEnv[0].setAttackLin8Steps(env1Attack, env1Points[0], env1Points[1], env1Points[2], env1Points[3], env1Points[4], env1Points[5]);
    
    masterGainEnv[1].setReleaseLin8Steps(env1Release, env1ReleasePoints[0], env1ReleasePoints[1], env1ReleasePoints[2], env1ReleasePoints[3], env1ReleasePoints[4], env1ReleasePoints[5]);
    masterGainEnv[1].setAttackLin8Steps(env1Attack, env1Points[0], env1Points[1], env1Points[2], env1Points[3], env1Points[4], env1Points[5]);

    // get input and set outvalues
    double * in = setSigStereo(signalValue);
    double outvalueL = masterGainEnv[0].arLin12Steps(in[0], trigger);
    double outvalueR = masterGainEnv[1].arLin12Steps(in[1], trigger);

    double* out = new double[2];
    out[0] = outvalueL;
    out[1] = outvalueR;

    return out;
}


//==============================================================================
//                                  Master
//==============================================================================

void SynthVoice::getMasterParameters2(float *gain, float *panning, float *(&voicegains)[3], float *(&voicemutetoggles)[3], float *(&voicesolotoggles)[3], float *(&voicepans)[3])
{
    // set values
    masterGain = pow(10, *gain / 20);   // gain in db
    masterPan = (*panning + 1) / 2; // pan between 0 and 1
    for (int i = 0; i < constAmountOfSubvoices; i++) { // for every synth subvoice
        vGain[i] = pow(10, *voicegains[i] / 20);
        vMutes[i] = *voicemutetoggles[i];
        vSolos[i] = *voicesolotoggles[i];
        vPans[i] = (*voicepans[i] + 1) / 2;
    }
}

void SynthVoice::getSynthMasterParameters(float* voices, float* currentVoice)
{
    amountOfSynthVoices = *voices;
    currentSynthVoice = *currentVoice;
}

double* SynthVoice::setSubVoiceParametersStereo  ()
{
    double masterOutputL = 0;
    double masterOutputR = 0;
    
    double outSV[3]; // outsubvoice
    double outSVLeft[3]; // outsubvoice left
    double outSVRight[3]; // outsubvoice right
    
    double pannings[3]; // panning
    double gains[3]; // gain

    // get all the outputs
    for (int i = 0; i < constAmountOfSubvoices; i++) {
        
        // check pan envelope is on
        if (env2PanOnOffButtonsArray[i] == 1) {
            pannings[i] = setEnv23Paramers(vPans[i], i + 2, true, env2PanAttacksArray[i], env2PanReleasesArray[i], env2PanDopplerButtonsArray[i], env2PanOnOffButtonsArray[i], env2PanDelaysArray[i], env2PanForwardsButtonsArray[i], env2PanSustainsArray[i] );
        }
        else {
            pannings[i] = vPans[i];
        }
        
        // check gain envelope is on
        if (env2GainOnOffButtonsArray[i] == 1) {
            gains[i] = setEnv23Paramers(vGain[i], i + 5, false, env2GainAttacksArray[i], env2GainReleasesArray[i], env2GainDopplerButtonsArray[i], env2GainOnOffButtonsArray[i], env2GainDelaysArray[i], env2GainForwardsButtonsArray[i], 0);
        }
        else {
            gains[i] = vGain[i];
        }
        
        // set outputs
        outSV[i] = (subVoiceArray[i].setSubVoiceOutput() * gains[i]);
        outSVLeft[i] = (outSV[i] * (1 - pannings[i]));
        outSVRight[i] = (outSV[i] * pannings[i]);
    }
    
    // check for solos
    bool solo = false;
    for (int i = 0; i < constAmountOfSubvoices; i++) {
        if (vSolos[i] == 1){
            masterOutputL = masterOutputL + outSVLeft[i];
            masterOutputR = masterOutputR + outSVRight[i];
            solo = true;
        }
    }
    
    // check for mutes
    if (solo != true) {
        for (int i = 0; i < constAmountOfSubvoices; i++) {
            if (amountOfSynthVoices > i && vMutes[i] != 1){
                masterOutputL = masterOutputL + outSVLeft[i];
                masterOutputR = masterOutputR + outSVRight[i];
            }
        }
    }
    
    // set output and reduce volume
    double* mOutputStereo = new double[2];
    mOutputStereo[0] = masterOutputL * pow(10, ((amountOfSynthVoices - 1) * -3)  / 20);
    mOutputStereo[1] = masterOutputR * pow(10, ((amountOfSynthVoices - 1) * -3)  / 20);
    
    // return output
    return mOutputStereo;
}


//==============================================================================
//                              Synth Functions
//==============================================================================

// Must return true if this voice object is capable of playing the given sound
bool SynthVoice::canPlaySound (SynthesiserSound* sound)
{
    return dynamic_cast<SynthSound*>(sound) != nullptr;
}

// link to processor
void SynthVoice::setupProcessor (TransitionFxAudioProcessor* p)
{
    processor = p;
}

// Called to start a new note.
void SynthVoice::startNote (int midiNoteNumber, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition)
{
    amplitude = velocity;
    trigger = 1;
    
    // TRIGGERS
    for (int i = 0; i < constAmountOfSubvoices; i++) {
        subVoiceArray[i].getGeneralTrigger(1);
        subVoiceArray[i].masterFrequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    }
    
    for (int i = 0; i < amountOfEnvelopesLoop; i++) {
        masterGainEnv[i].trigger = 1;
        masterGainEnv[i].setSampleRate(usedSamplerate);
        env2s[i].trigger = 1;
        env2s[i].setSampleRate(usedSamplerate);
    }
    // set samplerate for subvoices
    for (int i = 0; i < 3; i ++) {
        for (int ii = 0; ii < 4; ii++) {
            subVoiceArray[i].env2Detune[ii].setSampleRate(usedSamplerate);
        }
        subVoiceArray[i].env2OVD.setSampleRate(usedSamplerate);
    }

    envKick.trigger = 1;

    // get frequency from midiNoteNumber
    frequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
}

// Called to stop note
void SynthVoice::stopNote (float velocity, bool allowTailOff)
{
    allowTailOff = true;
    trigger = 0;
    
    // TRIGGERS
    // give parameters to subvoice
    for (int i = 0; i < constAmountOfSubvoices; i++){
        subVoiceArray[i].getGeneralTrigger(0);
    }
    for (int i = 0; i < amountOfEnvelopesLoop; i++) {
        masterGainEnv[i].trigger = 0;
        env2s[i].trigger = 0;
    }
    envKick.trigger = 0;
    
    // clear note after tailoff
    if (amplitude == 0) {
        clearCurrentNote();
    }
}

// Called to let the voice know that the pitch wheel has been moved.
void SynthVoice::pitchWheelMoved(int newPitchWheelValue)
{
}

// Called to let the voice know that a midi controller has been moved
void SynthVoice::controllerMoved (int controllerNumber, int newControlerValue)
{
}

// set samplerate
void SynthVoice::setSamplerate (float srate)
{
    usedSamplerate = srate;
}

// set the processors trigger value
void SynthVoice::setTrigger ()
{
    // check if trigger has changed
    if (oldTrigger != trigger) {
        int value;
        oldTrigger = trigger;
        if (trigger == 1){
            value = 1;
        }
        else {
            value = 0;
        }
        processor->setEnvTrigger(value);
    }
}


//==============================================================================
//                          Processblock, audiocallback
//==============================================================================

void SynthVoice::renderNextBlock(AudioBuffer<float> &outputBuffer, int startSample, int numSamples)
{
    // set trigger for processor
    setTrigger();
    
    // per sample
    for (int sample = 0; sample < numSamples; ++sample) {
        
        // set output tonal
        double * arr = setEnv1ParametersStereo();
        
        // set output hits and check if they should be played
        double hits;
        if (mKickOnOff == 1){
            hits = setHitsParameters() * 0.5;
        }
        else {
            hits = 0.0;
        }
        
        // set output and add volume
        double theSoundL = (hits + arr[0]) * masterGain;
        double theSoundR = (hits + arr[1]) * masterGain;
        
        
        if (theSoundL < -10 || theSoundL > 10){
            std::cout << "SoundToLoudCrash" << std::endl;
            theSoundL = 0;
            theSoundR = 0;
        }

        // check for env on pan and set pan
        double pan;
        if (env2PanOnOffButton == 1) {
            pan = setEnv23Paramers(masterPan, 1, true, env2PanAttack, env2PanRelease, env2PanDopplerButton, env2PanOnOffButton, env2PanDelay, env2PanForwards, env2PanSustainButton);
        }
        else {
            pan = masterPan;
        }
        
        // add samples
        outputBuffer.addSample(0, startSample, theSoundL * (1.0-pan));
        outputBuffer.addSample(1, startSample, theSoundR * pan);

        // next sample
        ++startSample;
    }
}



