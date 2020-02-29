/*
  ==============================================================================

    MasterEditor.h
    Created: 31 Oct 2018 5:10:22pm
    Author:  Stijn de Koning

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/*
*/
class MasterEditor    : public Component
{
public:
    MasterEditor(TransitionFxAudioProcessor&);
    ~MasterEditor();

    void paint (Graphics&) override;
    void resized() override;
    
    // sliders
    Slider masterGainSlider;
    Slider masterPanSlider;
    Slider masterVoiceMixSliders[3];
    Slider masterVoicePanSliders[3];
    
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> masterGainSliderTree;
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> masterPanSliderTree;
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> masterVoiceMixSlidersTree[3];
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> masterVoicePanSlidersTree[3];

private:
    // processor
    TransitionFxAudioProcessor& processor;

    // buttons
    ToggleButton voicePitchTonalButton { "On/Off" };
    ScopedPointer <AudioProcessorValueTreeState::ButtonAttachment> voicePitchTonalButtonTree;
    ToggleButton voiceMuteButtons[3];
    ScopedPointer <AudioProcessorValueTreeState::ButtonAttachment> voiceMuteBottunsTree[3];
    ToggleButton voiceSoloButtons[3];
    ScopedPointer <AudioProcessorValueTreeState::ButtonAttachment> voiceSoloBottunsTree[3];
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MasterEditor)
};
