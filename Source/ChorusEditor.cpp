/*
  ==============================================================================

    ChorusEditor.cpp
    Created: 15 Nov 2018 11:48:39am
    Author:  Stijn de Koning

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "ChorusEditor.h"

//==============================================================================
ChorusEditor::ChorusEditor(TransitionFxAudioProcessor& p) : processor(p)
{
    // Button
    addAndMakeVisible(chorusOnOffButton);
    chorusOnOffButtonTree = new AudioProcessorValueTreeState::ButtonAttachment (processor.tree, "chorusOnOffButtonID", chorusOnOffButton);
}

ChorusEditor::~ChorusEditor()
{
    chorusOnOffButtonTree.~ScopedPointer();
}

void ChorusEditor::paint (Graphics& g)
{
    g.fillAll (Colours::black);
    
    g.setColour (Colours::white);
    g.drawRect (getLocalBounds(), 1);
}

void ChorusEditor::resized()
{
    Rectangle<int> area2 = getLocalBounds().reduced(40);
    chorusOnOffButton.setBounds(area2.removeFromTop(20).removeFromRight(80));
}
