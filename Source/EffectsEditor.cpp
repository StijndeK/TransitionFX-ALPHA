/*
  ==============================================================================

    EffectsEditor.cpp
    Created: 14 Nov 2018 12:46:25pm
    Author:  Stijn de Koning

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "EffectsEditor.h"

//==============================================================================
EffectsEditor::EffectsEditor(TransitionFxAudioProcessor& p) : processor(p), filter(p), distortion(p), chorus(p), reverb(p)
{
    addAndMakeVisible(&filter);
    addAndMakeVisible(&distortion);
    addAndMakeVisible(&chorus);
    addAndMakeVisible(&reverb);

    // Combobox
    effectsMenu.addItem("Filter", 1);
    effectsMenu.addItem("Distortion", 2);
    effectsMenu.addItem("Chorus", 3);
    effectsMenu.addItem("Reverb", 4);
    effectsMenu.setSelectedId(1);
    effectsMenu.setJustificationType(Justification::centred);
    addAndMakeVisible(&effectsMenu);
    effectsMenu.addListener(this);
    effectsMenuTree = new AudioProcessorValueTreeState::ComboBoxAttachment (processor.tree, "effectsMenuID", effectsMenu);
}

EffectsEditor::~EffectsEditor()
{
    effectsMenuTree.~ScopedPointer();
}

void EffectsEditor::paint (Graphics& g)
{
    g.fillAll (Colours::black);
    
    Rectangle<int> area = getLocalBounds().reduced(40);
    
    g.setColour (Colours::white);
    g.setFont (14.0f);
    g.drawText ("Effects", area, Justification::centredTop);
}

void EffectsEditor::resized()
{
    Rectangle<int> area = getLocalBounds().reduced(40);
    effectsMenu.setBounds(area.withTrimmedTop(30).removeFromTop(20).removeFromLeft(area.getWidth()/2 + 120).withTrimmedLeft(area.getWidth()/2 - 120));
    
    filter.setBounds(getLocalBounds());
    distortion.setBounds(getLocalBounds());
    chorus.setBounds(getLocalBounds());
    reverb.setBounds(getLocalBounds());
}

//==============================================================================
void EffectsEditor::comboBoxChanged(ComboBox *comboBoxThatHasChanged){
    if (comboBoxThatHasChanged == &effectsMenu){
        if (effectsMenu.getSelectedId() == 1){
            filter.setVisible(true);
            distortion.setVisible(false);
            chorus.setVisible(false);
            reverb.setVisible(false);
        }
        else if (effectsMenu.getSelectedId() == 2){
            filter.setVisible(false);
            distortion.setVisible(true);
            chorus.setVisible(false);
            reverb.setVisible(false);
        }
        else if (effectsMenu.getSelectedId() == 3){
            filter.setVisible(false);
            distortion.setVisible(false);
            chorus.setVisible(true);
            reverb.setVisible(false);
        }
        else{
            filter.setVisible(false);
            distortion.setVisible(false);
            chorus.setVisible(false);
            reverb.setVisible(true);
        }
    }
}
