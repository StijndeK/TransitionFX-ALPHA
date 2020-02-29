/*
  ==============================================================================

    ChorusEditor.h
    Created: 15 Nov 2018 11:48:39am
    Author:  Stijn de Koning

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class ChorusEditor    : public Component
{
public:
    ChorusEditor(TransitionFxAudioProcessor&);
    ~ChorusEditor();

    void paint (Graphics&) override;
    void resized() override;
    
private:
    // create button + valuetree
    ToggleButton chorusOnOffButton   { "On/Off" };
    ScopedPointer<AudioProcessorValueTreeState::ButtonAttachment> chorusOnOffButtonTree;
    
    // processor
    TransitionFxAudioProcessor& processor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChorusEditor)
};
