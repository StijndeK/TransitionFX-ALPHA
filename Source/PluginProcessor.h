
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "maximilian.h"
#include "SynthSound.h"
#include "SynthVoice.h"
#include "Envelopes.h"
#include "SubVoice.h"
#include <algorithm>


class TransitionFxAudioProcessor  : public AudioProcessor
{
public:
    
    int oldTriggerButtonValue = 0;
    int manualTrigger = 0;
    
    //==============================================================================
    // sample playing
    AudioSourceChannelInfo ASCI;
    
    std::unique_ptr <AudioFormatReaderSource> PplaySource;
    std::unique_ptr<AudioFormatReaderSource> readerSource;
    AudioTransportSource transport;
    
    enum TransportState
    {
        Stopped,
        Starting,
        Stopping
    };
    
    TransportState transState;
    
    void transportStateChanged(TransportState state);
    
    AudioFormatManager formatManager;
    
    void getEditorInfo (int buttonPlay, int buttonLoad);
    int loadButton = 0;
    int playButton = 0;
    
    Envelopes sampleHitDelay;
    
    bool playSampleHit = false;
    int sampleDelayedTrigger = 0;
    
    
    //==============================================================================
    // colour for editor
    // TODO: find a better place
    int red = 0;
    int green = 5;
    int blue = 15;

    //==============================================================================
    TransitionFxAudioProcessor();
    ~TransitionFxAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //==============================================================================
    // triggers
    void setEnvTrigger(int trigger);
    double getEnvTrigger();
    
    //==============================================================================
    //create valuetree
    AudioProcessorValueTreeState tree;
    
    //==============================================================================
    // DAW waardes
    double* bpm;
    AudioPlayHead::FrameRateType framerate;
    double* sampleRate;
    int timeSigNum;
    int timeSigDenom;
    double currentPlayheadPosition; // als meervoud van de numerator is zit je op het begin van een maat
    bool isDawPlaying; // alleen wanneer de daw speelt, linken aan begin van een maat
    
    //==============================================================================
    // subvoices
    const int amountOfSubvoices = 3;
    
    //==============================================================================
    // Oscillator
    int oscMenuDefaultValues[3][4] = {
        {5,0,0,0},
        {2,0,0,0},
        {3,0,0,0}
    };
    
    string oscMenus[3][4] = {       // voices, oscs
        {"osc1MenuV1ID", "osc2MenuV1ID", "osc3MenuV1ID", "osc4MenuV1ID"},
        {"osc1MenuV2ID", "osc2MenuV2ID", "osc3MenuV2ID", "osc4MenuV2ID"},
        {"osc1MenuV3ID", "osc2MenuV3ID", "osc3MenuV3ID", "osc4MenuV3ID"}
    };
    string oscGains[3][4] = {       // voices, oscs
        {"osc1GainV1ID", "osc2GainV1ID", "osc3GainV1ID", "osc4GainV1ID"},
        {"osc1GainV2ID", "osc2GainV2ID", "osc3GainV2ID", "osc4GainV2ID"},
        {"osc1GainV3ID", "osc2GainV3ID", "osc3GainV3ID", "osc4GainV3ID"}
    };
    string oscDetunes[3][4] = {     // voices, oscs
        {"osc1DetuneV1ID", "osc2DetuneV1ID", "osc3DetuneV1ID", "osc4DetuneV1ID"},
        {"osc1DetuneV2ID", "osc2DetuneV2ID", "osc3DetuneV2ID", "osc4DetuneV2ID"},
        {"osc1DetuneV3ID", "osc2DetuneV3ID", "osc3DetuneV3ID", "osc4DetuneV3ID"}
    };
    
    string oscMenusNames[3][4];
    string oscMenusLabels[3][4];
    string oscGainsNames[3][4];
    string oscGainsLabels[3][4];
    string oscDetunesNames[3][4];
    string oscDetunesLabels[3][4];

    string osc1AmountOfOscVoicesIDS[3] = {"osc1VoiceCountSliderV1ID", "osc1VoiceCountSliderV2ID", "osc1VoiceCountSliderV3ID"};
    string osc1VoicesPitchOffsetIDS[3] = {"osc1VoicePitchOfsetSliderV1ID", "osc1VoicePitchOfsetSliderV2ID", "osc1VoicePitchOfsetSliderV3ID"};
    string osc1AmountOfOscVoicesNames[3], osc1AmountOfOscVoicesLabels[3], osc1VoicesPitchOffsetNames[3], osc1VoicesPitchOffsetLabels[3];
    
    //==============================================================================
    // LFO1
    string lfo1MenuIDs[1] = {"lfo1MenuID"};
    string lfo1FreqSliderIds[1] = {"lfo1FreqSliderID"};
    
    string lfo1MenuNames[1];
    string lfo1MenuLabels[1];
    string lfo1FreqSliderNames[1];
    string lfo1FreqSliderLabels[1];
    
    // LFO2
    string lfo2Menus[3][2] = {
        {"lfo1MenuV1ID", "lfo2MenuV1ID"},
        {"lfo1MenuV2ID", "lfo2MenuV2ID"},
        {"lfo1MenuV3ID", "lfo2MenuV3ID"}
    };
    string lfo2FreqSliders[3][2] = {
        {"lfo1FreqSliderV1ID", "lfo2FreqSliderV1ID"},
        {"lfo1FreqSliderV2ID", "lfo2FreqSliderV2ID"},
        {"lfo1FreqSliderV3ID", "lfo2FreqSliderV3ID"}
    };
    
    string lfo2MenusNames[3][2];
    string lfo2MenusLabels[3][2];
    string lfo2FreqSlidersNames[3][2];
    string lfo2FreqSlidersLabels[3][2];
   
    //==============================================================================
    // Reverb
    string reverbSliderIDs[5] = {"reverbRoomSizeSliderID", "reverbWetSliderID", "reverbDrySliderID" , "reverbDampingSliderID", "reverbWidthSliderID"};
    string reverbSliderNames[5];
    string reverbSliderLabels[5];
    
    //==============================================================================
    // env2
    string env2OnOffButtonsP    [3][4] = {
        {"onOffButtonV1ID", "onOffButtonOVDV1ID", "onOffButtonPanV1ID", "onOffButtonGainV1ID"},
        {"onOffButtonV2ID", "onOffButtonOVDV2ID", "onOffButtonPanV2ID", "onOffButtonGainV2ID"},
        {"onOffButtonV3ID", "onOffButtonOVDV3ID", "onOffButtonPanV3ID", "onOffButtonGainV3ID"}
    };
    string env2DopplerButtonsP  [3][4] = {
        {"dopplerButtonV1ID", "dopplerButtonOVDV1ID", "dopplerButtonPanV1ID", "dopplerButtonGainV1ID"},
        {"dopplerButtonV2ID", "dopplerButtonOVDV2ID", "dopplerButtonPanV2ID", "dopplerButtonGainV2ID"},
        {"dopplerButtonV3ID", "dopplerButtonOVDV3ID", "dopplerButtonPanV3ID", "dopplerButtonGainV3ID"}
    };
    string env2AttackSlidersP   [3][4] = {
        {"env2AttackSliderV1ID", "env2AttackSliderOVDV1ID", "env2AttackSliderPanV1ID", "env2AttackSliderGainV1ID"},
        {"env2AttackSliderV2ID", "env2AttackSliderOVDV2ID", "env2AttackSliderPanV2ID", "env2AttackSliderGainV2ID"},
        {"env2AttackSliderV3ID", "env2AttackSliderOVDV3ID", "env2AttackSliderPanV3ID", "env2AttackSliderGainV3ID"}
    };
    string env2ReleaseSlidersP  [3][4] = {
        {"env2ReleaseSliderV1ID", "env2ReleaseSliderOVDV1ID", "env2ReleaseSliderPanV1ID", "env2ReleaseSliderGainV1ID"},
        {"env2ReleaseSliderV2ID", "env2ReleaseSliderOVDV2ID", "env2ReleaseSliderPanV2ID", "env2ReleaseSliderGainV2ID"},
        {"env2ReleaseSliderV3ID", "env2ReleaseSliderOVDV3ID", "env2ReleaseSliderPanV3ID", "env2ReleaseSliderGainV3ID"}
    };
    string env2DelaySlidersP    [3][4] = {
        {"env2DelaySliderV1ID", "env2DelaySliderOVDV1ID", "env2DelaySliderPanV1ID", "env2DelaySliderGainV1ID"},
        {"env2DelaySliderV2ID", "env2DelaySliderOVDV2ID", "env2DelaySliderPanV2ID", "env2DelaySliderGainV2ID"},
        {"env2DelaySliderV3ID", "env2DelaySliderOVDV3ID", "env2DelaySliderPanV3ID", "env2DelaySliderGainV3ID"}
    };
    string env2ForwardsButtonsP [3][4] = {
        {"env2ForwardsButtonV1ID", "env2ForwardsButtonOVDV1ID", "env2ForwardsButtonPanV1ID", "env2ForwardsButtonGainV1ID"},
        {"env2ForwardsButtonV2ID", "env2ForwardsButtonOVDV2ID", "env2ForwardsButtonPanV2ID", "env2ForwardsButtonGainV2ID"},
        {"env2ForwardsButtonV3ID", "env2ForwardsButtonOVDV3ID", "env2ForwardsButtonPanV3ID", "env2ForwardsButtonGainV3ID"}
    };
    string env2RangeSlidersP    [2][3][4] = {
        { {"env2LowRangeSliderV1ID", "env2LowRangeSliderOVDV1ID", "env2LowRangeSliderPanV1ID", "env2LowRangeSliderGainV1ID"},
          {"env2LowRangeSliderV2ID", "env2LowRangeSliderOVDV2ID", "env2LowRangeSliderPanV2ID", "env2LowRangeSliderGainV2ID"},
            {"env2LowRangeSliderV3ID", "env2LowRangeSliderOVDV3ID", "env2LowRangeSliderPanV3ID", "env2LowRangeSliderGainV3ID"} },
        { {"env2HighRangeSliderV1ID", "env2HighRangeSliderOVDV1ID", "env2HighRangeSliderPanV1ID", "env2HighRangeSliderGainV1ID"},
          {"env2HighRangeSliderV2ID", "env2HighRangeSliderOVDV2ID", "env2HighRangeSliderPanV2ID", "env2HighRangeSliderGainV2ID"},
          {"env2HighRangeSliderV3ID", "env2HighRangeSliderOVDV3ID", "env2HighRangeSliderPanV3ID", "env2HighRangeSliderGainV3ID"} }
    };
    string env2SustainButtonsP [3][4] = {
        {"sustainButtonV1ID", "sustainButtonOVDV1ID", "sustainButtonPanV1ID", "sustainButtonGainV1ID"},
        {"sustainButtonV2ID", "sustainButtonOVDV2ID", "sustainButtonPanV2ID", "sustainButtonGainV2ID"},
        {"sustainButtonV3ID", "sustainButtonOVDV3ID", "sustainButtonPanV3ID", "sustainButtonGainV3ID"}
    };
    
    string env2OnOffButtonsNames    [3][4];
    string env2OnOffButtonsLabels   [3][4];
    string env2DopplerButtonsNames  [3][4];
    string env2DopplerButtonsLabels [3][4];
    string env2sustainButtonsNames  [3][4];
    string env2sustainButtonsLabels [3][4];
    string env2AttackSlidersNames   [3][4];
    string env2AttackSlidersLabels  [3][4];
    string env2ReleaseSlidersNames  [3][4];
    string env2ReleaseSlidersLabels [3][4];
    string env2DelaySlidersNames    [3][4];
    string env2DelaySlidersLabels   [3][4];
    string envForwardsButtonsNames  [3][4];
    string envForwardsButtonsLabels [3][4];
    string env2RangeSlidersNames    [2][3][4];
    string env2RangeSlidersLabels   [2][3][4];
    
    //==============================================================================
    // Env3
    string env3OnOffButtonsP[6] = {"env3OnOffButtonLFOID","env3OnOffButtonFilterID", "env3OnOffButtonPanID", "env3OnOffButtonFilterResonanceID", "env3OnOffButtonReverbID", "env3OnOffButtonDetuneID"};
    string env3DopplerButtonsP[6] = {"env3DopplerButtonLFOID", "env3DopplerButtonFilterID", "env3DopplerButtonPanID", "env3DopplerButtonFilterResonanceID", "env3DopplerButtonReverbID", "env3DopplerButtonDetuneID"};
    string env3AttackSlidersP[6] = {"env3AttackSliderLFOID", "env3AttackSliderFilterID", "env3AttackSliderPanID", "env3AttackSliderFilterResonanceID", "env3AttackSliderReverbID", "env3AttackSliderDetune"};
    string env3ReleaseSliderP[6] = {"env3ReleaseSliderLFOID", "env3ReleaseSliderFilterID", "env3ReleaseSliderPanID", "env3ReleaseSliderFilterResonanceID", "env3ReleaseSliderReverbID", "env3ReleaseSliderDetuneID"};
    string env3DelaySliderP[6] = {"env3DelaySliderLFOID", "env3DelaySliderFilterID", "env3DelaySliderPanID", "env3DelaySliderFilterResonanceID", "env3DelaySliderReverbID", "env3DelaySliderDetuneID"};
    string env3ForwardsButtonsP[6] = {"env3ForwardsButtonLFOID", "env3ForwardsButtonFilterID", "env3ForwardsButtonPanID", "env3ForwardsButtonFilterResonanceID", "env3ForwardsButtonReverbID", "env3ForwardsButtonDetuneID"};
    string env3LowRanges[6] = {"env3LowRangesLFOID", "env3LowRangesFilterID", "env3LowRangesPanID", "env3LowRangesFilterResonanceID", "env3LowRangesReverbID", "env3LowRangesDetuneID"};
    string env3HighRanges[6] = {"env3HighRangesLFOID", "env3HighRangesFilterID", "env3HighRangesPanID", "env3HighRangesFilterResonanceID", "env3HighRangesReverbID", "env3HighRangesDetuneID"};
    string env3SustainButtonsP[6] = {"env3SustainButtonLFOID","env3SustainButtonFilterID", "env3SustainButtonPanID", "env3SustainButtonFilterResonanceID", "env3SustainButtonReverbID", "env3SustainButtonDetuneID"};
    
    string env3AttackSliderNames    [6];
    string env3AttackSliderLabels   [6];
    string env3ReleaseSliderNames   [6];
    string env3ReleaseSliderLabels  [6];
    string env3DelaySliderNames     [6];
    string env3DelaySliderLabels    [6];
    string env3OnOffButtonNames     [6];
    string env3OnOffButtonLabels    [6];
    string env3SustainButtonNames   [6];
    string env3SustainButtonLabels  [6];
    string env3DopplerButtonNames   [6];
    string env3DopplerButtonLabels  [6];
    string env3ForwardsButtonNames  [6];
    string env3ForwardsButtonLabels [6];
    string env3LowRangesNames       [6];
    string env3LowRangesLabels      [6];
    string env3HighRangesNames      [6];
    string env3HighRangesLabels     [6];
    
    //==============================================================================
    // Env1 Points
    string env1PointSliderIDs[6] = {"env1PointSlider1ID", "env1PointSlider2ID", "env1PointSlider3ID", "env1PointSlider4ID", "env1PointSlider5ID", "env1PointSlider6ID"};
    string env1PointReleaseSliderIDs[6] = {"env1PointReleaseSlider1ID", "env1PointReleaseSlider2ID", "env1PointReleaseSlider3ID", "env1PointReleaseSlider4ID", "env1PointReleaseSlider5ID", "env1PointReleaseSlider6ID"};
    
    string env1PointSliderNames[6];
    string env1PointSliderLabels[6];
    string env1PointReleaseSliderNames[6];
    string env1PointReleaseSliderLabels[6];
    
    //==============================================================================
    // master
    string voiceMixSliderIDsP[3] = {"masterVoiceMixSliderV1ID", "masterVoiceMixSliderV2ID", "masterVoiceMixSliderV3ID"};
    string voicePanSliderIDsP[3] = {"masterVoicePanSliderV1ID", "masterVoicePanSliderV2ID", "masterVoicePanSliderV3ID"};
    string voiceMuteButtonIDsP[3] = {"voiceMuteButtonV1ID", "voiceMuteButtonV2ID", "voiceMuteButtonV3ID"};
    string voiceSoloButtonIDsP[3] = {"voiceSoloButtonV1ID", "voiceSoloButtonV2ID", "voiceSoloButtonV3ID"};
    
    string voiceMixSliderNames[3];
    string voiceMixSliderLabels[3];
    string voicePanSliderNames[3];
    string voicePanSliderLabels[3];
    string voiceMuteButtonNames[3];
    string voiceMuteButtonLabels[3];
    string voiceSoloButtonNames[3];
    string voiceSoloButtonLabels[3];
    
    //==============================================================================
    // modulation editor
    string modulationMasterTriggersP[6] = {"env3ModulationMasterTrigger1ID","env3ModulationMasterTrigger2ID", "env3ModulationMasterTrigger3ID", "env3ModulationMasterTrigger4ID", "env3ModulationMasterTrigger5ID", "env3ModulationMasterTrigger6ID"};
    
    string modulationMasterNames[6];
    string modulationMasterLabels[6];
    
    string modulationVoiceTriggersP[3][6] = {
        {"env3ModulationVoice1Trigger1ID","env3ModulationVoice1Trigger2ID", "env3ModulationVoice1Trigger3ID", "env3ModulationVoice1Trigger4ID", "env3ModulationVoice1Trigger5ID", "env3ModulationVoice1Trigger6ID"},
        {"env3ModulationVoice2Trigger1ID","env3ModulationVoice2Trigger2ID", "env3ModulationVoice2Trigger3ID", "env3ModulationVoice2Trigger4ID", "env3ModulationVoice2Trigger5ID", "env3ModulationVoice2Trigger6ID"},
        {"env3ModulationVoice3Trigger1ID","env3ModulationVoice3Trigger2ID", "env3ModulationVoice3Trigger3ID", "env3ModulationVoice3Trigger4ID", "env3ModulationVoice3Trigger5ID", "env3ModulationVoice3Trigger6ID"}
    };
    
    string modulationVoiceNames[3][6];
    string modulationVoiceLabels[3][6];

    
private:
    
    // timelinking
    bool hostLinked = false;
    double noteTimeValue;
    int timeLinkSelection;
    double sRate;
    double usedFramerate;
    
    // time setting
    int isNoteOff = 0;
    int notePlayed = true;
    int oldEnvTrigger = 0;
    float linkStartPositionInBeats = 2;
    
    //==============================================================================
    // lfo
    float lfoMenuBIDs[2], lfoFreqSliderBIDs[2];
    
    // master
    float masterAttack, masterRelease;
    
    // variabelen
    double lastSampleRate;
    
    //==============================================================================
    // filter
    dsp::ProcessorDuplicator<dsp::IIR::Filter <float>, dsp::IIR::Coefficients <float>> lowPassFilter;
    double filterCutOffOut;
    double filterResonanceOut;
    double filterCutOff, filterResonance, filterDopplerButton, filterOnOffButton, filterEnvOnOffButton, filterResonanceDopplerButton, filterResonanceOnOffButton;
    Envelopes env1FCutOff;
    Envelopes env2FResonance;
    
    // reverb
    enum{
        reverbIndex
    };
    dsp::ProcessorChain <juce::dsp::Reverb> fxChain;
    dsp::Reverb::Parameters reverbParameters;
    
    double reverbEnvOnOffButton, reverbDopplerButton;
    float verbWet, verbWetOut;
    Envelopes env3RWet;

    //==============================================================================
    // wel of niet xml aan
    bool saveXml = true;
    
    //==============================================================================
    // initialisatie
    bool firstInit[5] = {true, true, true, true, true}; // values to check if first initialisation is true
    float initVoiceValue[5] = {0,0,0,0,0};  // initial voicevalues for initialisation of every voice
    float* voiceValue;  // which voice is currently being edited
    float* voiceAmountValue;
    
    //==============================================================================
    // Create Synth
    Synthesiser mySynth;
    
    // create voice
    SynthVoice* myVoice;
    SubVoice* mySubVoice;
    
    // set amount of voices
    const int numVoices = 1;
    
    //==============================================================================
    int envtrig = 0;
    
    //==============================================================================
    // samplecounter
    int delayedTrigger = 0;
    int isNoteOn = false;
    Envelopes delayEnv;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TransitionFxAudioProcessor)
};
