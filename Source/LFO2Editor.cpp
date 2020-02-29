/*
  ==============================================================================

    LFO2Editor.cpp
    Created: 2 Dec 2018 8:59:07pm
    Author:  Stijn de Koning

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "LFO2Editor.h"

//==============================================================================
LFO2Editor::LFO2Editor(TransitionFxAudioProcessor& p, string (&menuIDs)[2], string (&fqSliderIDs)[2]) : processor(p)
{
    // Name
    componentName = menuIDs[0];
    componentName.replace(componentName.begin()+4, componentName.end()-4, " ");
    
    // Combobox
    lfo2ComponentChoiceMenu.addItem("Gain", 1);
    lfo2ComponentChoiceMenu.addItem("Detune", 2);
    addAndMakeVisible(&lfo2ComponentChoiceMenu);
    lfo2ComponentChoiceMenu.setVisible(false);
    lfo2ComponentChoiceMenu.addListener(this);
    lfo2ComponentChoiceMenu.setSelectedId(1);
    
    for (int i = 0; i < amountOfLFOs; i++) {
        // Combobox
        lfo2Menu[i].addItem("Off", 1);
        lfo2Menu[i].addItem("On Sine", 2);
        lfo2Menu[i].addItem("On Saw", 3);
        lfo2Menu[i].addItem("On Square", 4);
        lfo2Menu[i].addItem("On Triangle", 5);
        
        lfo2Menu[i].setJustificationType(Justification::centred);
        addAndMakeVisible(&lfo2Menu[i]);
        lfo2MenuTree[i] = new AudioProcessorValueTreeState::ComboBoxAttachment (processor.tree, menuIDs[i], lfo2Menu[i]);
        
        // Sliders
        lfo2FreqSlider[i].setSliderStyle(Slider::SliderStyle::LinearVertical);
        lfo2FreqSlider[i].setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
        addAndMakeVisible(&lfo2FreqSlider[i]);
        lfo2FreqSliderTree[i] = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, fqSliderIDs[i], lfo2FreqSlider[i]);
    }
    
    // Initialise
    selectGUI();
}

LFO2Editor::~LFO2Editor()
{
    for (int i = 0; i < amountOfLFOs; i++){
        lfo2FreqSliderTree[i].~ScopedPointer();
        lfo2MenuTree[i].~ScopedPointer();
    }
}

void LFO2Editor::selectGUI()
{
    if (lfo2ComponentChoiceMenu.getSelectedId() == 1){
        lfo2Menu[0].setVisible(true);
        lfo2FreqSlider[0].setVisible(true);
        lfo2Menu[1].setVisible(false);
        lfo2FreqSlider[1].setVisible(false);
    }
    else {
        lfo2Menu[0].setVisible(false);
        lfo2FreqSlider[0].setVisible(false);
        lfo2Menu[1].setVisible(true);
        lfo2FreqSlider[1].setVisible(true);
    }
}

void LFO2Editor::paint (Graphics& g)
{
    g.fillAll (Colour(processor.red, processor.green, processor.blue));   
}

void LFO2Editor::resized()
{
    lfo2ComponentChoiceMenu.setBounds(getLocalBounds().removeFromTop(20).withTrimmedLeft(120));
    
    Rectangle<int> area = getLocalBounds().reduced(40).withTrimmedLeft(120);
    lfo2Menu[0].setBounds(area.removeFromTop(20).withTrimmedRight(300));
    lfo2FreqSlider[0].setBounds(area.removeFromLeft(80));
    
    Rectangle<int> area2 = getLocalBounds().reduced(40).withTrimmedLeft(120);
    lfo2Menu[1].setBounds(area2.removeFromTop(20).withTrimmedRight(300));
    lfo2FreqSlider[1].setBounds(area2.removeFromLeft(80));
}

//==============================================================================
void LFO2Editor::comboBoxChanged(juce::ComboBox *comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == &lfo2ComponentChoiceMenu) {
        selectGUI();
    }
}
