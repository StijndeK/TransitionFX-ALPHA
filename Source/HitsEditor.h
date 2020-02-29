/*
  ==============================================================================

    HitsEditor.h
    Created: 17 Nov 2018 9:45:46pm
    Author:  Stijn de Koning

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/*
*/
class HitsEditor    : public Component
{
public:
    HitsEditor(TransitionFxAudioProcessor&);
    ~HitsEditor();
    
    void updateToggleState (Button* button);

    void paint (Graphics&) override;
    void resized() override;
    
    Slider kickTempoOffsetSlider;
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> kickTempoOffsetSliderTree;

    TextEditor offsetAmountText;

private:
    // buttons
    TextButton playButton {"play"};
    TextButton loadButton {"load"};
    
    ToggleButton sampleHitOnOffButton { "On/Off" };
    ScopedPointer <AudioProcessorValueTreeState::ButtonAttachment> sampleHitOnOffButtonTree;

    // sliders
    Slider kickPitchSlider;
    Slider kickReleaseSlider;
    Slider kickGainSlider;
    Slider kickSampleGainSlider;

    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> kickPitchSliderTree;
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> kickReleaseSliderTree;
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> kickGainSliderTree;
    
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> kickSampleGainSliderTree;

    // buttons
    ToggleButton kickOnOffButton { "On/Off" };
    ScopedPointer <AudioProcessorValueTreeState::ButtonAttachment> kickOnOffButtonTree;
    
    ToggleButton kickDistortionOnOffButton { "Distortion" };
    ScopedPointer <AudioProcessorValueTreeState::ButtonAttachment> kickDistortionOnOffButtonTree;
    
    // processor
    TransitionFxAudioProcessor& processor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HitsEditor)
};
