/*
  ==============================================================================

    masterModulationTabEditor.h
    Created: 6 May 2019 10:14:19pm
    Author:  Stijn de Koning

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class masterModulationTabEditor    : public Component
{
public:
    masterModulationTabEditor(TransitionFxAudioProcessor&);
    ~masterModulationTabEditor();

    void paint (Graphics&) override;
    void resized() override;
    
    void updateToggleState (Button* button);
    
    // buttons
    TextButton pitchModButton {"Pitch"};
    TextButton panModButton {"Panning"};
    TextButton gainModButton {"Gain"};
    TextButton lfoModButton {"Tremelo"};
    TextButton filterModButton {"Filter"};
    TextButton reverbModButton {"Reverb"};
    
    // buttons
    ToggleButton onOffButtons[6];
    ScopedPointer <AudioProcessorValueTreeState::ButtonAttachment> aspectOnOffButtonMasterTree[6];
    
    // env drawing
    Path myPath;
    float middlePoint;
    float startAttack;
    float endAttack;
    float endRelease;
    float left, right;
    float endPoint;
    
    Path pathShape;
    Path pathForwardsShape;
    
private:
    // processor
    TransitionFxAudioProcessor& processor;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (masterModulationTabEditor)
};
