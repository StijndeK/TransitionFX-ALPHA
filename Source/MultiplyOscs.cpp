/*
  ==============================================================================

    MultiplyOscs.cpp
    Created: 5 May 2019 11:57:37pm
    Author:  Stijn de Koning

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "MultiplyOscs.h"

//==============================================================================
MultiplyOscs::MultiplyOscs(TransitionFxAudioProcessor& p, string VoicesID, string VoicesPitchOffset) : processor(p)
{
    // Sliders
    osc1VoiceCountSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    osc1VoiceCountSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(&osc1VoiceCountSlider);
    osc1VoiceCountSliderTree = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, VoicesID, osc1VoiceCountSlider);
    
    osc1VoicePitchOfsetSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    osc1VoicePitchOfsetSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(&osc1VoicePitchOfsetSlider);
    osc1VoicePitchOfsetSliderTree = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, VoicesPitchOffset, osc1VoicePitchOfsetSlider);
    
    // Buttons
    addAndMakeVisible(&osc1DetuneUpSetter);
    osc1DetuneUpSetter.setButtonText("up");
    addAndMakeVisible(&osc1DetuneDownSetter);
    osc1DetuneDownSetter.setButtonText("down");
    
    osc1DetuneUpSetter  .onClick = [this] { updateToggleState (&osc1DetuneUpSetter);   };
    osc1DetuneDownSetter  .onClick = [this] { updateToggleState (&osc1DetuneDownSetter);   };
}

MultiplyOscs::~MultiplyOscs()
{
}

void MultiplyOscs::updateToggleState(Button *button)
{
    // detune up and down setters
    if (button == &osc1DetuneUpSetter) {
        // set numb
        currentnumb = currentnumb + 1;
        // check scope
        if (currentnumb < 0){
            currentnumb = currentnumb + 1;
        }
        if ( currentnumb > 18){
            currentnumb = currentnumb - 1;
        }
        osc1VoicePitchOfsetSlider.setValue(detuneValues[currentnumb]);
    }
    if (button == &osc1DetuneDownSetter) {
        // set numb
        currentnumb = currentnumb - 1;
        // check scope
        if (currentnumb < 0){
            currentnumb = currentnumb + 1;
        }
        if ( currentnumb > 18){
            currentnumb = currentnumb - 1;
        }

        osc1VoicePitchOfsetSlider.setValue(detuneValues[currentnumb]);
    }
}

void MultiplyOscs::paint (Graphics& g)
{
    g.fillAll (Colours::black);   // clear the background

    Rectangle<int> area = getLocalBounds().reduced(100);
    
    g.setColour (Colours::grey);
    Rectangle<int> areaRR = area.withTrimmedTop(20);
    g.drawRoundedRectangle(areaRR.getX(), areaRR.getY(), areaRR.getWidth(), areaRR.getHeight(), 8, 2.0);
    g.setColour(Colour(processor.red, processor.green, processor.blue));
    g.fillRoundedRectangle(areaRR.getX(), areaRR.getY(), areaRR.getWidth(), areaRR.getHeight(), 8);

    g.setColour(Colours::white);
    Rectangle<int> ovdAreaName = getLocalBounds().reduced(140).removeFromTop(20);
    g.drawText ("Multiply Oscs", ovdAreaName.removeFromLeft(ovdAreaName.getWidth()/2), Justification::centred);
    g.drawText ("Offset Pitch", ovdAreaName, Justification::centred);
}

void MultiplyOscs::resized()
{
    Rectangle<int> area = getLocalBounds().reduced(140).withTrimmedTop(20);
    osc1VoiceCountSlider.setBounds(area.removeFromLeft(area.getWidth()/2).withTrimmedBottom(20));
    osc1VoicePitchOfsetSlider.setBounds(area.withTrimmedBottom(20));
    Rectangle<int> buttonArea = area.removeFromBottom(20);
    osc1DetuneDownSetter.setBounds(buttonArea.removeFromLeft(area.getWidth()/2));
    osc1DetuneUpSetter.setBounds(buttonArea);
}
