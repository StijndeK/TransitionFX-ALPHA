/*
  ==============================================================================

    SourceSoundEditor.cpp
    Created: 26 Nov 2018 3:35:49pm
    Author:  Stijn de Koning

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "SourceSoundEditor.h"

//==============================================================================
SourceSoundEditor::SourceSoundEditor(TransitionFxAudioProcessor& p) : processor(p),
osc1 (p, processor.oscMenus[0], processor.oscGains[0], processor.oscDetunes[0]),
osc2 (p, processor.oscMenus[1], processor.oscGains[1], processor.oscDetunes[1]),
osc3 (p, processor.oscMenus[2], processor.oscGains[2], processor.oscDetunes[2]),
multi1 (p, processor.osc1AmountOfOscVoicesIDS[0], processor.osc1VoicesPitchOffsetIDS[0]),
multi2 (p, processor.osc1AmountOfOscVoicesIDS[1], processor.osc1VoicesPitchOffsetIDS[1]),
multi3 (p, processor.osc1AmountOfOscVoicesIDS[2], processor.osc1VoicesPitchOffsetIDS[2]),
hits(p)
{
    addAndMakeVisible(&osc1);
    addAndMakeVisible(&osc2);
    addAndMakeVisible(&osc3);
    addAndMakeVisible(&multi1);
    addAndMakeVisible(&multi2);
    addAndMakeVisible(&multi3);
    addAndMakeVisible(&hits);
}

SourceSoundEditor::~SourceSoundEditor()
{
}

void SourceSoundEditor::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::white);
    g.setFont (14.0f);
    g.drawText ("SourceSoundEditor", getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
}

void SourceSoundEditor::resized()
{
    osc1.setBounds(getLocalBounds());
    osc2.setBounds(getLocalBounds());
    osc3.setBounds(getLocalBounds());
    multi1.setBounds(getLocalBounds());
    multi2.setBounds(getLocalBounds());
    multi3.setBounds(getLocalBounds());
    hits.setBounds(getLocalBounds());
}
