/*
  ==============================================================================

    MasterEditor.cpp
    Created: 31 Oct 2018 5:10:22pm
    Author:  Stijn de Koning

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "MasterEditor.h"

//==============================================================================
MasterEditor::MasterEditor(TransitionFxAudioProcessor& p) : processor(p)
{
    // get IDS
    string voicePanSliderIDS [processor.amountOfSubvoices];
    string voiceMixSliderIDS [processor.amountOfSubvoices];
    string voiceMuteButtonIDS[processor.amountOfSubvoices];
    string voiceSoloButtonIDS[processor.amountOfSubvoices];
    
    // Sliders
    masterGainSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    masterGainSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(&masterGainSlider);
    masterGainSliderTree = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "masterGainSliderID", masterGainSlider);
    
    masterPanSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    masterPanSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(&masterPanSlider);
    masterPanSliderTree = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "masterPanSliderID", masterPanSlider);
    
    for (int i = 0; i < processor.amountOfSubvoices; i++) {
        // Sliders
        voiceMixSliderIDS[i] = processor.voiceMixSliderIDsP[i];
        voicePanSliderIDS[i] = processor.voicePanSliderIDsP[i];
        
        masterVoiceMixSliders[i].setSliderStyle(Slider::SliderStyle::LinearVertical);
        masterVoiceMixSliders[i].setTextBoxStyle(Slider::NoTextBox, false, 60, 20);
        addAndMakeVisible(&masterVoiceMixSliders[i]);
        masterVoiceMixSlidersTree[i] = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, voiceMixSliderIDS[i] , masterVoiceMixSliders[i]);
        
        masterVoicePanSliders[i].setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
        masterVoicePanSliders[i].setTextBoxStyle(Slider::NoTextBox, false, 60, 20);
        addAndMakeVisible(&masterVoicePanSliders[i]);
        masterVoicePanSlidersTree[i] = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, voicePanSliderIDS[i] , masterVoicePanSliders[i]);

        // Buttons
        voiceMuteButtonIDS[i] = processor.voiceMuteButtonIDsP[i];
        voiceSoloButtonIDS[i] = processor.voiceSoloButtonIDsP[i];
        
        voiceMuteButtons[i].setButtonText("M");
        addAndMakeVisible(&voiceMuteButtons[i]);
        voiceMuteBottunsTree[i] = new AudioProcessorValueTreeState::ButtonAttachment (processor.tree, voiceMuteButtonIDS[i], voiceMuteButtons[i]);
        voiceSoloButtons[i].setButtonText("S");
        addAndMakeVisible(&voiceSoloButtons[i]);
        voiceSoloBottunsTree[i] = new AudioProcessorValueTreeState::ButtonAttachment (processor.tree, voiceSoloButtonIDS[i], voiceSoloButtons[i]);
    }

    // Buttons
    addAndMakeVisible(voicePitchTonalButton);
    voicePitchTonalButtonTree = new AudioProcessorValueTreeState::ButtonAttachment (processor.tree, "voicePitchTonalButtonID", voicePitchTonalButton);
}

MasterEditor::~MasterEditor()
{
    masterGainSliderTree.~ScopedPointer();
    masterPanSliderTree.~ScopedPointer();
    voicePitchTonalButtonTree.~ScopedPointer();
    for (int i = 0; i < 3; i ++){
        masterVoiceMixSlidersTree[i].~ScopedPointer();
        masterVoicePanSlidersTree[i].~ScopedPointer();
        voiceMuteBottunsTree[i].~ScopedPointer();
        voiceSoloBottunsTree[i].~ScopedPointer();
    }
}

void MasterEditor::paint (Graphics& g)
{
    g.fillAll (Colours::black);

    Rectangle<int> area = getLocalBounds().reduced(10);

    for (int i = 0; i < 3; i ++) {
        voiceSoloButtons[i].setColour(ToggleButton::tickColourId, Colour(230, 230, 0));
        voiceMuteButtons[i].setColour(ToggleButton::tickColourId, Colour(0, 230, 230));
    }
    
    g.setColour (Colours::grey);
    Rectangle<int> areaRR = area;
    g.drawRoundedRectangle(areaRR.getX(), areaRR.getY(), areaRR.getWidth(), areaRR.getHeight(), 8, 2.0);
    g.setColour(Colour(processor.red, processor.green, processor.blue));
    g.fillRoundedRectangle(areaRR.getX(), areaRR.getY(), areaRR.getWidth(), areaRR.getHeight(), 8);

    g.setColour (Colours::white);
    g.setFont (14.0f);
    g.drawText ("Master", area.removeFromLeft(160).withTrimmedTop(10).withTrimmedLeft(40), Justification::centredTop);
    
    Rectangle<int> area2 = getLocalBounds().reduced(20);
    Rectangle<int> area5 = area2.removeFromTop(40);

    g.drawText ("Voice 3", area5.removeFromRight(120), Justification::centred);
    g.drawText ("Voice 2", area5.removeFromRight(120), Justification::centred);
    g.drawText ("Voice 1", area5.removeFromRight(120), Justification::centred);
    
    
    g.setColour (Colours::lightgrey);
    Rectangle<int> areaSliderTexts = getLocalBounds().reduced(20).removeFromBottom(15);
    g.drawText ("Gain", areaSliderTexts.removeFromLeft(80), Justification::centredBottom);
    g.drawText ("Pan", areaSliderTexts.removeFromLeft(80), Justification::centredBottom);
    areaSliderTexts.removeFromLeft(40);
    g.drawText ("Gain", areaSliderTexts.removeFromLeft(40), Justification::centredBottom);
    areaSliderTexts.removeFromLeft(20);
    g.drawText ("Pan", areaSliderTexts.removeFromLeft(40), Justification::centredBottom);
    areaSliderTexts.removeFromLeft(20);
    g.drawText ("Gain", areaSliderTexts.removeFromLeft(40), Justification::centredBottom);
    areaSliderTexts.removeFromLeft(20);
    g.drawText ("Pan", areaSliderTexts.removeFromLeft(40), Justification::centredBottom);
    areaSliderTexts.removeFromLeft(20);
    g.drawText ("Gain", areaSliderTexts.removeFromLeft(40), Justification::centredBottom);
    areaSliderTexts.removeFromLeft(20);
    g.drawText ("Pan", areaSliderTexts.removeFromLeft(40), Justification::centredBottom);
}


void MasterEditor::resized()
{
    // master
    Rectangle<int> area = getLocalBounds().reduced(20).withTrimmedBottom(15);
    
    masterGainSlider.setBounds(area.removeFromLeft(80));
    masterPanSlider.setBounds(area.removeFromLeft(80));
    area.removeFromLeft(40);
    
    // voices
    Rectangle<int> area5 = area.withTrimmedTop(20);
    Rectangle<int> area2 = area5;
    Rectangle<int> area3 = area5.removeFromTop(40).withTrimmedTop(20);
    Rectangle<int> area4 = area5.removeFromTop(80);

    for (int i = 0; i < 3; i ++){
        masterVoiceMixSliders[i].setBounds(area2.removeFromLeft(40));
        area3.removeFromLeft(40);
        area4.removeFromLeft(40);
        area2.removeFromLeft(80);
        masterVoicePanSliders[i].setBounds(area4.removeFromLeft(80));
        voiceMuteButtons[i].setBounds(area3.removeFromLeft(40));
        voiceSoloButtons[i].setBounds(area3.removeFromLeft(40));
    }
}
