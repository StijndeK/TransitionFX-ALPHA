/*
  ==============================================================================

    SoundTabManager.h
    Created: 4 May 2019 11:15:48am
    Author:  Stijn de Koning

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "SourceSoundEditor.h"
#include "SoundTabEditor.h"
#include "ModulationEditor.h"
#include "EffectsEditor.h"
#include "MasterScreenEditor.h"
#include "MasterEditor.h"
#include "ModulationTabEditor.h"
#include "masterModulationTabEditor.h"


//==============================================================================
/*
*/
class SoundTabManager    : public Component
{
public:
    SoundTabManager(TransitionFxAudioProcessor&);
    ~SoundTabManager();

    void paint (Graphics&) override;
    void resized() override;
    
    void updateToggleState (Button* button);

private:
    // back button
    TextButton backButton {"Back"};
    
    // processor
    TransitionFxAudioProcessor& processor;
    
    // source sound
    SourceSoundEditor sourceSound;
    
    // sound tab menu
    SoundTabEditor soundTab;
    
    // modulation
    ModulationEditor modulation;
    
    // effects
    EffectsEditor effects;
    
    // envelope
    MasterScreenEditor masterEnvelope;
    
    // mixer
    MasterEditor mixer;
    
    // modulation tab
    ModulationTabEditor modulationTab;
    
    // master modulation tab
    masterModulationTabEditor masterModulationTab;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoundTabManager)
};
