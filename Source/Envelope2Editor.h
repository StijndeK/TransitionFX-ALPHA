/*
  ==============================================================================

    Envelope2Editor.h
    Created: 6 Nov 2018 10:21:43am
    Author:  Stijn de Koning

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/*
*/
class Envelope2Editor    : public Component, public ComboBox::Listener, public Slider::Listener
{
public:
    
    Envelope2Editor(TransitionFxAudioProcessor&, string (&OnOffButtons)[4], string (&DopplerButtons)[4], string (&AttackSliders)[4], string (&ReleaseSliders)[4], string (&DelaySliders)[4], string (&ForwardsButtons)[4], string (&LowRangeSliders)[4], string (&HighRangeSliders)[4], string (&SustainButtons)[4]);
    ~Envelope2Editor();

    void paint (Graphics&) override;
    void resized() override;
    
    void updateToggleState (Button* button);
    void comboBoxChanged(ComboBox *comboBoxThatHasChanged) override;
    void sliderValueChanged(Slider *slider) override;
    
    void selectGUI();
    
    // menus
    ComboBox env2Menu;
    ScopedPointer <AudioProcessorValueTreeState::ComboBoxAttachment> env2MenuTree;
    
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

private:
    const int amountOfOptions = 4;
    
    string componentName;
    
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

    // processor
    TransitionFxAudioProcessor& processor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Envelope2Editor)
};
