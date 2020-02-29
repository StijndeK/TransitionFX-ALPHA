/*
  ==============================================================================

    LFOEditor.h
    Created: 1 Nov 2018 2:52:20pm
    Author:  Stijn de Koning

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/*
*/
class LFOEditor    : public Component
{
public:
    LFOEditor(TransitionFxAudioProcessor&, string menuID, string freqSliderID);
    ~LFOEditor();

    void paint (Graphics&) override;
    void resized() override;

private:
    string componentName;

    //create ComboBox + valuetree
    ComboBox lfo1Menu;
    ScopedPointer <AudioProcessorValueTreeState::ComboBoxAttachment> lfo1MenuTree;

    //create Sliders + valuetree
    Slider lfo1FreqSlider;
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> lfo1FreqSliderTree;
    
    // processor
    TransitionFxAudioProcessor& processor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LFOEditor)
};
