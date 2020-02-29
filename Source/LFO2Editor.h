/*
  ==============================================================================

    LFO2Editor.h
    Created: 2 Dec 2018 8:59:07pm
    Author:  Stijn de Koning

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/*
*/
class LFO2Editor    : public Component, public ComboBox::Listener
{
public:
    LFO2Editor(TransitionFxAudioProcessor&, string (&menuIDs)[2], string (&fqSliderIDs)[2]);
    ~LFO2Editor();

    void paint (Graphics&) override;
    void resized() override;
    void comboBoxChanged(ComboBox *comboBoxThatHasChanged) override;
    
    void selectGUI();
    
    // create ComboBox
    ComboBox lfo2ComponentChoiceMenu;

private:
    const int amountOfLFOs = 2;
    
    string componentName;
    
    // create ComboBox + valuetree
    ComboBox lfo2Menu[2];
    ScopedPointer <AudioProcessorValueTreeState::ComboBoxAttachment> lfo2MenuTree[2];
    
    // create Sliders + valuetree
    Slider lfo2FreqSlider[2];
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> lfo2FreqSliderTree[2];
    
    // processor
    TransitionFxAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LFO2Editor)
};
