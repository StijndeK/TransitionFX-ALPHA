
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TransitionFxAudioProcessorEditor::TransitionFxAudioProcessorEditor (TransitionFxAudioProcessor& p)
: AudioProcessorEditor (&p), processor (p),
masterenv(p),
soundTab(p)
{
    setSize (pluginWidth, 680);     // 640 bij 740

    addAndMakeVisible(&masterenv);
    addAndMakeVisible(&soundTab);
    
    addAndMakeVisible(triggerButton);
    triggerButton.onClick = [this] { updateToggleState (&triggerButton);   };
}


TransitionFxAudioProcessorEditor::~TransitionFxAudioProcessorEditor()
{
}

void TransitionFxAudioProcessorEditor::paint (Graphics& g)
{
    // set input elemenet colours
    getLookAndFeel().setColour(Slider::thumbColourId, Colours::aqua);

    getLookAndFeel().setColour(Slider::textBoxTextColourId, Colours::dimgrey);

    getLookAndFeel().setColour(TextButton::buttonColourId, Colour(25, 25, 30));

    getLookAndFeel().setColour(ComboBox::backgroundColourId, Colour(25, 25, 30));
    getLookAndFeel().setColour(ComboBox::buttonColourId, Colour(25, 25, 30));

    // set background color and text
    g.fillAll (Colours::black);
    Rectangle<int> area = getLocalBounds().removeFromLeft(pluginWidth).reduced(20).withTrimmedTop(10);

    g.setColour (Colours::white);
    g.setFont (26.0f);
    g.drawText ("TRANSITION FX", area, Justification::centredTop);
}

void TransitionFxAudioProcessorEditor::resized()
{
    // manual trigger
    Rectangle<int> triggerArea = getLocalBounds().removeFromLeft(pluginWidth).reduced(20).withTrimmedTop(20);
    triggerButton.setBounds(triggerArea.withTrimmedLeft(5).removeFromLeft(60).removeFromTop(20));
    
    // MAIN INTERFACE
    Rectangle<int> area = getLocalBounds().removeFromLeft(pluginWidth).reduced(15, 0).withTrimmedTop(40).withTrimmedBottom(15);
    Rectangle<int> area1 = area.withTrimmedTop(20);
    
    // sound tab
    Rectangle<int> area1c = area1.removeFromTop(440);
    soundTab.setBounds(area1c);
    
    // bottom envelope
    Rectangle<int> area2 = area.removeFromBottom(200).withTrimmedTop(40).reduced(8);
    masterenv.setBounds(area2);
}


//==============================================================================
void TransitionFxAudioProcessorEditor::updateToggleState (Button* button)
{
    if (button == &triggerButton) {
        processor.manualTrigger = 1;
    }
}

void TransitionFxAudioProcessorEditor::sliderValueChanged(Slider *slider)
{
   
}

void TransitionFxAudioProcessorEditor::comboBoxChanged(ComboBox *comboBoxThatHasChanged)
{

}



