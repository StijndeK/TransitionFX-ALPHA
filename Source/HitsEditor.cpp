/*
  ==============================================================================

    HitsEditor.cpp
    Created: 17 Nov 2018 9:45:46pm
    Author:  Stijn de Koning

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "HitsEditor.h"

//==============================================================================
HitsEditor::HitsEditor(TransitionFxAudioProcessor& p) : processor(p)
{
    addAndMakeVisible(offsetAmountText);
    offsetAmountText.setEnabled(false);
    
    // Sliders
    kickPitchSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    kickPitchSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(&kickPitchSlider);
    kickPitchSliderTree = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "kickPitchSliderID", kickPitchSlider);
    
    kickReleaseSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    kickReleaseSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(&kickReleaseSlider);
    kickReleaseSliderTree = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "kickReleaseSliderID", kickReleaseSlider);
    
    kickGainSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    kickGainSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(&kickGainSlider);
    kickGainSliderTree = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "kickGainSliderID", kickGainSlider);
    
    kickTempoOffsetSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    kickTempoOffsetSlider.setTextBoxStyle(Slider::NoTextBox, false, 60, 20);
    addAndMakeVisible(&kickTempoOffsetSlider);
    kickTempoOffsetSliderTree = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "kickTempoOffsetSliderID", kickTempoOffsetSlider);
    
    kickSampleGainSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    kickSampleGainSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(&kickSampleGainSlider);
    kickSampleGainSliderTree = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "kickSampleGainSliderID", kickSampleGainSlider);
    
    // Buttons
    addAndMakeVisible(kickOnOffButton);
    kickOnOffButtonTree = new AudioProcessorValueTreeState::ButtonAttachment (processor.tree, "kickOnOffButtonID", kickOnOffButton);
    
    addAndMakeVisible(kickDistortionOnOffButton);
    kickDistortionOnOffButtonTree = new AudioProcessorValueTreeState::ButtonAttachment (processor.tree, "kickDistortionOnOffButtonID", kickDistortionOnOffButton);
    
    addAndMakeVisible(sampleHitOnOffButton);
    sampleHitOnOffButton  .onClick = [this] { updateToggleState (&sampleHitOnOffButton);   };

    addAndMakeVisible(&playButton);
    addAndMakeVisible(&loadButton);
    playButton  .onClick = [this] { updateToggleState (&playButton);   };
    loadButton  .onClick = [this] { updateToggleState (&loadButton);   };
}

HitsEditor::~HitsEditor()
{
    kickPitchSliderTree.~ScopedPointer();
    kickReleaseSliderTree.~ScopedPointer();
    kickTempoOffsetSliderTree.~ScopedPointer();
    kickGainSliderTree.~ScopedPointer();
    kickOnOffButtonTree.~ScopedPointer();
    kickDistortionOnOffButtonTree.~ScopedPointer();
    kickSampleGainSliderTree.~ScopedPointer();
}

void HitsEditor::paint (Graphics& g)
{
    g.fillAll (Colours::black);
    
    Rectangle<int> area = getLocalBounds().reduced(10);
    
    g.setColour (Colours::grey);
    Rectangle<int> areaRR = area.withTrimmedTop(20);
    g.drawRoundedRectangle(areaRR.getX(), areaRR.getY(), areaRR.getWidth(), areaRR.getHeight(), 8, 2.0);
    g.setColour(Colour(processor.red, processor.green, processor.blue));
    g.fillRoundedRectangle(areaRR.getX(), areaRR.getY(), areaRR.getWidth(), areaRR.getHeight(), 8);
    
    g.setColour (Colours::white);
    g.setFont (16.0f);
    g.drawText ("Impacts", area, Justification::centredTop);
    
    Rectangle<int> areaText = getLocalBounds().reduced(40);
    Rectangle<int> areaSynth = areaText.removeFromTop((areaText.getHeight() / 2) - 40).removeFromBottom(40);
    g.drawText ("Frequency", areaSynth.removeFromLeft(80), Justification::centred);
    g.drawText ("Release ", areaSynth.removeFromLeft(80), Justification::centred);
    g.drawText ("Volume", areaSynth.removeFromLeft(80), Justification::centred);
    
    Rectangle<int> areaSample = areaText.removeFromBottom((areaText.getHeight() / 2) - 40).removeFromBottom(80);
    areaSample.removeFromLeft(160);
    g.drawText ("Volume", areaSample.removeFromLeft(80), Justification::centred);
    
    g.setColour (Colours::grey);
    g.drawLine(10 , (area.getHeight() + 30) / 2, area.getWidth() * 0.8, (area.getHeight() + 30) / 2);
    
    g.setColour (Colours::white);
    Rectangle<int> areaTitle1 = getLocalBounds().reduced(10).withTrimmedTop(30);
    g.drawText ("Synth ", areaTitle1.removeFromTop(20).withTrimmedLeft(40), Justification::left);

    Rectangle<int> areaTitle2 = getLocalBounds().reduced(10).withTrimmedTop(20).removeFromBottom(areaTitle1.getHeight() / 2);
    g.drawText ("Sample ", areaTitle2.removeFromTop(20).withTrimmedLeft(40), Justification::left);
    
    g.setColour (Colours::grey);
    g.drawLine(area.getWidth() * 0.8 , areaRR.getBottom() , area.getWidth() * 0.8, areaRR.getY());
    
    g.setColour (Colours::white);
    Rectangle<int> offsetNameArea = getLocalBounds().removeFromRight(150).removeFromTop(80).withTrimmedTop(30);
    g.drawText ("Offset ", offsetNameArea.withTrimmedBottom(20), Justification::centred);
}

void HitsEditor::resized()
{
    Rectangle<int> area2 = getLocalBounds().reduced(40);
    kickOnOffButton          .setBounds(area2.removeFromTop(20).removeFromLeft(160).withTrimmedLeft(80));
    
    Rectangle<int> area = getLocalBounds().reduced(40).withTrimmedBottom(40);
    Rectangle<int> areaSynth = area.removeFromTop(area.getHeight() / 2);
    
    kickPitchSlider          .setBounds(areaSynth.removeFromLeft(80));
    kickReleaseSlider        .setBounds(areaSynth.removeFromLeft(80));
    kickGainSlider           .setBounds(areaSynth.removeFromLeft(80));
    kickDistortionOnOffButton.setBounds(areaSynth.removeFromLeft(80));
    
    kickTempoOffsetSlider    .setBounds(getLocalBounds().removeFromRight(150).withTrimmedBottom(100).withTrimmedTop(100));
    offsetAmountText         .setBounds(getLocalBounds().removeFromRight(150).withTrimmedBottom(60).removeFromBottom(20).withTrimmedLeft(40).withTrimmedRight(40));
    
    Rectangle<int> area3 = getLocalBounds().reduced(40);
    sampleHitOnOffButton     .setBounds(area3.removeFromBottom(area3.getHeight() / 2).withTrimmedTop(20).removeFromTop(20).removeFromLeft(160).withTrimmedLeft(80));
    
    Rectangle<int> area1 = getLocalBounds().reduced(40);
    Rectangle<int> areaSample = area1.removeFromBottom(area1.getHeight()/2);

    playButton.setBounds(areaSample.removeFromLeft(60).removeFromBottom(20));
    loadButton.setBounds(areaSample.removeFromLeft(60).removeFromBottom(20));
    
    areaSample.removeFromLeft(40);
    kickSampleGainSlider      .setBounds(areaSample.removeFromLeft(80));
}

//==============================================================================
void HitsEditor::updateToggleState(juce::Button *button)
{
    if (button == &playButton || button == &loadButton) {
        processor.getEditorInfo(playButton.getState(), loadButton.getState());
    }
    else {
        if (sampleHitOnOffButton.getToggleState() == 1) {
            processor.playSampleHit = true;
        }
        else {
            processor.playSampleHit = false;
        }
    }
}
