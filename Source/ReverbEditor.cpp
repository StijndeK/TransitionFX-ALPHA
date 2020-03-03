/*
  ==============================================================================

    ReverbEditor.cpp
    Created: 15 Nov 2018 3:12:35pm
    Author:  Stijn de Koning

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "ReverbEditor.h"

//==============================================================================
ReverbEditor::ReverbEditor(TransitionFxAudioProcessor& p) : processor(p)
{
    // Buttons
    addAndMakeVisible(reverbOnOffButton);
    reverbOnOffButtonTree = new AudioProcessorValueTreeState::ButtonAttachment (processor.tree, "reverbOnOffButtonID", reverbOnOffButton);
    
    // Sliders
    string sliderIDs[5];
    
    for (int i = 0; i < amountOfSliders; i++){
        sliderIDs[i] = processor.reverbSliderIDs[i];
        reverbSliders[i].setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
        reverbSliders[i].setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
        addAndMakeVisible(&reverbSliders[i]);
        reverbSlidersTree[i] = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, sliderIDs[i], reverbSliders[i]);
    }
}

ReverbEditor::~ReverbEditor()
{
    reverbOnOffButtonTree.~ScopedPointer();
    for (int i = 0; i < amountOfSliders; i++){
        reverbSlidersTree[i].~ScopedPointer();
    }
}

void ReverbEditor::paint (Graphics& g)
{
    g.fillAll (Colours::black);   
    
    Rectangle<int> area = getLocalBounds().reduced(40);
    
    g.setColour (Colours::grey);
    Rectangle<int> areaRR = area.withTrimmedTop(20);
    g.drawRoundedRectangle(areaRR.getX(), areaRR.getY(), areaRR.getWidth(), areaRR.getHeight(), 8, 2.0);
    g.setColour(Colour(processor.red, processor.green, processor.blue));
    g.fillRoundedRectangle(areaRR.getX(), areaRR.getY(), areaRR.getWidth(), areaRR.getHeight(), 8);
    
    // draw slider text
    g.setColour (Colours::white);
    Rectangle<int> area2 = getLocalBounds().reduced(60).removeFromBottom(40);
    string sliderNames[5] = {"roomsize", "Wet", "Dry", "Damping", "Width"};
    for (int i = 0; i < amountOfSliders; i++){
        g.drawText (sliderNames[i], area2.removeFromLeft(80), Justification::centred);
    }
}

void ReverbEditor::resized()
{
    Rectangle<int> area2 = getLocalBounds().reduced(60);
    reverbOnOffButton.setBounds(area2.removeFromTop(60).removeFromRight(80));

    Rectangle<int> area = getLocalBounds().reduced(60).withTrimmedBottom(40);
    for (int i = 0; i < amountOfSliders; i++){
        reverbSliders[i].setBounds(area.removeFromLeft(80));
    }
}
