/*
  ==============================================================================

    DistortionEditor.h
    Created: 14 Nov 2018 12:44:09pm
    Author:  Stijn de Koning

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class DistortionEditor    : public Component
{
public:
    DistortionEditor(TransitionFxAudioProcessor&);
    ~DistortionEditor();

    void paint (Graphics&) override;
    void resized() override;
    
private:
    // create button + valuetree
    ToggleButton distortionOnOffButton   { "On/Off" };
    ScopedPointer<AudioProcessorValueTreeState::ButtonAttachment> distortionOnOffButtonTree;
    
    // create Sliders + valuetree
    Slider distortionDriveSlider;
    Slider distortionRangeSlider;
    Slider distortionBlendSlider;
    Slider distortionGainSlider;
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> distortionDriveSliderTree;
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> distortionRangeSliderTree;
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> distortionBlendSliderTree;
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> distortionGainSliderTree;
    
    // create combobox + valuetree
    ComboBox distortionMenu;
    ScopedPointer <AudioProcessorValueTreeState::ComboBoxAttachment> distortionMenuTree;
    
    // processor
    TransitionFxAudioProcessor& processor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistortionEditor)
};
