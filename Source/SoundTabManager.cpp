/*
  ==============================================================================

    SoundTabManager.cpp
    Created: 4 May 2019 11:15:48am
    Author:  Stijn de Koning

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "SoundTabManager.h"

//==============================================================================
SoundTabManager::SoundTabManager(TransitionFxAudioProcessor& p) : processor(p), sourceSound(p), soundTab(p), modulation(p), effects(p), masterEnvelope(p), mixer(p), modulationTab(p), masterModulationTab(p)
{
    // source sound
    addAndMakeVisible(&sourceSound);
    
    // sound tab
    addAndMakeVisible(&soundTab);
    
    soundTab.oscSelectionButtons       [0].onClick = [this] { updateToggleState (&soundTab.oscSelectionButtons        [0]);   };
    soundTab.oscSelectionButtons       [1].onClick = [this] { updateToggleState (&soundTab.oscSelectionButtons        [1]);   };
    soundTab.oscSelectionButtons       [2].onClick = [this] { updateToggleState (&soundTab.oscSelectionButtons        [2]);   };
    
    soundTab.multiSelectionButtons       [0].onClick = [this] { updateToggleState (&soundTab.multiSelectionButtons        [0]);   };
    soundTab.multiSelectionButtons       [1].onClick = [this] { updateToggleState (&soundTab.multiSelectionButtons        [1]);   };
    soundTab.multiSelectionButtons       [2].onClick = [this] { updateToggleState (&soundTab.multiSelectionButtons        [2]);   };
    
    soundTab.modulationButtons       [0].onClick = [this] { updateToggleState (&soundTab.modulationButtons        [0]);   };
    soundTab.modulationButtons       [1].onClick = [this] { updateToggleState (&soundTab.modulationButtons        [1]);   };
    soundTab.modulationButtons       [2].onClick = [this] { updateToggleState (&soundTab.modulationButtons        [2]);   };
    soundTab.modulationButtons       [3].onClick = [this] { updateToggleState (&soundTab.modulationButtons        [3]);   };
    
    soundTab.impactButton.onClick = [this] { updateToggleState (&soundTab.impactButton); };
    soundTab.effectsButton.onClick = [this] { updateToggleState (&soundTab.effectsButton); };
    
    soundTab.envelopeButton.onClick = [this] { updateToggleState (&soundTab.envelopeButton); };
    soundTab.mixerButton.onClick = [this] { updateToggleState (&soundTab.mixerButton); };

    // modulation
    addAndMakeVisible(&modulation);
    
    // effects
    addAndMakeVisible(&effects);
    
    // master env
    addAndMakeVisible(&masterEnvelope);
    
    // mixer
    addAndMakeVisible(&mixer);
    
    // modulationtab
    addAndMakeVisible(&modulationTab);
    
    modulationTab.panModButton.onClick = [this] { updateToggleState (&modulationTab.panModButton);   };
    modulationTab.gainModButton.onClick = [this] { updateToggleState (&modulationTab.gainModButton);   };
    modulationTab.pitchModButton.onClick = [this] { updateToggleState (&modulationTab.pitchModButton);   };
    modulationTab.multiModButton.onClick = [this] { updateToggleState (&modulationTab.multiModButton);   };
    
    modulationTab.onOffButtons[0].onClick = [this] { updateToggleState (&modulationTab.onOffButtons[0]);   };
    modulationTab.onOffButtons[1].onClick = [this] { updateToggleState (&modulationTab.onOffButtons[1]);   };
    modulationTab.onOffButtons[2].onClick = [this] { updateToggleState (&modulationTab.onOffButtons[2]);   };
    modulationTab.onOffButtons[3].onClick = [this] { updateToggleState (&modulationTab.onOffButtons[3]);   };
    
    // master modulationtab
    addAndMakeVisible(&masterModulationTab);
    masterModulationTab.panModButton.onClick = [this] { updateToggleState (&masterModulationTab.panModButton);   };
    masterModulationTab.gainModButton.onClick = [this] { updateToggleState (&masterModulationTab.gainModButton);   };
    masterModulationTab.pitchModButton.onClick = [this] { updateToggleState (&masterModulationTab.pitchModButton);   };
    masterModulationTab.lfoModButton.onClick = [this] { updateToggleState (&masterModulationTab.lfoModButton);   };
    masterModulationTab.filterModButton.onClick = [this] { updateToggleState (&masterModulationTab.filterModButton);   };
    masterModulationTab.reverbModButton.onClick = [this] { updateToggleState (&masterModulationTab.reverbModButton);   };

    masterModulationTab.onOffButtons[0].onClick = [this] { updateToggleState (&masterModulationTab.onOffButtons[0]);   };
    masterModulationTab.onOffButtons[1].onClick = [this] { updateToggleState (&masterModulationTab.onOffButtons[1]);   };
    masterModulationTab.onOffButtons[2].onClick = [this] { updateToggleState (&masterModulationTab.onOffButtons[2]);   };
    masterModulationTab.onOffButtons[3].onClick = [this] { updateToggleState (&masterModulationTab.onOffButtons[3]);   };
    masterModulationTab.onOffButtons[4].onClick = [this] { updateToggleState (&masterModulationTab.onOffButtons[4]);   };
    masterModulationTab.onOffButtons[5].onClick = [this] { updateToggleState (&masterModulationTab.onOffButtons[5]);   };


    // initialise
    sourceSound.setVisible(false);
    soundTab.setVisible(true);
    modulation.setVisible(false);
    effects.setVisible(false);
    masterEnvelope.setVisible(false);
    mixer.setVisible(false);
    sourceSound.osc1.setVisible(false);
    sourceSound.osc2.setVisible(false);
    sourceSound.osc3.setVisible(false);
    sourceSound.multi1.setVisible(false);
    sourceSound.multi2.setVisible(false);
    sourceSound.multi3.setVisible(false);
    sourceSound.hits.setVisible(false);
    modulationTab.setVisible(false);
    masterModulationTab.setVisible(false);
    
    // back button
    addAndMakeVisible(&backButton);
    backButton.onClick = [this] { updateToggleState (&backButton);   };
    backButton.setVisible(false);
    
    backButton.setColour( TextButton::ColourIds::buttonColourId, Colours::darkgrey);

}


SoundTabManager::~SoundTabManager()
{
}

void SoundTabManager::updateToggleState (Button* button)
{

        
    for (int i = 0; i < 3; i++) {
        if (button == &soundTab.oscSelectionButtons[i]) {
            
            // set pop up screen visible
            sourceSound.setVisible(true);
            soundTab.setVisible(false);
            backButton.setVisible(true);
            
            // select which osc visible
            if (i == 0) {
                sourceSound.osc1.setVisible(true);
                soundTab.voicesSlider.setValue(1);
            }
            else if (i == 1) {
                sourceSound.osc2.setVisible(true);
                soundTab.voicesSlider.setValue(2);
            }
            else {
                sourceSound.osc3.setVisible(true);
                soundTab.voicesSlider.setValue(3);
            }
        }
        if (button == &soundTab.multiSelectionButtons[i]) {
            
            // set pop up screen visible
            sourceSound.setVisible(true);
            soundTab.setVisible(false);
            backButton.setVisible(true);
            
            // select which osc visible
            if (i == 0) {
                sourceSound.multi1.setVisible(true);
                soundTab.voicesSlider.setValue(1);
            }
            else if (i == 1) {
                sourceSound.multi2.setVisible(true);
                soundTab.voicesSlider.setValue(2);
            }
            else {
                sourceSound.multi3.setVisible(true);
                soundTab.voicesSlider.setValue(3);
            }
        }
    }
    
    for (int i = 0; i < 6; i++) {
        
        if (button == &masterModulationTab.onOffButtons[i]) {
            // TODO DIT LOGISCHER LATEN LOPEN
            // set envelopes from aspects on or off
            
            bool onOff = masterModulationTab.onOffButtons[i].getToggleState() == 1 ? true : false;
            if (i == 0) {   // pan
                modulation.env3.onOffButton[2].setToggleState(onOff, NotificationType::sendNotification);
            }
            else if (i == 1) {  // gain
                
            }
            else if (i == 2) {  // pitch
            }
            else if (i == 3) {  // filter
                modulation.env3.onOffButton[1].setToggleState(onOff, NotificationType::sendNotification);
                modulation.env3.onOffButton[3].setToggleState(onOff, NotificationType::sendNotification);
            }
            else if (i == 4) {  // reverb
                modulation.env3.onOffButton[4].setToggleState(onOff, NotificationType::sendNotification);
            }
            else if (i == 5) {  // lfo
                modulation.env3.onOffButton[0].setToggleState(onOff, NotificationType::sendNotification);
            }
        }
    }
    
    for (int i = 0; i < 4; i++) {
        if (button == &soundTab.modulationButtons[i]) {
            
            // set pop up screen visible
            soundTab.setVisible(false);
            backButton.setVisible(true);

            
            // select which mod visible
            if (i == 0) {
                modulation.modulationMenu.setSelectedId(1);
                masterModulationTab.setVisible(true);
            }
            else if (i == 1) {
                modulation.modulationMenu.setSelectedId(2);
                soundTab.voicesSlider.setValue(1);
                modulationTab.setVisible(true);
            }
            else if (i == 2) {
                modulation.modulationMenu.setSelectedId(2);
                soundTab.voicesSlider.setValue(2);
                modulationTab.setVisible(true);
            }
            else {
                modulation.modulationMenu.setSelectedId(2);
                soundTab.voicesSlider.setValue(3);
                modulationTab.setVisible(true);
            }
        }
    }
    
    if (button == &modulationTab.panModButton) {
        modulationTab.setVisible(false);
        modulation.setVisible(true);
        modulation.modulationChoiceMenuVoices.setSelectedId(5);
    }
    if (button == &modulationTab.gainModButton) {
        modulationTab.setVisible(false);
        modulation.setVisible(true);
        modulation.modulationChoiceMenuVoices.setSelectedId(3);
    }
    if (button == &modulationTab.pitchModButton) {
        modulationTab.setVisible(false);
        modulation.setVisible(true);
        modulation.modulationChoiceMenuVoices.setSelectedId(2);
    }
    if (button == &modulationTab.multiModButton) {
        modulationTab.setVisible(false);
        modulation.setVisible(true);
        modulation.modulationChoiceMenuVoices.setSelectedId(4);
    }
    
    if (button == &masterModulationTab.panModButton) {
        masterModulationTab.setVisible(false);
        modulation.setVisible(true);
        modulation.modulationChoiceMenu.setSelectedId(5);
    }
    if (button == &masterModulationTab.gainModButton) {
        masterModulationTab.setVisible(false);
        modulation.setVisible(true);
        modulation.modulationChoiceMenu.setSelectedId(6);
    }
    if (button == &masterModulationTab.pitchModButton) {
        masterModulationTab.setVisible(false);
        modulation.setVisible(true);
        modulation.modulationChoiceMenu.setSelectedId(7);
    }
    if (button == &masterModulationTab.lfoModButton) {
        masterModulationTab.setVisible(false);
        modulation.setVisible(true);
        modulation.modulationChoiceMenu.setSelectedId(2);
    }
    if (button == &masterModulationTab.filterModButton) {
        masterModulationTab.setVisible(false);
        modulation.setVisible(true);
        modulation.modulationChoiceMenu.setSelectedId(3);
    }
    if (button == &masterModulationTab.reverbModButton) {
        masterModulationTab.setVisible(false);
        modulation.setVisible(true);
        modulation.modulationChoiceMenu.setSelectedId(4);
    }
    
    if (button == &soundTab.impactButton) {
        sourceSound.setVisible(true);
        soundTab.setVisible(false);
        backButton.setVisible(true);
        sourceSound.hits.setVisible(true);
    }
    
    
    if (button == &soundTab.effectsButton) {
        sourceSound.setVisible(false);
        soundTab.setVisible(false);
        backButton.setVisible(true);
        sourceSound.hits.setVisible(false);
        effects.setVisible(true);
    }
    
    if (button == &soundTab.envelopeButton) {
        soundTab.setVisible(false);
        backButton.setVisible(true);
        masterEnvelope.setVisible(true);
    }
    
    if (button == &soundTab.mixerButton) {
        soundTab.setVisible(false);
        backButton.setVisible(true);
        mixer.setVisible(true);
    }
    
    if (button == &backButton) {
        sourceSound.osc1.setVisible(false);
        sourceSound.osc2.setVisible(false);
        sourceSound.osc3.setVisible(false);
        sourceSound.hits .setVisible(false);
        sourceSound.setVisible(false);
        backButton.setVisible(false);
        soundTab.setVisible(true);
        modulation.setVisible(false);
        effects.setVisible(false);
        masterEnvelope.setVisible(false);
        mixer.setVisible(false);
        sourceSound.multi1.setVisible(false);
        sourceSound.multi2.setVisible(false);
        sourceSound.multi3.setVisible(false);
        modulationTab.setVisible(false);
        masterModulationTab.setVisible(false);
    }
}

void SoundTabManager::paint (Graphics& g)
{

}

void SoundTabManager::resized()
{
    Rectangle<int> area = getLocalBounds();
    Rectangle<int> backButtonArea = getLocalBounds();

    backButton.setBounds(backButtonArea.reduced(5).withTrimmedLeft(10).removeFromLeft(80).removeFromTop(30));
    
    sourceSound.setBounds(area);
    soundTab.setBounds(area);
    modulation.setBounds(area);
    effects.setBounds(area);
    masterEnvelope.setBounds(area);
    mixer.setBounds(area.reduced(0, area.getHeight()/4));
    modulationTab.setBounds(area);
    masterModulationTab.setBounds(area);

}
