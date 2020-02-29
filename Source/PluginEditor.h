#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "Oscillator1.h"
#include "Filter.h"
#include "HitsEditor.h"
#include "MasterEnvelopeEditor.h"
#include "SoundTabManager.h"

// CALLS ALL VISUALS
class TransitionFxAudioProcessorEditor  : public AudioProcessorEditor, public ComboBox::Listener, public Slider::Listener
{
public:
    TransitionFxAudioProcessorEditor (TransitionFxAudioProcessor&);
    ~TransitionFxAudioProcessorEditor();

    void paint (Graphics&) override;
    void resized() override;
    
    void comboBoxChanged(ComboBox *comboBoxThatHasChanged) override;
    void sliderValueChanged(Slider *slider) override;
    void updateToggleState (Button* button);

    void selectGUI();

private:
    const int pluginWidth = 640;

    // manual note trigger
    TextButton triggerButton { "Play" };
    
    // for checking if the middle value of range slider moved, maybe remove?
    float oldEnv2RangeSliderValues [4];
    float oldEnv2bRangeSliderValues[4];
    float oldEnv2cRangeSliderValues[4];
    
    // processor
    TransitionFxAudioProcessor& processor;
    
    // master envelope
    MasterEnvelopeEditor masterenv;
    
    // SOUND TAB EDITOR
    SoundTabManager soundTab;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TransitionFxAudioProcessorEditor)
};
