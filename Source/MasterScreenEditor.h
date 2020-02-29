/*
  ==============================================================================

    MasterScreenEditor.h
    Created: 26 Nov 2018 3:35:12pm
    Author:  Stijn de Koning

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "EnvelopeEditor.h"


//==============================================================================
/*
*/
class MasterScreenEditor    : public Component
{
public:
    MasterScreenEditor(TransitionFxAudioProcessor&);
    ~MasterScreenEditor();

    void paint (Graphics&) override;
    void resized() override;
    
    void updateToggleState (Button* button);


private:
    TransitionFxAudioProcessor& processor;
    
    EnvelopeEditor envelope;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MasterScreenEditor)
};
