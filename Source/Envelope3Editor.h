/*
  ==============================================================================

    Envelope3Editor.h
    Created: 27 Nov 2018 10:55:24am
    Author:  Stijn de Koning

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/*
*/
class Envelope3Editor    : public Component , public ComboBox::Listener, public Slider::Listener
{
public:
    string componentName;
    
    Envelope3Editor(TransitionFxAudioProcessor&, string (&OnOffButtons)[6], string (&DopplerButtons)[6], string (&AttackSliders)[6], string (&ReleaseSliders)[6], string (&DelaySliders)[6], string (&ForwardsButtons)[6], string (&LowRanges)[6], string (&HighRanges)[6], string (&SustainButtons)[6]);
    ~Envelope3Editor();

    void paint (Graphics&) override;
    void resized() override;
    
    void comboBoxChanged(ComboBox *comboBoxThatHasChanged) override;
    void updateToggleState (Button* button);
    void sliderValueChanged(Slider *slider) override;

    void selectGUI();
    
    // sliders
    Slider env2AttackSlider[6];
    Slider env2ReleaseSlider[6];
    Slider env2DelaySlider[6];
    Slider env2RangeSlider[6];
    Slider env2LowRangeSlider[6];
    Slider env2HighRangeSlider[6];
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> env2AttackSliderTree[6];
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> env2ReleaseSliderTree[6];
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> env2DelaySliderTree[6];
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> env2LowRangeSliderTree[6];
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> env2HighRangeSliderTree[6];

    // buttons
    ToggleButton dopplerButton[6];
    ToggleButton onOffButton[6];
    ScopedPointer<AudioProcessorValueTreeState::ButtonAttachment> dopplerButtonTree[6];
    ScopedPointer<AudioProcessorValueTreeState::ButtonAttachment> onOffButtonTree[6];
    
    ToggleButton FBToggleButton[6];
    ScopedPointer <AudioProcessorValueTreeState::ButtonAttachment> FBToggleButtonTree[6];
    
    ToggleButton sustainButton[6];
    ScopedPointer <AudioProcessorValueTreeState::ButtonAttachment> sustainButtonTree[6];

    ShapeButton env3BackwardsButtons[6] {{"name2", Colours::darkgrey, Colours::black, Colours::darkblue}, {"name2", Colours::darkgrey, Colours::black, Colours::darkblue}, {"name2", Colours::darkgrey, Colours::black, Colours::darkblue}, {"name2", Colours::darkgrey, Colours::black, Colours::darkblue}, {"name2", Colours::darkgrey, Colours::black, Colours::darkblue}, {"name2", Colours::darkgrey, Colours::black, Colours::darkblue}};
    ShapeButton env3ForwardsButtons[6] {{"name2", Colours::darkgrey, Colours::black, Colours::darkblue}, {"name2", Colours::darkgrey, Colours::black, Colours::darkblue}, {"name2", Colours::darkgrey, Colours::black, Colours::darkblue}, {"name2", Colours::darkgrey, Colours::black, Colours::darkblue}, {"name2", Colours::darkgrey, Colours::black, Colours::darkblue}, {"name2", Colours::darkgrey, Colours::black, Colours::darkblue}};
    
    // paths
    Path pathShape;
    Path pathForwardsShape;
    
    // menus
    ComboBox env3Menu;
    ComboBox env3FilterMenu;

private:
    // modulation direction
    int dopplerOn = 0;
    int backwardsOn = 0;
    int forwardsOn = 1;
    
    // env drawing
    Path myPath;
    float middlePoint;
    float startAttack;
    float endAttack;
    float endRelease;
    float left, right;
    float endPoint;
    
    const int amountOfOptions = 6;

    // processor
    TransitionFxAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Envelope3Editor)
};
