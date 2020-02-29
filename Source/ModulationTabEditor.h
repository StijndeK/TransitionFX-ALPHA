/*
  ==============================================================================

    ModulationTabEditor.h
    Created: 6 May 2019 9:45:07pm
    Author:  Stijn de Koning

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class ModulationTabEditor    : public Component
{
public:
    ModulationTabEditor(TransitionFxAudioProcessor&);
    ~ModulationTabEditor();

    void paint (Graphics&) override;
    void resized() override;
    
    void updateToggleState (Button* button);

    // buttons
    TextButton pitchModButton {"Pitch"};
    TextButton panModButton {"Panning"};
    TextButton gainModButton {"Gain"};
    TextButton multiModButton {"Multiply Oscs"};
    
    ToggleButton onOffButtons[4];
    
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
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModulationTabEditor)
};
