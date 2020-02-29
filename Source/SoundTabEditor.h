/*
  ==============================================================================

    SoundTabEditor.h
    Created: 1 May 2019 11:14:33pm
    Author:  Stijn de Koning

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "SourceSoundEditor.h"

// ALL SPECIFIC TABS
class SoundTabEditor    : public Component
{
public:
    SoundTabEditor(TransitionFxAudioProcessor&);
    ~SoundTabEditor();
    
    void paint (Graphics&) override;
    void resized() override;
    
    void updateToggleState (Button* button);
    
    void setVoices();

    // processor
    TransitionFxAudioProcessor& processor;
    
    // buttons
    TextButton voiceButtons[3];
    TextButton oscSelectionButtons[3];
    TextButton multiSelectionButtons[3];
    TextButton modulationButtons[4];
    TextButton masterButton {"master"};
    TextButton impactButton {"impact"};
    TextButton effectsButton {"effects"};
    TextButton envelopeButton {"envelope"};
    TextButton mixerButton {"mixer"};

    
    // sliders for selection for voices
    Slider voicesSlider;
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> voicesSliderTree;
    Slider voiceAmountSlider;
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> voiceAmountSliderTree;
    
    // sliders for gain and pan
    Slider masterGainSlider;
    Slider masterPanSlider;
    Slider masterVoiceMixSliders[3];
    Slider masterVoicePanSliders[3];
    
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> masterGainSliderTree;
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> masterPanSliderTree;
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> masterVoiceMixSlidersTree[3];
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> masterVoicePanSlidersTree[3];
    
private:
    bool voiceOnOff[3];
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoundTabEditor)
};
