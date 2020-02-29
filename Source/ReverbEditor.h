/*
  ==============================================================================

    ReverbEditor.h
    Created: 15 Nov 2018 3:12:35pm
    Author:  Stijn de Koning

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class ReverbEditor    : public Component
{
public:
    ReverbEditor(TransitionFxAudioProcessor&);
    ~ReverbEditor();

    void paint (Graphics&) override;
    void resized() override;
    
private:
    const int amountOfSliders = 5;

    Slider reverbSliders[5];
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> reverbSlidersTree[5];
    
    // create button + valuetree
    ToggleButton reverbOnOffButton   { "On/Off" };
    ScopedPointer <AudioProcessorValueTreeState::ButtonAttachment> reverbOnOffButtonTree;
    
    // processor
    TransitionFxAudioProcessor& processor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverbEditor)
};
