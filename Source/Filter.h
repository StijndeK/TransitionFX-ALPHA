/*
  ==============================================================================

    Filter.h
    Created: 12 Nov 2018 9:43:00am
    Author:  Stijn de Koning

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class Filter    : public Component
{
public:
    Filter(TransitionFxAudioProcessor&);
    ~Filter();

    void paint (Graphics&) override;
    void resized() override;
    
private:
    // buttons
    ToggleButton filterOnOffButton   { "On/Off" };
    ScopedPointer<AudioProcessorValueTreeState::ButtonAttachment> filterOnOffButtonTree;
    
    // sliders
    Slider filterCutOffSlider;
    Slider filterResonanceSlider;
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> filterCutOffSliderTree;
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> filterResonanceSliderTree;
    
    // processor
    TransitionFxAudioProcessor& processor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Filter)
};
