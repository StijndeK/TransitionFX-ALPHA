
#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "SynthSound.h"
#include "maximilian.h"
#include "Envelopes.h"

#include "SubVoice.h"

class TransitionFxAudioProcessor;

class SynthVoice : public SynthesiserVoice
{
public:

    // Routing Systeem
    double* setSigStereo (int inputNumb);
    
    // Hits
    void getHitsParameters (float* kickPitch, float* kickRelease, float* onofftoggle, float* gain, float* offset, float* distortiontoggle);
    double setHitsParameters ();

    // Osc1
    void getOscPitchOffsetVoices (float* oscvoices, float* oscvoicespitchoffset);
    void getAllOsc1Parameters (float* (&selections)[4], float* (&detunes)[4], float* (&gains)[4]);
    
    // Lfo1
    void getLfo1Parameters (float* selection, float* frequency);
    double* setLfo1ParametersStereo ();
    
    void getLfo1VoicesParameters (float selection, float frequency);
    void getLfo1VoicesParametersDetune (float selection, float frequency);
    
    // effects
    void getDistortionParameters (float* toggle, float* drive, float* range, float* blendDist, float* gain, float* selection);
    double* setDistortionStereo ();

    void getChorus (float* toggle);
    double setChorus ();
    
    // env2
    void getEnv2ParametersDetuneV (float* attack, float* release, float* dopplertoggle, float* onofftoggle, float* delay, float* forwardstoggle, float* sustaintoggle);
    void getEnv2ParametersOVDV (float* attack, float* release, float* dopplertoggle, float* onofftoggle, float* delay, float* forwardstoggle, float* sustaintoggle);
    void getEnv2ParametersPanV (float* attack, float* release, float* dopplertoggle, float* onofftoggle, float* delay, float* forwardstoggle, float* sustaintoggle);
    void getEnv2ParametersGainV (float* attack, float* release, float* dopplertoggle, float* onofftoggle, float* delay, float* forwardstoggle);
    
    void getEnv2ParametersDetune (float* attack, float* release, float* dopplertoggle, float* onofftoggle, float* delay, float* forwardstoggle, float* sustaintoggle);
    
    void getEnv2ParametersLFO (float* attack, float* release, float* dopplertoggle, float* onofftoggle, float* delay, float* forwardstoggle, float (&range)[2], float* sustaintoggle);
    double setEnvLFORateParameters ();
    
    double setEnv23Paramers (float envinput, int envnumber, bool reverserelease, float envattack, float envrelease, float envdopplertoggle, float envonofftoggle, float envdelay, float envforwardstoggle, float sustaintoggle);
    
    void getEnv2ParametersPan (float* attack, float* release, float* dopplertoggle, float* onofftoggle, float* delay, float* forwardstoggle, float* sustaintoggle);
    double setEnvPanParameters();

    // env1
    void getEnv1Parameters (float* selection, float* smooth, float* attack8th, float* release8th, float* smoothamount);
    void getEnv1AR8StepsParameters (float* (&attackPoints) [6], float* (&releasePoints) [6]);
    
    double* setEnv1ParametersStereo ();

    // master
    void getMasterParameters2 (float* gain, float* panning, float* (&voicegains) [3], float* (&voicemutetoggles) [3], float* (&voicesolotoggles) [3], float* (&voicepans) [3]);
    
    // synth master
    void getSynthMasterParameters(float* voices, float* currentVoice);
    
    // set the output from subvoices
    double* setSubVoiceParametersStereo () ;
    
    // synth functions
    bool canPlaySound (SynthesiserSound* sound);
    void setupProcessor (TransitionFxAudioProcessor* p);
    void startNote (int midiNoteNumber, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition);
    void stopNote (float velocity, bool allowTailOff);
    void pitchWheelMoved (int newPitchWheelValue);
    void controllerMoved (int controllerNumber, int newControlerValue);
    
    void setSamplerate (float srate);
    void setTrigger ();
    float trigger;

    // processblock, audiocallback
    void renderNextBlock (AudioBuffer<float> &outputBuffer, int startSample, int numSamples);
    
private:

    // processor
    TransitionFxAudioProcessor *processor;
    
    // subvoices amount
    const int constAmountOfSubvoices = 3;
    
    // master
    double amplitude;
    double frequency;
    double masterGain;
    double masterPan;
    double previousGain;
    float vGain[3];
    float vMutes[3];
    float vSolos[3];
    float vPans[3];
    
    // master2
    int amountOfSynthVoices;
    int currentSynthVoice;
    
    // osc
    float oscAmountOfOscVoicesArray[3];
    float oscOscVoiceOffsetArray[3];
    float oscAllVolumeArrays[4][3]; // osc, voice
    float oscAllDetuneArrays[4][3];
    float oscAllSelectionArrays[4][3];
    
    // env 1
    int env1Selection;
    float env1Attack = 0.0;
    float env1Release;

    float env1Points[6];
    float env1ReleasePoints[6];

    float env1AmpStartValue;
    float env1VerhoudingGemid;
    
    // lfo 1
    int lfo1Selection;
    float lfo1Freq;
    
    float lfoVoicesMenusArray[3];
    float lfoVoicesFreqSlidersArray[3];
    float lfoVoicesMenusDetuneArray[3];
    float lfoVoicesFreqSlidersDetuneArray[3];

    // env 2
    float env2LFOAttack, env2LFORelease, env2LFODopplerButton, env2LFOOnOffButton, env2LFODelay, env2LFOForwards, env2LFORange[2], env2LFOSustainButton;
    float env2PanAttack, env2PanRelease, env2PanDopplerButton, env2PanOnOffButton, env2PanDelay, env2PanForwards, env2PanSustainButton;
    float env2DetuneAttack, env2DetuneRelease, env2DetuneDopplerButton, env2DetuneOnOffButton, env2DetuneDelay, env2DetuneForwards, env2DetuneSustainButton;
    int checkForwardsOn = 0;
    float oldForwards = 0;
    
    float env2DetuneAttacksArray[3];
    float env2DetuneReleasesArray[3];
    float env2DetuneDopplerButtonsArray[3];
    float env2DetuneOnOffButtonsArray[3];
    float env2DetuneDelaysArray[3];
    float env2DetuneForwardsButtonsArray[3];
    float env2DetuneRangesArrays[2][3];
    float env2DetuneSustainsArray[3];
    
    float env2OVDAttacksArray[3];
    float env2OVDReleasesArray[3];
    float env2OVDDopplerButtonsArray[3];
    float env2OVDOnOffButtonsArray[3];
    float env2OVDDelaysArray[3];
    float env2OVDForwardsButtonsArray[3];
    float env2OVDSustainsArray[3];
    
    float env2PanAttacksArray[3];
    float env2PanReleasesArray[3];
    float env2PanDopplerButtonsArray[3];
    float env2PanOnOffButtonsArray[3];
    float env2PanDelaysArray[3];
    float env2PanForwardsButtonsArray[3];
    float env2PanSustainsArray[3];
    
    float env2GainAttacksArray[3];
    float env2GainReleasesArray[3];
    float env2GainDopplerButtonsArray[3];
    float env2GainOnOffButtonsArray[3];
    float env2GainDelaysArray[3];
    float env2GainForwardsButtonsArray[3];
    
    // distortion
    float distortionOnOffToggle;
    float distortionDrive;
    float distortionRange;
    float distortionBlend;
    float distortionGain;
    maxiDistortion maxiDist;
    float distortionMenuChoice;
    float mKickDistortionOnOff;
    
    // chorus / flanger
    maxiChorus chorus;
    float chorusOnOffToggle;
    maxiFlanger flanger;
    
    // synth
    float usedSamplerate;
    float usedFramerate;
    AudioPlayHead::FrameRateType usedFramerateType;
    int oldTrigger = 0;

    // create LFO
    maxiOsc lfoL;
    maxiOsc lfoR;

    // create envelopes
    Envelopes env2s[8];
    Envelopes masterGainEnv[2];
    const int amountOfEnvelopesLoop = 8;
    
    // kick
    maxiKick mKick;
    Envelopes envKick;
    float mKickPitch;
    float mKickRelease;
    float mKickTrigger;
    float mKickOnOff;
    float mKickGain;
    double hitTimingOffset;

    // get subvoices
    SubVoice subVoiceArray[3];
        
};
