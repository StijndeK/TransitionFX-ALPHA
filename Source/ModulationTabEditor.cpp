/*
  ==============================================================================

    ModulationTabEditor.cpp
    Created: 6 May 2019 9:45:07pm
    Author:  Stijn de Koning

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "ModulationTabEditor.h"

//==============================================================================
ModulationTabEditor::ModulationTabEditor(TransitionFxAudioProcessor& p) : processor(p)
{
    addAndMakeVisible(&panModButton);
    addAndMakeVisible(&gainModButton);
    addAndMakeVisible(&pitchModButton);
    addAndMakeVisible(&multiModButton);
    
    panModButton.setColour( TextButton::ColourIds::buttonColourId, Colour(0,20,100));
    gainModButton.setColour( TextButton::ColourIds::buttonColourId, Colour(0,20,180));
    pitchModButton.setColour( TextButton::ColourIds::buttonColourId, Colour(0,80,180));
    multiModButton.setColour( TextButton::ColourIds::buttonColourId, Colours::darkblue);
    
    // combine with value tree
    string aspectOnOffMasterNames[4];
    for (int i = 0; i < 4; i++) {
        addAndMakeVisible(&onOffButtons[i]);
    }

}

ModulationTabEditor::~ModulationTabEditor()
{
}

void ModulationTabEditor::updateToggleState (Button* button)
{

}


void ModulationTabEditor::paint (Graphics& g)
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

void ModulationTabEditor::resized()
{
    Rectangle<int> area = getLocalBounds().reduced(10).withTrimmedTop(getLocalBounds().reduced(10).getHeight()/5);
    Rectangle<int> topArea = area.removeFromTop(area.getHeight()/2);
    Rectangle<int> bottomArea = area;
    
    panModButton.setBounds(topArea.removeFromLeft(area.getWidth()/4));
    onOffButtons[0].setBounds(panModButton.getBounds().removeFromRight(40).removeFromTop(panModButton.getBounds().getHeight()/2));
    gainModButton.setBounds(topArea.removeFromLeft(area.getWidth()/4));
    onOffButtons[1].setBounds(gainModButton.getBounds().removeFromRight(40).removeFromTop(panModButton.getBounds().getHeight()/2));
    pitchModButton.setBounds(topArea.removeFromLeft(area.getWidth()/4));
    onOffButtons[2].setBounds(pitchModButton.getBounds().removeFromRight(40).removeFromTop(panModButton.getBounds().getHeight()/2));
    multiModButton.setBounds(topArea.removeFromLeft(area.getWidth()/4));
    onOffButtons[3].setBounds(multiModButton.getBounds().removeFromRight(40).removeFromTop(panModButton.getBounds().getHeight()/2));
}
