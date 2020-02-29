/*
  ==============================================================================

    Filter.cpp
    Created: 12 Nov 2018 9:43:00am
    Author:  Stijn de Koning

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Filter.h"

//==============================================================================
Filter::Filter(TransitionFxAudioProcessor& p) : processor(p)
{
    // set slider parameters
    filterCutOffSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    filterCutOffSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(&filterCutOffSlider);
    filterCutOffSliderTree = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "filterCutOffSliderID", filterCutOffSlider);
    
    filterResonanceSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    filterResonanceSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(&filterResonanceSlider);
    filterResonanceSliderTree = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "filterResonanceSliderID", filterResonanceSlider);
    
    // set button parameters
    addAndMakeVisible(filterOnOffButton);
    filterOnOffButtonTree = new AudioProcessorValueTreeState::ButtonAttachment (processor.tree, "filterOnOffButtonID", filterOnOffButton);
}

Filter::~Filter()
{
    filterCutOffSliderTree.~ScopedPointer();
    filterResonanceSliderTree.~ScopedPointer();
    filterOnOffButtonTree.~ScopedPointer();
}

void Filter::paint (Graphics& g)
{
    g.fillAll (Colours::black);
    
    Rectangle<int> area = getLocalBounds().reduced(40);
    
    g.setColour (Colours::grey);
    Rectangle<int> areaRR = area.withTrimmedTop(20);
    g.drawRoundedRectangle(areaRR.getX(), areaRR.getY(), areaRR.getWidth(), areaRR.getHeight(), 8, 2.0);
    g.setColour(Colour(processor.red, processor.green, processor.blue));
    g.fillRoundedRectangle(areaRR.getX(), areaRR.getY(), areaRR.getWidth(), areaRR.getHeight(), 8);
    
    Rectangle<int> areaText = getLocalBounds().reduced(60).removeFromBottom(40);
    g.drawText ("Cutoff", areaText.removeFromLeft(80), Justification::centred);
    g.drawText ("Resonance", areaText.removeFromLeft(80), Justification::centred);
    
    // draw slider text
    g.setColour (Colours::white);
    Rectangle<int> area2 = getLocalBounds().reduced(60).removeFromBottom(40);
    g.drawText ("Cutoff", area2.removeFromLeft(80), Justification::centred);
    g.drawText ("Resonance", area2.removeFromLeft(80), Justification::centred);
}

void Filter::resized()
{
    Rectangle<int> area2 = getLocalBounds().reduced(60);
    filterOnOffButton.setBounds(area2.removeFromTop(60).removeFromRight(80));
    
    Rectangle<int> area = getLocalBounds().reduced(60).withTrimmedBottom(40);
    filterCutOffSlider.setBounds(area.removeFromLeft(80));
    filterResonanceSlider.setBounds(area.removeFromLeft(80));
}
