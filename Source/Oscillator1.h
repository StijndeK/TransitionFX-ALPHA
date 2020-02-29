/*
  ==============================================================================

    Oscillator1.h
    Created: 29 Oct 2018 11:10:36am
    Author:  Stijn de Koning

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class Oscillator1    : public Component, public ComboBox::Listener
{
public:
    Oscillator1(TransitionFxAudioProcessor&, string (&IDS)[4], string (&Gains)[4], string (&Detunes)[4]);
    ~Oscillator1();

    void paint (Graphics&) override;
    void resized() override;
    
    void comboBoxChanged(ComboBox *comboBoxThatHasChanged) override;
    void updateToggleState (Button* button);
    
    // combobox
    ComboBox osc1Menu[4];
    ScopedPointer <AudioProcessorValueTreeState::ComboBoxAttachment> osc1MenuTree[4];
    ScopedPointer <AudioProcessorValueTreeState::ComboBoxAttachment> osc1MenuTreeV2[4];

    // sliders
    Slider osc1Gain[4];
    Slider osc1Detune[4];
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> osc1DetuneTree[4];
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> osc1GainTree[4];
    
    // buttons
    TextButton randomButton { "Random" };
    ScopedPointer <AudioProcessorValueTreeState::ButtonAttachment> randomButtonTree;
    
    TextButton osc1DetuneUpSetter[4];
    TextButton osc1DetuneDownSetter[4];
    
    // amount of oscs
    const int amountOfOscillators = 4;
    
private:
    string componentName;
    
    // values to jump between
    float detuneValues[19] = {-3, -2.583, -2.333, -2, -1.583, -1.333, -1, -0.583, -0.333, 0, 0.333, 0.583, 1, 1.333, 1.583, 2, 2.333, 2.583, 3};
    int currentnumb[5] {9,9,9,9,9};
    
    // processor
    TransitionFxAudioProcessor& processor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Oscillator1)
};
