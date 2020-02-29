/*
  ==============================================================================

    SoundTabEditor.cpp
    Created: 1 May 2019 11:14:33pm
    Author:  Stijn de Koning

  ==============================================================================
*/

#include "SoundTabEditor.h"

#include "../JuceLibraryCode/JuceHeader.h"


//==============================================================================
SoundTabEditor::SoundTabEditor(TransitionFxAudioProcessor& p) : processor(p)
{
    // menu
    for (int i = 0; i < 3; i++) {
        
        // TODO: SET IDS
        addAndMakeVisible(&voiceButtons[i]);
        
        addAndMakeVisible(&oscSelectionButtons[i]);
        
        addAndMakeVisible(&multiSelectionButtons[i]);
        
        voiceButtons[i].setColour( TextButton::ColourIds::buttonColourId, Colours::grey);
        
        oscSelectionButtons[i].setButtonText("Oscs");
        multiSelectionButtons[i].setButtonText("Multiply Oscs");
    }
    
    for (int i = 0; i < 4; i++) {
        modulationButtons[i].setButtonText("Modulation");
        addAndMakeVisible(&modulationButtons[i]);
    }

    voiceButtons[0].setButtonText("Voice 1");
    voiceButtons[1].setButtonText("Voice 2");
    voiceButtons[2].setButtonText("Voice 3");

    voiceButtons       [0].onClick = [this] { updateToggleState (&voiceButtons        [0]);   };
    voiceButtons       [1].onClick = [this] { updateToggleState (&voiceButtons        [1]);   };
    voiceButtons       [2].onClick = [this] { updateToggleState (&voiceButtons        [2]);   };
    
    
    addAndMakeVisible(&masterButton);
    masterButton.setColour( TextButton::ColourIds::buttonColourId, Colours::blue);
    
    addAndMakeVisible(&impactButton);
    
    addAndMakeVisible(&effectsButton);
    
    addAndMakeVisible(&envelopeButton);
    envelopeButton.setColour( TextButton::ColourIds::buttonColourId, Colour(26, 36, 40));
    
    addAndMakeVisible(&mixerButton);
    mixerButton.setColour( TextButton::ColourIds::buttonColourId, Colour(26, 36, 40));

    addAndMakeVisible(&voicesSlider);
    voicesSliderTree = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "voicesSliderID", voicesSlider);
    voicesSlider.setValue(1);
    
    addAndMakeVisible(&voiceAmountSlider);
    voiceAmountSliderTree = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "voiceAmountSliderID", voiceAmountSlider);
    
    // gain and pan sliders
    masterGainSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    masterGainSlider.setTextBoxStyle(Slider::NoTextBox, false, 60, 20);
    addAndMakeVisible(&masterGainSlider);
    masterGainSliderTree = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "masterGainSliderID", masterGainSlider);
    
    masterPanSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    masterPanSlider.setTextBoxStyle(Slider::NoTextBox, false, 60, 20);
    addAndMakeVisible(&masterPanSlider);
    masterPanSliderTree = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "masterPanSliderID", masterPanSlider);
    
    // get IDS
    string voicePanSliderIDS [processor.amountOfSubvoices];
    string voiceMixSliderIDS [processor.amountOfSubvoices];


    for (int i = 0; i < processor.amountOfSubvoices; i++) {
        // SLIDERS
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
    }
    
    // initialise
    voiceOnOff[0] = true;
    voiceButtons[0].setColour( TextButton::ColourIds::buttonColourId, Colours::blue);

}

SoundTabEditor::~SoundTabEditor()
{
}

void SoundTabEditor::updateToggleState (Button* button)
{
    for (int i = 0; i < 3; i++) {
        if (button == &voiceButtons[i]) {
            if (voiceOnOff[i] == true) {
                voiceButtons[i].setColour( TextButton::ColourIds::buttonColourId, Colours::grey);
                voiceOnOff[i] = false;
            }
            else {
                voiceButtons[i].setColour( TextButton::ColourIds::buttonColourId, Colours::blue);
                voiceOnOff[i] = true;
            }
            setVoices();
        }
    }
}

void SoundTabEditor::setVoices ()
{
    if (voiceOnOff[0] == true && voiceOnOff[1] == true && voiceOnOff[2] == true){
        voiceAmountSlider.setValue(3);
    }
    else if (voiceOnOff[0] == true && voiceOnOff[1] == true) {
        voiceAmountSlider.setValue(2);
    }
    else {
        voiceAmountSlider.setValue(1);
    }
}

void SoundTabEditor::paint (Graphics& g)
{

}

void SoundTabEditor::resized()
{
    Rectangle<int> area = getLocalBounds().reduced(10).withTrimmedTop(getLocalBounds().reduced(10).getHeight()/5);
    Rectangle<int> sliderArea = getLocalBounds().reduced(10).withTrimmedTop(getLocalBounds().reduced(10).getHeight()/5).removeFromLeft(40);
    Rectangle<int> menuArea = getLocalBounds().reduced(10).removeFromTop(getLocalBounds().reduced(10).getHeight()/5);
    Rectangle<int> voiceArea = area.removeFromLeft(area.getWidth()/3);
    
    Rectangle<int> hitsArea = area.withTrimmedRight(area.getWidth()/3*2);
    Rectangle<int> effectsArea = area.reduced(area.getWidth()/3, 0);
    Rectangle<int> modulationArea = area.withTrimmedLeft(area.getWidth()/3*2);

    area = area.withTrimmedTop(getLocalBounds().reduced(10).getHeight() / 5);
    Rectangle<int> oscArea = area.withTrimmedRight(area.getWidth()/4*3);
    Rectangle<int> multiArea = area.withTrimmedRight(area.getWidth()/2).withTrimmedLeft(area.getWidth()/4);
    Rectangle<int> voiceModulationArea = area.withTrimmedLeft(area.getWidth()/2);

    // master row bounds
    envelopeButton.setBounds(menuArea.removeFromLeft(menuArea.getWidth()/2).reduced(5, 5));
    mixerButton.setBounds(menuArea.reduced(5,5));
    
    masterButton.setBounds(voiceArea.removeFromTop(getLocalBounds().reduced(10).getHeight() / 5).reduced(0, 5));
    impactButton.setBounds(hitsArea.removeFromTop(getLocalBounds().reduced(10).getHeight() / 5).reduced(0, 5));
    effectsButton.setBounds(effectsArea.removeFromTop(getLocalBounds().reduced(10).getHeight() / 5).reduced(0, 5));
    modulationButtons[0].setBounds(modulationArea.removeFromTop(getLocalBounds().reduced(10).getHeight() / 5).reduced(0, 5));
    masterGainSlider.setBounds(sliderArea.removeFromTop(getLocalBounds().reduced(10).getHeight() / 5).reduced(0, 5));    
    oscSelectionButtons[0].setBounds(area);

    // voice row bounds
    for (int i = 0; i < 3; i++) {
        voiceButtons[i].setBounds(voiceArea.removeFromTop(getLocalBounds().reduced(10).getHeight() / 5).reduced(0, 5));
        oscSelectionButtons[i].setBounds(oscArea.removeFromTop(getLocalBounds().reduced(10).getHeight() / 5).reduced(0, 5));
        multiSelectionButtons[i].setBounds(multiArea.removeFromTop(getLocalBounds().reduced(10).getHeight() / 5).reduced(0, 5));
        masterVoiceMixSliders[i].setBounds(sliderArea.removeFromTop(getLocalBounds().reduced(10).getHeight() / 5).reduced(0, 5));
        modulationButtons[i+1].setBounds(voiceModulationArea.removeFromTop(getLocalBounds().reduced(10).getHeight() / 5).reduced(0, 5));
    }
}
