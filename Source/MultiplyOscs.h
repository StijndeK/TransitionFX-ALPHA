/*
  ==============================================================================

    MultiplyOscs.h
    Created: 5 May 2019 11:57:37pm
    Author:  Stijn de Koning

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/*
*/
class MultiplyOscs    : public Component
{
public:
    MultiplyOscs(TransitionFxAudioProcessor&, string VoicesID, string VoicesPitchOffset);
    ~MultiplyOscs();

    void paint (Graphics&) override;
    void resized() override;
    
    void updateToggleState (Button* button);


private:
    // values to jump between
    float detuneValues[19] = {-3, -2.583, -2.333, -2, -1.583, -1.333, -1, -0.583, -0.333, 0, 0.333, 0.583, 1, 1.333, 1.583, 2, 2.333, 2.583, 3};
    int currentnumb = 9;
    
    TextButton osc1DetuneUpSetter;
    TextButton osc1DetuneDownSetter;
    
    Slider osc1VoiceCountSlider;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> osc1VoiceCountSliderTree;
    
    Slider osc1VoicePitchOfsetSlider;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> osc1VoicePitchOfsetSliderTree;
    
    // processor
    TransitionFxAudioProcessor& processor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MultiplyOscs)
};
