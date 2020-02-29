/*
  ==============================================================================

    EffectsEditor.h
    Created: 14 Nov 2018 12:46:25pm
    Author:  Stijn de Koning

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "Filter.h"
#include "DistortionEditor.h"
#include "ChorusEditor.h"
#include "ReverbEditor.h"


//==============================================================================
/*
*/
class EffectsEditor    : public Component , public ComboBox::Listener
{
public:
    EffectsEditor(TransitionFxAudioProcessor&);
    ~EffectsEditor();

    void paint (Graphics&) override;
    void resized() override;
    
    void updateToggleState (Button* button);

    
    void comboBoxChanged(ComboBox *comboBoxThatHasChanged) override;

private:
    // processor
    TransitionFxAudioProcessor& processor;
    
    //create comboBox + valuetree
    ComboBox effectsMenu;
    ScopedPointer <AudioProcessorValueTreeState::ComboBoxAttachment> effectsMenuTree;
    
    Filter filter;
    DistortionEditor distortion;
    ChorusEditor chorus;
    ReverbEditor reverb;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EffectsEditor)
};
