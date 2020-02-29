/*
  ==============================================================================

    DistortionEditor.cpp
    Created: 14 Nov 2018 12:44:09pm
    Author:  Stijn de Koning

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "DistortionEditor.h"

//==============================================================================
DistortionEditor::DistortionEditor(TransitionFxAudioProcessor& p) : processor(p)
{
    // Sliders
    distortionDriveSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    distortionDriveSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(&distortionDriveSlider);
    distortionDriveSliderTree = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "distortionDriveSliderID", distortionDriveSlider);
    
    distortionRangeSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    distortionRangeSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(&distortionRangeSlider);
    distortionRangeSliderTree = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "distortionRangeSliderID", distortionRangeSlider);
    
    distortionBlendSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    distortionBlendSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(&distortionBlendSlider);
    distortionBlendSliderTree = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "distortionBlendSliderID", distortionBlendSlider);
    
    distortionGainSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    distortionGainSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(&distortionGainSlider);
    distortionGainSliderTree = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "distortionGainSliderID", distortionGainSlider);
    
    // Button
    addAndMakeVisible(distortionOnOffButton);
    distortionOnOffButtonTree = new AudioProcessorValueTreeState::ButtonAttachment (processor.tree, "distortionOnOffButtonID", distortionOnOffButton);
    
    // Combobox
    distortionMenu.addItem("dist 1", 1);
    distortionMenu.addItem("dist Maxim", 2);
    distortionMenu.setJustificationType(Justification::centred);
    addAndMakeVisible(&distortionMenu);
    distortionMenuTree = new AudioProcessorValueTreeState::ComboBoxAttachment (processor.tree, "distortionMenuID", distortionMenu);
}

DistortionEditor::~DistortionEditor()
{
    distortionDriveSliderTree.~ScopedPointer();
    distortionRangeSliderTree.~ScopedPointer();
    distortionBlendSliderTree.~ScopedPointer();
    distortionGainSliderTree.~ScopedPointer();
    distortionOnOffButtonTree.~ScopedPointer();
    distortionMenuTree.~ScopedPointer();
}

void DistortionEditor::paint (Graphics& g)
{
    g.fillAll (Colours::black);
    
    Rectangle<int> area = getLocalBounds().reduced(40);
    
    g.setColour (Colours::grey);
    Rectangle<int> areaRR = area.withTrimmedTop(20);
    g.drawRoundedRectangle(areaRR.getX(), areaRR.getY(), areaRR.getWidth(), areaRR.getHeight(), 8, 2.0);
    g.setColour(Colour(processor.red, processor.green, processor.blue));
    g.fillRoundedRectangle(areaRR.getX(), areaRR.getY(), areaRR.getWidth(), areaRR.getHeight(), 8);
    
    g.setColour (Colours::lightcyan);
    Rectangle<int> areaText = getLocalBounds().reduced(60).removeFromBottom(40);
    g.drawText ("Drive", areaText.removeFromLeft(80), Justification::centred);
    g.drawText ("Range", areaText.removeFromLeft(80), Justification::centred);
    g.drawText ("Blend", areaText.removeFromLeft(80), Justification::centred);
    g.drawText ("Volume", areaText.removeFromLeft(80), Justification::centred);
}

void DistortionEditor::resized()
{
    Rectangle<int> area2 = getLocalBounds().reduced(60);
    distortionOnOffButton.setBounds(area2.removeFromTop(20).removeFromRight(80));
    distortionMenu.setBounds(area2.removeFromTop(60).removeFromRight(80));
    
    Rectangle<int> area = getLocalBounds().reduced(60).withTrimmedBottom(40);
    distortionDriveSlider.setBounds(area.removeFromLeft(80));
    distortionRangeSlider.setBounds(area.removeFromLeft(80));
    distortionBlendSlider.setBounds(area.removeFromLeft(80));
    distortionGainSlider.setBounds(area.removeFromLeft(80));
}
