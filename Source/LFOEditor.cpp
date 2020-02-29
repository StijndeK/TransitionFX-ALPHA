/*
  ==============================================================================

    LFOEditor.cpp
    Created: 1 Nov 2018 2:52:20pm
    Author:  Stijn de Koning

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "LFOEditor.h"

//==============================================================================
LFOEditor::LFOEditor(TransitionFxAudioProcessor& p, string menuID, string freqSliderID) : processor(p)
{
    // Name
    componentName = menuID;
    componentName.replace(componentName.begin()+4, componentName.end()-4, " ");
    
    // Combobox
    lfo1Menu.addItem("Off", 1);
    lfo1Menu.addItem("On Sine", 2);
    lfo1Menu.addItem("On Saw", 3);
    lfo1Menu.addItem("On Square", 4);
    lfo1Menu.addItem("On Triangle", 5);

    lfo1Menu.setJustificationType(Justification::centred);
    addAndMakeVisible(&lfo1Menu);
    lfo1MenuTree = new AudioProcessorValueTreeState::ComboBoxAttachment (processor.tree, menuID, lfo1Menu);

    // Sliders
    lfo1FreqSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    lfo1FreqSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(&lfo1FreqSlider);
    lfo1FreqSliderTree = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, freqSliderID, lfo1FreqSlider);
}

LFOEditor::~LFOEditor()
{
    lfo1FreqSliderTree.~ScopedPointer();
    lfo1MenuTree.~ScopedPointer();
}

void LFOEditor::paint (Graphics& g)
{
    g.fillAll (Colour(processor.red, processor.green, processor.blue));
}

void LFOEditor::resized()
{
    Rectangle<int> area = getLocalBounds().reduced(40).withTrimmedLeft(120);
    lfo1Menu.setBounds(area.removeFromTop(20).withTrimmedRight(300));
    lfo1FreqSlider.setBounds(area.removeFromLeft(80));
}
