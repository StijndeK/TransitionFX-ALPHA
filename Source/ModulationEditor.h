/*
  ==============================================================================

    ModulationEditor.h
    Created: 26 Nov 2018 3:30:15pm
    Author:  Stijn de Koning

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "LFOEditor.h"
#include "LFO2Editor.h"
#include "Envelope2Editor.h"
#include "Envelope3Editor.h"
#include "MasterEnvelopeEditor.h"
#include "SoundTabEditor.h"
#include <list>


//==============================================================================
/*
*/
class ModulationEditor    : public Component, public ComboBox::Listener, public Slider::Listener
{
public:
    ModulationEditor(TransitionFxAudioProcessor&);
    ~ModulationEditor();

    void paint (Graphics&) override;
    void resized() override;
    
    void comboBoxChanged(ComboBox *comboBoxThatHasChanged) override;
    void sliderValueChanged(Slider *slider) override;
    void updateToggleState (Button* button);
    
    void selectGui2();

    // processor
    TransitionFxAudioProcessor& processor;

    // envelops
    Envelope2Editor env2;
    Envelope2Editor env2b;
    Envelope2Editor env2c;
    
    // lfos
    LFOEditor lfo;
    LFO2Editor lfoV1;
    LFO2Editor lfoV2;
    LFO2Editor lfoV3;
    
    // comboboxes
    ComboBox modulationMenu;
    ComboBox modulationChoiceMenu;
    ComboBox modulationChoiceMenuVoices;
    ComboBox modulationTypeMenu;
    
    // envelopes
    Envelope3Editor env3;

private:
    // menu buttons
    TextButton masterButton {"Master"};
    TextButton voiceButton {"Voice"};
    
    TextButton openMenuButton {"Target"};
    
    std::string masterChoiceButtonNames[6] = {"LFO", "Filter", "Reverb", "Pan", "Gain", "Detune"};
    TextButton masterChoiceButtons[6];
    
    std::string voiceChoiceButtonNames[6] = {"Detune", "Gain", "Osc Voices Detune", "Pan", " ", " "};
    TextButton voiceChoiceButtons[6];
    
    ToggleButton aspectOnOffButtonMaster[6];
    ScopedPointer <AudioProcessorValueTreeState::ButtonAttachment> aspectOnOffButtonMasterTree[6];
    ToggleButton aspectOnOffButtonVoice[3][6];
    ScopedPointer <AudioProcessorValueTreeState::ButtonAttachment> aspectOnOffButtonVoiceTree[3][6];

    ToggleButton aspectOnOffButtonLFOMaster[6];

    // sound tab for voices
    SoundTabEditor masterEnvelope;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModulationEditor)
};
