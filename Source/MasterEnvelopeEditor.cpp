/*
  ==============================================================================

    MasterEnvelopeEditor.cpp
    Created: 18 Nov 2018 10:45:16am
    Author:  Stijn de Koning

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "MasterEnvelopeEditor.h"

//==============================================================================
MasterEnvelopeEditor::MasterEnvelopeEditor(TransitionFxAudioProcessor& p) : processor(p)
{
    // sliders
    masterAttackSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    masterAttackSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(&masterAttackSlider);
    masterAttackSliderTree = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "masterAttackSliderID", masterAttackSlider);
    
    masterReleaseSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    masterReleaseSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(&masterReleaseSlider);
    masterReleaseSliderTree = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "masterReleaseSliderID", masterReleaseSlider);
    
    masterAttackBeatsNotesSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    masterAttackBeatsNotesSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(&masterAttackBeatsNotesSlider);
    masterAttackBeatsNotesSliderTree = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "masterAttackBeatsNotesSliderID", masterAttackBeatsNotesSlider);
    
    masterReleaseBeatsNotesSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    masterReleaseBeatsNotesSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(&masterReleaseBeatsNotesSlider);
    masterReleaseBeatsNotesSliderTree = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "masterReleaseBeatsNotesSliderID", masterReleaseBeatsNotesSlider);
    
    masterAttackSamplesSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    masterAttackSamplesSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(&masterAttackSamplesSlider);
    masterAttackSamplesSliderTree = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "masterAttackSamplesSliderID", masterAttackSamplesSlider);
    
    masterReleaseSamplesSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    masterReleaseSamplesSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(&masterReleaseSamplesSlider);
    masterReleaseSamplesSliderTree = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "masterReleaseSamplesSliderID", masterReleaseSamplesSlider);
    
    masterAttackFramesSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    masterAttackFramesSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(&masterAttackFramesSlider);
    masterAttackFramesSliderTree = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "masterAttackFramesSliderID", masterAttackFramesSlider);
    
    masterReleaseFramesSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    masterReleaseFramesSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(&masterReleaseFramesSlider);
    masterReleaseFramesSliderTree = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "masterReleaseFramesSliderID", masterReleaseFramesSlider);
    
    // beat linking
    addAndMakeVisible(linkOnOffButton);
    linkOnOffButtonTree = new AudioProcessorValueTreeState::ButtonAttachment (processor.tree, "linkOnOffButtonID", linkOnOffButton);
    addAndMakeVisible(linkTime);
    linkTime.setInputRestrictions(2, "0123456789");
    linkTimeSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    linkTimeSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(&linkTimeSlider);
    linkTimeSliderTree = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "LinkTimeSliderID", linkTimeSlider);
    linkTimeSlider.setVisible(false);
    addAndMakeVisible(linkTimeButton);
    linkTimeButton.onClick = [this] { updateToggleState (&linkTimeButton);   };
    
    // combobox
    timeLinkMenu.addItem("time in MS", 1);
    timeLinkMenu.addItem("half beats", 2);
    timeLinkMenu.addItem("beats", 3);
    timeLinkMenu.addItem("measures", 4);
    timeLinkMenu.addItem("samples", 5);
    timeLinkMenu.addItem("frames", 6);
    timeLinkMenu.setSelectedId(1);
    timeLinkMenu.setJustificationType(Justification::centred);
    timeLinkMenu.addListener(this);
    timeLinkMenuTree = new AudioProcessorValueTreeState::ComboBoxAttachment (processor.tree, "timeLinkMenuID", timeLinkMenu);
    addAndMakeVisible(&timeLinkMenu);
}

MasterEnvelopeEditor::~MasterEnvelopeEditor()
{
    masterAttackSliderTree.~ScopedPointer();
    masterReleaseSliderTree.~ScopedPointer();
    masterAttackBeatsNotesSliderTree.~ScopedPointer();
    masterReleaseBeatsNotesSliderTree.~ScopedPointer();
    masterAttackSamplesSliderTree.~ScopedPointer();
    masterReleaseSamplesSliderTree.~ScopedPointer();
    masterAttackFramesSliderTree.~ScopedPointer();
    masterReleaseFramesSliderTree.~ScopedPointer();
    timeLinkMenuTree.~ScopedPointer();
}

void MasterEnvelopeEditor::updateToggleState (Button* button)
{
    if (button == &linkTimeButton) {
        linkTimeSlider.setValue(linkTime.getText().getIntValue());
    }
    
}

void MasterEnvelopeEditor::paint (Graphics& g)
{
    g.fillAll (Colour(36, 36, 40));
    
    getLookAndFeel().setColour(Slider::thumbColourId, Colours::deepskyblue);
    
    Rectangle<int> area = getLocalBounds().reduced(10);
    
    g.setColour (Colours::grey);
//    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
    g.drawRoundedRectangle(getLocalBounds().getX(), getLocalBounds().getY(), getLocalBounds().getWidth(), getLocalBounds().getHeight(), 8, 2.0);

    
    g.setColour (Colours::white);
    g.setFont (18.0f);
    g.drawText ("Length", area.removeFromLeft(400), Justification::centredTop);
    g.drawText ("Beat Linking", area, Justification::centredTop);
    
}

void MasterEnvelopeEditor::resized()
{
    // length sliders
    Rectangle<int> area = getLocalBounds().reduced(20).withTrimmedTop(20);
    Rectangle<int> area2 = area.removeFromLeft(200);
    masterAttackSlider.setBounds(area2);
    masterAttackBeatsNotesSlider.setBounds(area2);
    masterAttackSamplesSlider.setBounds(area2);
    masterAttackFramesSlider.setBounds(area2);
    Rectangle<int> area3 = area.removeFromLeft(200);
    masterReleaseSlider.setBounds(area3);
    masterReleaseBeatsNotesSlider.setBounds(area3);
    masterReleaseSamplesSlider.setBounds(area3);
    masterReleaseFramesSlider.setBounds(area3);
    
    // time link menu
    Rectangle<int> area4 = getLocalBounds().reduced(20).withTrimmedTop(20);
    timeLinkMenu.setBounds(area4.removeFromTop(20).removeFromLeft(400));
    
    // beat linking
    Rectangle<int> area5 = area.withTrimmedLeft(20);
    linkOnOffButton.setBounds(area5.removeFromTop(area5.getHeight()/3));
    area5 = area5.withTrimmedBottom(area5.getHeight()/2);

    linkTime.setBounds(area5.removeFromLeft(area5.getWidth()/2));
    linkTimeButton.setBounds(area5);
}

//==============================================================================
void MasterEnvelopeEditor::comboBoxChanged(juce::ComboBox *comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == &timeLinkMenu){
        masterAttackSlider.setVisible           (false);
        masterReleaseSlider.setVisible          (false);
        masterAttackBeatsNotesSlider.setVisible (false);
        masterReleaseBeatsNotesSlider.setVisible(false);
        masterAttackSamplesSlider.setVisible    (false);
        masterReleaseSamplesSlider.setVisible   (false);
        masterAttackFramesSlider.setVisible     (false);
        masterReleaseFramesSlider.setVisible    (false);
        if (timeLinkMenu.getSelectedId() == 1) {
            masterAttackSlider.setVisible(true);
            masterReleaseSlider.setVisible(true);
        }
        else if (timeLinkMenu.getSelectedId() == 5) {
            masterAttackSamplesSlider.setVisible(true);
            masterReleaseSamplesSlider.setVisible(true);
        }
        else if (timeLinkMenu.getSelectedId() == 6) {
            masterAttackFramesSlider.setVisible(true);
            masterReleaseFramesSlider.setVisible(true);
        }
        else {
            masterAttackBeatsNotesSlider.setVisible(true);
            masterReleaseBeatsNotesSlider.setVisible(true);
        }
    }
}







