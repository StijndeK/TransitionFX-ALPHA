/*
  ==============================================================================

    ModulationEditor.cpp
    Created: 26 Nov 2018 3:30:15pm
    Author:  Stijn de Koning

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "ModulationEditor.h"

//==============================================================================
ModulationEditor::ModulationEditor(TransitionFxAudioProcessor& p) :
processor(p),
env2(p, processor.env2OnOffButtonsP[0], processor.env2DopplerButtonsP[0], processor.env2AttackSlidersP[0], processor.env2ReleaseSlidersP[0], processor.env2DelaySlidersP[0], processor.env2ForwardsButtonsP[0], processor.env2RangeSlidersP[0][0], processor.env2RangeSlidersP[1][0], processor.env2SustainButtonsP[0]),
env2b(p, processor.env2OnOffButtonsP[1], processor.env2DopplerButtonsP[1], processor.env2AttackSlidersP[1], processor.env2ReleaseSlidersP[1], processor.env2DelaySlidersP[1], processor.env2ForwardsButtonsP[1], processor.env2RangeSlidersP[0][1], processor.env2RangeSlidersP[1][1], processor.env2SustainButtonsP[1]),
env2c(p, processor.env2OnOffButtonsP[2], processor.env2DopplerButtonsP[2], processor.env2AttackSlidersP[2], processor.env2ReleaseSlidersP[2], processor.env2DelaySlidersP[2], processor.env2ForwardsButtonsP[2], processor.env2RangeSlidersP[0][2], processor.env2RangeSlidersP[1][2], processor.env2SustainButtonsP[2]),
lfo(p, processor.lfo1MenuIDs[0], processor.lfo1FreqSliderIds[0]),
lfoV1(p, processor.lfo2Menus[0], processor.lfo2FreqSliders[0]),
lfoV2(p, processor.lfo2Menus[1], processor.lfo2FreqSliders[1]),
lfoV3(p, processor.lfo2Menus[2], processor.lfo2FreqSliders[2]),
env3 (p, processor.env3OnOffButtonsP, processor.env3DopplerButtonsP, processor.env3AttackSlidersP, processor.env3ReleaseSliderP, processor.env3DelaySliderP, processor.env3ForwardsButtonsP, processor.env3LowRanges, processor.env3HighRanges, processor.env3SustainButtonsP),
masterEnvelope(p)
{
    addAndMakeVisible(&env2b);
    addAndMakeVisible(&env2c);
    addAndMakeVisible(&env2);
    addAndMakeVisible(&env3);
    
    addAndMakeVisible(&lfoV2);
    addAndMakeVisible(&lfoV3);
    addAndMakeVisible(&lfoV1);
    addAndMakeVisible(&lfo);

    // Comboboxes
    modulationMenu.addItem("master", 1);
    modulationMenu.addItem("voice", 2);
    modulationMenu.setJustificationType(Justification::centred);
    addAndMakeVisible(&modulationMenu);
    modulationMenu.addListener(this);
    modulationMenu.setSelectedId(1);
    
    modulationMenu.setVisible(false);
    
    addAndMakeVisible(&masterButton);
    addAndMakeVisible(&voiceButton);
    masterButton.onClick = [this] { updateToggleState (&masterButton);   };
    voiceButton.onClick = [this] { updateToggleState (&voiceButton);   };
    
    modulationChoiceMenu.addItem("No selection", 1);
    modulationChoiceMenu.addItem("LFO", 2);
    modulationChoiceMenu.addItem("Filter", 3);
    modulationChoiceMenu.addItem("Reverb", 4);
    modulationChoiceMenu.addItem("Pan", 5);
    modulationChoiceMenu.addItem("Gain", 6);
    modulationChoiceMenu.addItem("Detune", 7);
    modulationChoiceMenu.setJustificationType(Justification::centred);
    addAndMakeVisible(&modulationChoiceMenu);
    modulationChoiceMenu.addListener(this);
    modulationChoiceMenu.setSelectedId(1);
    
    modulationChoiceMenuVoices.addItem("No selection", 1);
    modulationChoiceMenuVoices.addItem("Detune", 2);
    modulationChoiceMenuVoices.addItem("Gain", 3);
    modulationChoiceMenuVoices.addItem("Osc Voices Detune", 4);
    modulationChoiceMenuVoices.addItem("Pan", 5);
    addAndMakeVisible(&modulationChoiceMenuVoices);
    modulationChoiceMenuVoices.setJustificationType(Justification::centred);
    modulationChoiceMenuVoices.addListener(this);
    modulationChoiceMenuVoices.setSelectedId(1);
    
    modulationTypeMenu.addItem("Envelope", 1);
    modulationTypeMenu.addItem("LFO", 2);
    modulationTypeMenu.addItem("Sequencer", 3);
    addAndMakeVisible(&modulationTypeMenu);
    modulationTypeMenu.setJustificationType(Justification::centred);
    modulationTypeMenu.addListener(this);
    modulationTypeMenu.setSelectedId(1);
    
    string aspectOnOffMasterNames[6];
    string aspectOnOffVoiceNames[3][6];

    
    // Buttons
    for (int i = 0; i < 6; i++) {
        addAndMakeVisible(masterChoiceButtons[i]);
        masterChoiceButtons[i].setButtonText(masterChoiceButtonNames[i]);
        addAndMakeVisible(voiceChoiceButtons[i]);
        voiceChoiceButtons[i].setButtonText(voiceChoiceButtonNames[i]);
        addAndMakeVisible(aspectOnOffButtonMaster[i]);
        aspectOnOffMasterNames[i] = processor.modulationMasterTriggersP[i];
        aspectOnOffButtonMasterTree[i] = new AudioProcessorValueTreeState::ButtonAttachment (processor.tree, aspectOnOffMasterNames[i], aspectOnOffButtonMaster[i]);

        aspectOnOffVoiceNames[0][i] = processor.modulationVoiceTriggersP[0][i];
        aspectOnOffVoiceNames[1][i] = processor.modulationVoiceTriggersP[1][i];
        aspectOnOffVoiceNames[2][i] = processor.modulationVoiceTriggersP[2][i];
        
        aspectOnOffButtonVoiceTree[0][i] = new AudioProcessorValueTreeState::ButtonAttachment (processor.tree, aspectOnOffVoiceNames[0][i], aspectOnOffButtonVoice[0][i]);
        aspectOnOffButtonVoiceTree[1][i] = new AudioProcessorValueTreeState::ButtonAttachment (processor.tree, aspectOnOffVoiceNames[1][i], aspectOnOffButtonVoice[1][i]);
        aspectOnOffButtonVoiceTree[2][i] = new AudioProcessorValueTreeState::ButtonAttachment (processor.tree, aspectOnOffVoiceNames[2][i], aspectOnOffButtonVoice[2][i]);
        
        addAndMakeVisible(aspectOnOffButtonVoice[0][i]);
        addAndMakeVisible(aspectOnOffButtonVoice[1][i]);
        addAndMakeVisible(aspectOnOffButtonVoice[2][i]);
        
        addAndMakeVisible(aspectOnOffButtonLFOMaster[i]);
    }
    
    addAndMakeVisible(openMenuButton);
    openMenuButton.onClick = [this] { updateToggleState (&openMenuButton);   };

    masterChoiceButtons[0].onClick = [this] { updateToggleState (&masterChoiceButtons[0]);   };
    masterChoiceButtons[1].onClick = [this] { updateToggleState (&masterChoiceButtons[1]);   };
    masterChoiceButtons[2].onClick = [this] { updateToggleState (&masterChoiceButtons[2]);   };
    masterChoiceButtons[3].onClick = [this] { updateToggleState (&masterChoiceButtons[3]);   };
    masterChoiceButtons[4].onClick = [this] { updateToggleState (&masterChoiceButtons[4]);   };
    masterChoiceButtons[5].onClick = [this] { updateToggleState (&masterChoiceButtons[5]);   };
    
    voiceChoiceButtons[0].onClick = [this] { updateToggleState (&voiceChoiceButtons[0]);   };
    voiceChoiceButtons[1].onClick = [this] { updateToggleState (&voiceChoiceButtons[1]);   };
    voiceChoiceButtons[2].onClick = [this] { updateToggleState (&voiceChoiceButtons[2]);   };
    voiceChoiceButtons[3].onClick = [this] { updateToggleState (&voiceChoiceButtons[3]);   };
    voiceChoiceButtons[4].onClick = [this] { updateToggleState (&voiceChoiceButtons[4]);   };
    voiceChoiceButtons[5].onClick = [this] { updateToggleState (&voiceChoiceButtons[5]);   };
    
    aspectOnOffButtonMaster[0].onClick = [this] { updateToggleState (&aspectOnOffButtonMaster[0]);   };
    aspectOnOffButtonMaster[1].onClick = [this] { updateToggleState (&aspectOnOffButtonMaster[1]);   };
    aspectOnOffButtonMaster[2].onClick = [this] { updateToggleState (&aspectOnOffButtonMaster[2]);   };
    aspectOnOffButtonMaster[3].onClick = [this] { updateToggleState (&aspectOnOffButtonMaster[3]);   };
    aspectOnOffButtonMaster[4].onClick = [this] { updateToggleState (&aspectOnOffButtonMaster[4]);   };
    aspectOnOffButtonMaster[5].onClick = [this] { updateToggleState (&aspectOnOffButtonMaster[5]);   };
    
    aspectOnOffButtonVoice[0][0].onClick = [this] { updateToggleState (&aspectOnOffButtonVoice[0][0]);   };
    aspectOnOffButtonVoice[0][1].onClick = [this] { updateToggleState (&aspectOnOffButtonVoice[0][1]);   };
    aspectOnOffButtonVoice[0][2].onClick = [this] { updateToggleState (&aspectOnOffButtonVoice[0][2]);   };
    aspectOnOffButtonVoice[0][3].onClick = [this] { updateToggleState (&aspectOnOffButtonVoice[0][3]);   };
    aspectOnOffButtonVoice[0][4].onClick = [this] { updateToggleState (&aspectOnOffButtonVoice[0][4]);   };
    aspectOnOffButtonVoice[0][5].onClick = [this] { updateToggleState (&aspectOnOffButtonVoice[0][5]);   };
    
    aspectOnOffButtonVoice[1][0].onClick = [this] { updateToggleState (&aspectOnOffButtonVoice[1][0]);   };
    aspectOnOffButtonVoice[1][1].onClick = [this] { updateToggleState (&aspectOnOffButtonVoice[1][1]);   };
    aspectOnOffButtonVoice[1][2].onClick = [this] { updateToggleState (&aspectOnOffButtonVoice[1][2]);   };
    aspectOnOffButtonVoice[1][3].onClick = [this] { updateToggleState (&aspectOnOffButtonVoice[1][3]);   };
    aspectOnOffButtonVoice[1][4].onClick = [this] { updateToggleState (&aspectOnOffButtonVoice[1][4]);   };
    aspectOnOffButtonVoice[1][5].onClick = [this] { updateToggleState (&aspectOnOffButtonVoice[1][5]);   };
    
    aspectOnOffButtonVoice[2][0].onClick = [this] { updateToggleState (&aspectOnOffButtonVoice[2][0]);   };
    aspectOnOffButtonVoice[2][1].onClick = [this] { updateToggleState (&aspectOnOffButtonVoice[2][1]);   };
    aspectOnOffButtonVoice[2][2].onClick = [this] { updateToggleState (&aspectOnOffButtonVoice[2][2]);   };
    aspectOnOffButtonVoice[2][3].onClick = [this] { updateToggleState (&aspectOnOffButtonVoice[2][3]);   };
    aspectOnOffButtonVoice[2][4].onClick = [this] { updateToggleState (&aspectOnOffButtonVoice[2][4]);   };
    aspectOnOffButtonVoice[2][5].onClick = [this] { updateToggleState (&aspectOnOffButtonVoice[2][5]);   };
    
    // voice slider listener
    masterEnvelope.voicesSlider.addListener(this);
    
    // Initialise
    selectGui2();
}

ModulationEditor::~ModulationEditor()
{
}

void ModulationEditor::paint (Graphics& g)
{
    g.fillAll (Colours::black);
    
    Rectangle<int> area = getLocalBounds().reduced(10);
    
    g.setColour (Colours::grey);
    Rectangle<int> areaRR = area.withTrimmedTop(20);
    g.drawRoundedRectangle(areaRR.getX(), areaRR.getY(), areaRR.getWidth(), areaRR.getHeight(), 8, 2.0);
    g.setColour(Colour(processor.red, processor.green, processor.blue));
    g.fillRoundedRectangle(areaRR.getX(), areaRR.getY(), areaRR.getWidth(), areaRR.getHeight(), 8);

    g.setColour (Colours::white);
    g.setFont (16.0f);
    g.drawText ("Modulation", getLocalBounds().withTrimmedTop(10), Justification::centredTop, true);
    
    Rectangle<int> areaModulationType = getLocalBounds().reduced(10).withTrimmedTop(20).removeFromTop(40);
    g.drawText ("Modulation type: ", areaModulationType.removeFromRight(250), Justification::centredLeft, true);
}

void ModulationEditor::resized()
{
    Rectangle<int> area = getLocalBounds().reduced(10).withTrimmedTop(20);
    
    // master/voice choice
    Rectangle<int> mvbArea = area.removeFromTop(30).withTrimmedLeft(20).withTrimmedTop(10);
    modulationMenu.setBounds(mvbArea.removeFromLeft(220));
//    masterButton.setBounds(mvbArea.removeFromLeft(60));
//    voiceButton.setBounds(mvbArea.removeFromLeft(60));
    
    // modulutation type menu
    modulationTypeMenu.setBounds(mvbArea.removeFromRight(120).withTrimmedRight(20));
    
    // envelopes and lfos
    Rectangle<int> area2 = area.withTrimmedTop(20);
    
    env2.setBounds(area2);
    env2b.setBounds(area2);
    env2c.setBounds(area2);
    env3.setBounds(area2);
    
    lfo.setBounds(area2);
    lfoV1.setBounds(area2);
    lfoV2.setBounds(area2);
    lfoV3.setBounds(area2);
}

//==============================================================================
void ModulationEditor::selectGui2()
{
    lfoV1.setVisible(false);
    lfoV2.setVisible(false);
    lfoV3.setVisible(false);
    env2c.setVisible(false);
    env2b.setVisible(false);
    env2.setVisible(false);
    
    // env on off toggles
    for (int i = 0; i < 6; i++) {
        aspectOnOffButtonVoice[0][i].setVisible(false);
        aspectOnOffButtonVoice[1][i].setVisible(false);
        aspectOnOffButtonVoice[2][i].setVisible(false);
    }
    if (modulationMenu.getSelectedId() == 2) {
        if (masterEnvelope.voicesSlider.getValue() == 1) {
            for (int i = 0; i < 6; i++) {
                aspectOnOffButtonVoice[0][i].setVisible(true);
            }
        }
        else if (masterEnvelope.voicesSlider.getValue() == 2) {
            for (int i = 0; i < 6; i++) {
                aspectOnOffButtonVoice[1][i].setVisible(true);
            }
        }
        else {
            for (int i = 0; i < 6; i++) {
                aspectOnOffButtonVoice[2][i].setVisible(true);
            }
        }
    }

    
    // 1: check which moudlationvoice component is selected, 2: check which voice the voiceselectionslider is on, 3: check lfo or envelope
    if ( modulationChoiceMenuVoices.getSelectedId() == 1) { // if nothing is selected
    }
    else if ( modulationChoiceMenuVoices.getSelectedId() == 2) { // if detune is selected
        
        if (masterEnvelope.voicesSlider.getValue() == 1){       // voice 1
            // set correct ids
            lfoV1.lfo2ComponentChoiceMenu.setSelectedId(2);
            env2.env2Menu.setSelectedId(2);
            
            // check modulation type
            if (modulationTypeMenu.getSelectedId() == 1){
                env2.setVisible(true);
            }
            else {
                lfoV1.setVisible(true);
            }
        }
        else if (masterEnvelope.voicesSlider.getValue() == 2){  // voice 2
            // set correct ids
            lfoV2.lfo2ComponentChoiceMenu.setSelectedId(2);
            env2b.env2Menu.setSelectedId(2);
            
            // check modulation type
            if (modulationTypeMenu.getSelectedId() == 1){
                env2b.setVisible(true);
            }
            else {
                lfoV2.setVisible(true);
            }
        }
        else {                                                  // voice 3
            // set correct ids
            lfoV3.lfo2ComponentChoiceMenu.setSelectedId(2);
            env2c.env2Menu.setSelectedId(2);
            
            // check modulation type
            if (modulationTypeMenu.getSelectedId() == 1){
                env2c.setVisible(true);
            }
            else {
                lfoV3.setVisible(true);
            }
        }
    }
    
    else if ( modulationChoiceMenuVoices.getSelectedId() == 3) { // if gain is selected
        if (masterEnvelope.voicesSlider.getValue() == 1){       // voice 1
            // set correct ids
            lfoV1.lfo2ComponentChoiceMenu.setSelectedId(1);
            env2.env2Menu.setSelectedId(5);
            
            // check modulation type
            if (modulationTypeMenu.getSelectedId() == 1){
                env2.setVisible(true);
            }
            else {
                lfoV1.setVisible(true);
            }
        }
        else if (masterEnvelope.voicesSlider.getValue() == 2){  // voice 2
            // set correct ids
            lfoV2.lfo2ComponentChoiceMenu.setSelectedId(1);
            env2b.env2Menu.setSelectedId(5);
            
            // check modulation type
            if (modulationTypeMenu.getSelectedId() == 1){
                env2b.setVisible(true);
            }
            else {
                lfoV2.setVisible(true);
            }
        }
        else {                                                  // voice 3
            // set correct ids
            lfoV3.lfo2ComponentChoiceMenu.setSelectedId(1);
            env2c.env2Menu.setSelectedId(5);
            
            // check modulation type
            if (modulationTypeMenu.getSelectedId() == 1){
                env2c.setVisible(true);
            }
            else {
                lfoV3.setVisible(true);
            }
        }
    }
    
    else if (modulationChoiceMenuVoices.getSelectedId() == 4) {  // if osc voice detune is selected
        if (masterEnvelope.voicesSlider.getValue() == 1){       // voice 1
            // set correct ids
            env2.env2Menu.setSelectedId(3);
            
            // check modulation type
            if (modulationTypeMenu.getSelectedId() == 1){
                env2.setVisible(true);
            }
            else {
            }
        }
        else if (masterEnvelope.voicesSlider.getValue() == 2){  // voice 2
            // set correct ids
            env2b.env2Menu.setSelectedId(3);
            
            // check modulation type
            if (modulationTypeMenu.getSelectedId() == 1){
                env2b.setVisible(true);
            }
            else {
            }
        }
        else {                                                  // voice 3
            // set correct ids
            env2c.env2Menu.setSelectedId(3);
            
            // check modulation type
            if (modulationTypeMenu.getSelectedId() == 1){
                env2c.setVisible(true);
            }
            else {
            }
        }
    }
    else {                                                      // if pan is selected
        if (masterEnvelope.voicesSlider.getValue() == 1){       // voice 1
            // set correct ids
            env2.env2Menu.setSelectedId(4);
            
            // check modulation type
            if (modulationTypeMenu.getSelectedId() == 1){
                env2.setVisible(true);
            }
            else {
            }
        }
        else if (masterEnvelope.voicesSlider.getValue() == 2){  // voice 2
            // set correct ids
            env2b.env2Menu.setSelectedId(4);
            
            // check modulation type
            if (modulationTypeMenu.getSelectedId() == 1){
                env2b.setVisible(true);
            }
            else {
            }
        }
        else {                                                  // voice 3
            // set correct ids
            env2c.env2Menu.setSelectedId(4);
            
            // check modulation type
            if (modulationTypeMenu.getSelectedId() == 1){
                env2c.setVisible(true);
            }
            else {
            }
        }
    }
}

//==============================================================================
void ModulationEditor::updateToggleState(juce::Button *button)
{

}

void ModulationEditor::sliderValueChanged(Slider *slider)
{
    // select gui based on the voices selection
    if (slider == &masterEnvelope.voicesSlider){
        selectGui2();
    }
    
    
}

void ModulationEditor::comboBoxChanged(ComboBox *comboBoxThatHasChanged){
    // MASTER OR VOICES
    if (comboBoxThatHasChanged == &modulationMenu) {
        if (modulationMenu.getSelectedId() == 1){
            modulationChoiceMenu.setSelectedId(1);
            modulationChoiceMenuVoices.setSelectedId(1);
            for (int i = 0; i < 6; i ++) {
                masterChoiceButtons[i].setVisible(true);
                voiceChoiceButtons[i].setVisible(false);
                aspectOnOffButtonVoice[0][i].setVisible(false);
                aspectOnOffButtonVoice[1][i].setVisible(false);
                aspectOnOffButtonVoice[2][i].setVisible(false);
                aspectOnOffButtonMaster[i].setVisible(true);
            }
        }
        else {
            modulationChoiceMenu.setSelectedId(1);
            modulationChoiceMenuVoices.setSelectedId(1);
            selectGui2();
            for (int i = 0; i < 6; i ++) {
                masterChoiceButtons[i].setVisible(false);
                voiceChoiceButtons[i].setVisible(true);
                aspectOnOffButtonMaster[i].setVisible(false);
            }
        }
    }
    // MASTER OPTIONS
    if (comboBoxThatHasChanged == &modulationChoiceMenu){
        if (modulationChoiceMenu.getSelectedId() == 1) {    // no selection
            env3.env3Menu.setSelectedId(1);
            lfo.setVisible(false);
            env3.setVisible(false);
        }
        else if (modulationChoiceMenu.getSelectedId() == 2) { // lfo rate
            env3.env3Menu.setSelectedId(2);
            lfo.setVisible(false);
            env3.setVisible(true);
        }
        else if (modulationChoiceMenu.getSelectedId() == 3) { // filter cutoff
            env3.env3Menu.setSelectedId(3);
            lfo.setVisible(false);
            env3.setVisible(true);
        }
        else if (modulationChoiceMenu.getSelectedId() == 4) { // reverb dry/wet
            env3.env3Menu.setSelectedId(6);
            lfo.setVisible(false);
            env3.setVisible(true);
        }
        else if (modulationChoiceMenu.getSelectedId() == 5) { // pan
            env3.env3Menu.setSelectedId(4);
            lfo.setVisible(false);
            env3.setVisible(true);
        }
        else if (modulationChoiceMenu.getSelectedId() == 7) { // detune
            // TODO: Insert
            env3.env3Menu.setSelectedId(7);
            lfo.setVisible(false);
            env3.setVisible(false);
        }
        else {                                                // gain
            lfo.setVisible(true);
            env3.setVisible(false);
        }
    }
    // VOICES OPTIONS
    if (comboBoxThatHasChanged == &modulationChoiceMenuVoices){
        selectGui2();
    }
    
    // modulation type
    if (comboBoxThatHasChanged == &modulationTypeMenu){
        selectGui2();
    }
}



