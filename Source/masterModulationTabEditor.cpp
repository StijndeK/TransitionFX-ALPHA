/*
  ==============================================================================

    masterModulationTabEditor.cpp
    Created: 6 May 2019 10:14:19pm
    Author:  Stijn de Koning

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "masterModulationTabEditor.h"

//==============================================================================
masterModulationTabEditor::masterModulationTabEditor(TransitionFxAudioProcessor& p) : processor(p)
{
    addAndMakeVisible(&panModButton);
    addAndMakeVisible(&gainModButton);
    addAndMakeVisible(&pitchModButton);
    addAndMakeVisible(&filterModButton);
    addAndMakeVisible(&reverbModButton);
    addAndMakeVisible(&lfoModButton);

    panModButton.setColour( TextButton::ColourIds::buttonColourId, Colour(0,20,100));
    gainModButton.setColour( TextButton::ColourIds::buttonColourId, Colour(0,20,180));
    pitchModButton.setColour( TextButton::ColourIds::buttonColourId, Colour(0,80,180));
    filterModButton.setColour( TextButton::ColourIds::buttonColourId, Colour(80,80,180));
    reverbModButton.setColour( TextButton::ColourIds::buttonColourId, Colour(0,80,20));
    lfoModButton.setColour( TextButton::ColourIds::buttonColourId, Colour(0,80,80));

    string aspectOnOffMasterNames[6];
    for (int i = 0; i < 6; i++) {
        addAndMakeVisible(&onOffButtons[i]);
        aspectOnOffMasterNames[i] = processor.modulationMasterTriggersP[i];
        aspectOnOffButtonMasterTree[i] = new AudioProcessorValueTreeState::ButtonAttachment (processor.tree, aspectOnOffMasterNames[i], onOffButtons[i]);
    }
}

masterModulationTabEditor::~masterModulationTabEditor()
{
}

void masterModulationTabEditor::updateToggleState (Button* button)
{
}


void masterModulationTabEditor::paint (Graphics& g)
{
    // ENV DRAWING
    g.setColour (Colours::grey);
    Rectangle<int> area1 = getLocalBounds().reduced(10).withTrimmedTop(getLocalBounds().reduced(10).getHeight()/5);
    Rectangle<int> drawArea2 = area1.removeFromBottom(area1.getHeight()/2);
    g.drawRect(drawArea2,2);
    
    // TODO: actually calculate these values
    left = drawArea2.getX();
    right = drawArea2.getWidth();
    middlePoint = ((right - left) / 2) + left;
    startAttack = left;
    endAttack = middlePoint;
    endRelease = right;
    endPoint = 0;
    
    // impact line
    g.setColour (Colours::white);
    g.drawLine(middlePoint, drawArea2.getBottom(), middlePoint, drawArea2.getY());
    
    // env path
    Rectangle<int> drawArea = drawArea2.reduced(4);
    g.setColour (Colour(0,20,180));
    myPath.startNewSubPath(drawArea.getX(), drawArea.getBottom());
    // delay
    myPath.lineTo(startAttack, drawArea.getBottom());
    // attack
    myPath.lineTo(endAttack, drawArea.getY());
    // middle
    myPath.lineTo(middlePoint, drawArea.getY());
    // release
    if (endPoint == 1) { // sustain
        myPath.lineTo(drawArea.getWidth(), drawArea.getY());
    }                    // release
    else {
        myPath.lineTo(endRelease, drawArea.getBottom());
    }
    g.strokePath(myPath, PathStrokeType (2.0f));
}

void masterModulationTabEditor::resized()
{
    Rectangle<int> area = getLocalBounds().reduced(10).withTrimmedTop(getLocalBounds().reduced(10).getHeight()/5);
    Rectangle<int> topArea = area.removeFromTop(area.getHeight()/2);
    Rectangle<int> halfTopArea = topArea.removeFromTop(topArea.getHeight()/2);
    
    panModButton.setBounds(halfTopArea.removeFromLeft(area.getWidth()/3));
    onOffButtons[0].setBounds(panModButton.getBounds().removeFromRight(40).removeFromTop(panModButton.getBounds().getHeight()/2));
    gainModButton.setBounds(halfTopArea.removeFromLeft(area.getWidth()/3));
    onOffButtons[1].setBounds(gainModButton.getBounds().removeFromRight(40).removeFromTop(panModButton.getBounds().getHeight()/2));
    pitchModButton.setBounds(halfTopArea.removeFromLeft(area.getWidth()/3));
    onOffButtons[2].setBounds(pitchModButton.getBounds().removeFromRight(40).removeFromTop(panModButton.getBounds().getHeight()/2));
    filterModButton.setBounds(topArea.removeFromLeft(area.getWidth()/3));
    onOffButtons[3].setBounds(filterModButton.getBounds().removeFromRight(40).removeFromTop(panModButton.getBounds().getHeight()/2));
    reverbModButton.setBounds(topArea.removeFromLeft(area.getWidth()/3));
    onOffButtons[4].setBounds(reverbModButton.getBounds().removeFromRight(40).removeFromTop(panModButton.getBounds().getHeight()/2));
    lfoModButton.setBounds(topArea.removeFromLeft(area.getWidth()/3));
    onOffButtons[5].setBounds(lfoModButton.getBounds().removeFromRight(40).removeFromTop(panModButton.getBounds().getHeight()/2));

}
