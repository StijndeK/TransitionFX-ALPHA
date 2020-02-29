/*
  ==============================================================================

    SourceSoundEditor.h
    Created: 26 Nov 2018 3:35:49pm
    Author:  Stijn de Koning

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "Oscillator1.h"
#include "HitsEditor.h"
#include "MultiplyOscs.h"

//==============================================================================
/*
*/
class SourceSoundEditor    : public Component
{
public:
    SourceSoundEditor(TransitionFxAudioProcessor&);
    ~SourceSoundEditor();

    void paint (Graphics&) override;
    void resized() override;
    
    // processor
    TransitionFxAudioProcessor& processor;
    
    // oscs
    Oscillator1 osc1;
    Oscillator1 osc2;
    Oscillator1 osc3;
    
    // multiply oscs
    MultiplyOscs multi1;
    MultiplyOscs multi2;
    MultiplyOscs multi3;
    
    // create hits
    HitsEditor hits;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SourceSoundEditor)
};
