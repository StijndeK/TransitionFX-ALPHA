/*
  ==============================================================================

    MasterScreenEditor.cpp
    Created: 26 Nov 2018 3:35:12pm
    Author:  Stijn de Koning

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "MasterScreenEditor.h"


//==============================================================================
MasterScreenEditor::MasterScreenEditor(TransitionFxAudioProcessor& p) : processor(p), envelope(p)
{
    addAndMakeVisible(&envelope);



}

MasterScreenEditor::~MasterScreenEditor()
{
}

void MasterScreenEditor::updateToggleState (Button* button)
{

    
}

void MasterScreenEditor::paint (Graphics& g)
{
}

void MasterScreenEditor::resized()
{
    Rectangle<int> area = getLocalBounds();
    envelope.setBounds(area.withTrimmedTop(50).withTrimmedBottom(50));

}
