/*
  ==============================================================================

    Tests.cpp
    Created: 26 Nov 2018 10:44:12am
    Author:  Stijn de Koning

  ==============================================================================
*/

#include "Tests.h"

/*

void TransitionFxAudioProcessorEditor::sliderValueChanged(Slider *slider){
    // TODO: overzichterlijker systeem met for-loops hiervoor maken
    // change the value of slider based on which voice is selected
    for (int i = 0; i < osc1.amountOfOscillators; i++){
        
        if (slider == &masterenv.voicesSlider) {
            if (masterenv.voicesSlider.getValue() == 1)
            {
                osc1.osc1Gain[i].setValue(oldOscValueVoice1[i]);
                osc1.osc1Detune[i].setValue(oldOscValueDetuneVoice1[i]);
                
                osc1.setVisible(true);
                osc2.setVisible(false);
                osc3.setVisible(false);
            }
            else if (masterenv.voicesSlider.getValue() == 2)
            {
                osc1.osc1Gain[i].setValue(oldOscValueVoice2[i]);
                osc1.osc1Detune[i].setValue(oldOscValueDetuneVoice2[i]);
                
                osc1.setVisible(false);
                osc2.setVisible(true);
                osc3.setVisible(false);
            }
            else {
                osc1.osc1Gain[i].setValue(oldOscValueVoice3[i]);
                osc1.osc1Detune[i].setValue(oldOscValueDetuneVoice3[i]);
                
                osc1.setVisible(false);
                osc2.setVisible(false);
                osc3.setVisible(true);
            }
            
            // random button experiments
            //            osc1.osc1Gain[0].getRange();
            //            std::cout << "min: " << osc1.osc1Gain[0].getMinimum() << std::endl;
            //            int randomValue = rand() % 84 - 84;
            //            osc1.osc1Gain[0].setValue(randomValue);
        }
        if (slider == &osc1.osc1Gain[i]){
            if (masterenv.voicesSlider.getValue() == 1)
            {
                oldOscValueVoice1[i] = osc1.osc1Gain[i].getValue();
            }
            else if (masterenv.voicesSlider.getValue() == 2)
            {
                oldOscValueVoice2[i] = osc1.osc1Gain[i].getValue();
            }
            else {
                oldOscValueVoice3[i] = osc1.osc1Gain[i].getValue();
            }
        }
        if (slider == &osc1.osc1Detune[i]){
            if (masterenv.voicesSlider.getValue() == 1)
            {
                oldOscValueDetuneVoice1[i] = osc1.osc1Detune[i].getValue();
            }
            if (masterenv.voicesSlider.getValue() == 2)
            {
                oldOscValueDetuneVoice2[i] = osc1.osc1Detune[i].getValue();
            }
            else {
                oldOscValueDetuneVoice3[i] = osc1.osc1Detune[i].getValue();
            }
        }
    }
}

 double SubVoice::setOsc1Parameters (int osc1Selection, double osc1Pitch, double osc1Volume, double pitchOfset, int oscvoicecount)
 {
 double oscSelection = 0;
 double oscSelectionOutput ;
 double calculatedPitchOffset;
 
 // set tonal pitch
 if (pitchOfset == 1 || pitchOfset == -1){
 calculatedPitchOffset = 0.333;
 }
 else if (pitchOfset == 2 || pitchOfset == -2) {
 calculatedPitchOffset = 0.583;
 }
 else if (pitchOfset == 3 || pitchOfset == -3) {
 calculatedPitchOffset = 1.0;
 }
 else {
 calculatedPitchOffset = 0.0;
 }
 
 // set pitch
 double pitch = setOsc1Pitch(osc1Pitch);
 
 // hier detune env aanroepen
 
 for (int i = 0; i < oscvoicecount; i++) {
 oscSelection = oscSelection + setOscSelection(osc1Selection, i, pitch);
 pitch = pitch * pow(2.0 , calculatedPitchOffset);
 }
 
 testValue = osc1Selection;
 
 oscSelectionOutput = oscSelection * osc1Volume;
 return oscSelectionOutput;
 }
 
 //    env1AttackSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
 //    env1AttackSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
 //    addAndMakeVisible(&env1AttackSlider);
 //    env1AttackSliderTree = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "env1AttackSliderID", env1AttackSlider);
 //
 //    env1ReleaseSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
 //    env1ReleaseSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
 //    addAndMakeVisible(&env1ReleaseSlider);
 //    env1ReleaseSliderTree = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "env1ReleaseSliderID", env1ReleaseSlider);
 

//==============================================================================
EnvelopeEditor::EnvelopeEditor(TransitionFxAudioProcessor& p) : processor(p)
{
    setSize(400, 400);
    
    // get IDs
    string pointSliderIDs[6];
    string pointReleaseSliderIDs[6];
    for (int i = 0; i < 6; i++) {
        pointSliderIDs[i] = processor.env1PointSliderIDs[i];
        pointReleaseSliderIDs[i] = processor.env1PointReleaseSliderIDs[i];
    }
    
    // COMBOBOX
    env1Menu.addItem("Linear", 1);
    env1Menu.addItem("Exponential", 2);
    env1Menu.addItem("Gemiddelde", 3);
    env1Menu.addItem("4-steps", 4);
    env1Menu.addItem("8-steps, fixed time(X) positions", 5);
    env1Menu.setJustificationType(Justification::centred);
    addAndMakeVisible(&env1Menu);
    env1Menu.addListener(this);
    env1MenuTree = new AudioProcessorValueTreeState::ComboBoxAttachment (processor.tree, "env1MenuID", env1Menu);
    
    
    // SLIDERS
    env1AmpStartValue.setSliderStyle(Slider::SliderStyle::LinearVertical);
    env1AmpStartValue.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(&env1AmpStartValue);
    env1AmpStartValueTree = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "env1AmpStartValueID", env1AmpStartValue);
    
    env1Point1YSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    env1Point1YSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(&env1Point1YSlider);
    env1Point1YSliderTree = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "env1Point1YSliderID", env1Point1YSlider);
    
    env1Point2YSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    env1Point2YSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(&env1Point2YSlider);
    env1Point2YSliderTree = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "env1Point2YSliderID", env1Point2YSlider);
    
    env1Point1XSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    env1Point1XSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(&env1Point1XSlider);
    env1Point1XSliderTree = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "env1Point1XSliderID", env1Point1XSlider);
    
    env1Point2XSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    env1Point2XSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(&env1Point2XSlider);
    env1Point2XSliderTree = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "env1Point2XSliderID", env1Point2XSlider);
    
    env1VerhoudingGemiddelde.setSliderStyle(Slider::SliderStyle::LinearVertical);
    env1VerhoudingGemiddelde.setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(&env1VerhoudingGemiddelde);
    env1VerhoudingGemiddeldeTree = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "env1VerhoudingGemiddeldeID", env1VerhoudingGemiddelde);
    
    for (int i = 0; i < 6; i++){
        env1PointSliders[i].setSliderStyle(Slider::SliderStyle::LinearVertical);
        env1PointSliders[i].setTextBoxStyle(Slider::TextBoxBelow, false, 30, 20);
        addAndMakeVisible(&env1PointSliders[i]);
        env1PointSlidersTree[i] = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, pointSliderIDs[i], env1PointSliders[i]);
        
        env1PointReleaseSliders[i].setSliderStyle(Slider::SliderStyle::LinearVertical);
        env1PointReleaseSliders[i].setTextBoxStyle(Slider::TextBoxBelow, false, 30, 20);
        addAndMakeVisible(&env1PointReleaseSliders[i]);
        env1PointReleaseSlidersTree[i] = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, pointReleaseSliderIDs[i], env1PointReleaseSliders[i]);
        
    }
    
    // slider voor het zetten van 8 step envelope
    envGemiddeldeSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    envGemiddeldeSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 60, 20);
    addAndMakeVisible(&envGemiddeldeSlider);
    envGemiddeldeSlider.addListener(this);
    envGemiddeldeSlider.setRange(0, 1);
    
    // slider voor amount of smooth
    env1SmoothAmountSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    env1SmoothAmountSlider.setTextBoxStyle(Slider::NoTextBox, true, 60, 20);
    addAndMakeVisible(&env1SmoothAmountSlider);
    env1SmoothAmountSliderTree = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "env1SmoothAmountSliderID", env1SmoothAmountSlider);
    
    
    // sliders op 0 en 1 voor visuals
    NormalisableRange<double> fixed0Range (0, 1, 2);
    for (int i = 0; i < 3; i++){
        env1Point0Sliders[i].setSliderStyle(Slider::SliderStyle::LinearVertical);
        env1Point0Sliders[i].setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
        addAndMakeVisible(&env1Point0Sliders[i]);
        env1Point0Sliders[i].setNormalisableRange(fixed0Range);
    }
    env1Point0Sliders[0].setValue(0);
    env1Point0Sliders[1].setValue(3);
    env1Point0Sliders[2].setValue(0);
    
    
    // BUTTONS
    addAndMakeVisible(&smoothOnOffButton);
    smoothOnOffButtonTree = new AudioProcessorValueTreeState::ButtonAttachment (processor.tree, "env1SmoothOnOffButtonID", smoothOnOffButton);
    addAndMakeVisible(&linearButton);
    linearButton  .onClick = [this] { updateToggleState (&linearButton);   };
    addAndMakeVisible(&exponentialButton);
    exponentialButton  .onClick = [this] { updateToggleState (&exponentialButton);   };
    addAndMakeVisible(&anticlimacticButton);
    anticlimacticButton  .onClick = [this] { updateToggleState (&anticlimacticButton);   };
    addAndMakeVisible(&sShapeButton);
    sShapeButton  .onClick = [this] { updateToggleState (&sShapeButton);   };
    
    
    // INITIALISE
    if (env1Menu.getSelectedId() == 2 || env1Menu.getSelectedId() == 3){
        env1AmpStartValue.setVisible(true);
    }
    else {
        env1AmpStartValue.setVisible(false);
    }
    if (env1Menu.getSelectedId() == 3){
        env1VerhoudingGemiddelde.setVisible(true);
    }
    else {
        env1VerhoudingGemiddelde.setVisible(false);
    }
    if (env1Menu.getSelectedId() == 4){
        env1Point1XSlider.setVisible(true);
        env1Point2XSlider.setVisible(true);
        env1Point1YSlider.setVisible(true);
        env1Point2YSlider.setVisible(true);        }
    else{
        env1Point1XSlider.setVisible(false);
        env1Point2XSlider.setVisible(false);
        env1Point1YSlider.setVisible(false);
        env1Point2YSlider.setVisible(false);
    }
    if (env1Menu.getSelectedId() == 5){
        for (int i = 0; i < 6; i++){
            env1PointSliders[i].setVisible(true);
        }
    }
    else {
        for (int i = 0; i < 6; i++){
            env1PointSliders[i].setVisible(false);
        }
    }
}

EnvelopeEditor::~EnvelopeEditor()
{
    env1MenuTree.~ScopedPointer();
    env1AttackSliderTree.~ScopedPointer();
    env1ReleaseSliderTree.~ScopedPointer();
    env1AmpStartValueTree.~ScopedPointer();
    env1VerhoudingGemiddeldeTree.~ScopedPointer();
    smoothOnOffButtonTree.~ScopedPointer();
    env1SmoothAmountSliderTree.~ScopedPointer();
}

void EnvelopeEditor::paint (Graphics& g)
{
    g.fillAll (Colours::royalblue);   // clear the background
    
    Rectangle<int> area = getLocalBounds().reduced(10);
    
    g.setColour (Colours::white);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
    
    g.setColour (Colours::white);
    g.setFont (14.0f);
    g.drawText ("Envelope", area, Justification::centredTop);   // draw some placeholder text
}

void EnvelopeEditor::resized()
{
    Rectangle<int> area = getLocalBounds().reduced(40);
    
    // drawMenu
    env1Menu.setBounds(area.removeFromTop(20));
    Rectangle<int> area3 = area;
    Rectangle<int> area4 = area3.withTrimmedBottom(140);
    Rectangle<int> area5 = area3;
    Rectangle<int> area7 = area3.withTrimmedTop(140);
    
    
    env1AmpStartValue.setBounds(area5.removeFromLeft(80));
    env1VerhoudingGemiddelde.setBounds(area5);
    
    env1Point1YSlider.setBounds(area3.removeFromLeft(80));
    env1Point2YSlider.setBounds(area3.removeFromLeft(80));
    env1Point1XSlider.setBounds(area3.removeFromTop(80));
    env1Point2XSlider.setBounds(area3.removeFromTop(80));
    
    env1Point0Sliders[0].setBounds(area4.removeFromLeft(30));
    for (int i = 0; i < 6; i++){
        env1PointSliders[i].setBounds(area4.removeFromLeft(30));
    }
    env1Point0Sliders[1].setBounds(area4.removeFromLeft(30));
    for (int i = 0; i < 6; i++){
        env1PointReleaseSliders[i].setBounds(area4.removeFromLeft(30));
    }
    env1Point0Sliders[2].setBounds(area4.removeFromLeft(30));
    
    smoothOnOffButton.setBounds(area7.removeFromLeft(80));
    env1SmoothAmountSlider.setBounds(area7.removeFromLeft(80));
    
    Rectangle<int> area6 = area7.withTrimmedTop(10).withTrimmedRight(200);
    
    linearButton.setBounds(area6.removeFromTop(20));
    exponentialButton.setBounds(area6.removeFromTop(20));
    anticlimacticButton.setBounds(area6.removeFromTop(20));
    sShapeButton.setBounds(area6.removeFromTop(20));
    envGemiddeldeSlider.setBounds(area6);
    
}

void EnvelopeEditor::updateToggleState (Button* button)
{
    //    auto state = button->getState();
    float startValueExp = 0.05;
    float startValueAC = 0.05;
    double ACWaardes[6];
    double ExpWaardes[6];
    for (int i = 0; i < 6; i++){
        startValueExp = startValueExp * pow(20.0,1.0/7.0);
        ExpWaardes[i] = startValueExp;
        int b;
        b = (i - 5) * -1;
        startValueAC = startValueAC * pow(20.0,1.0/7.0);
        ACWaardes[b] = 1 - startValueAC;
    }
    
    if (button == &linearButton){
        float startValue = 0.0;
        for (int i = 0; i < 6; i++){
            startValue = startValue + (1.0/7.0);
            env1PointSliders[i].setValue(startValue);
            env1PointReleaseSliders[i].setValue(1-startValue);
        }
    }
    else if (button == &exponentialButton){
        for (int i = 0; i < 6; i++){
            env1PointSliders[i].setValue(ExpWaardes[i]);
            env1PointReleaseSliders[i].setValue(1-ACWaardes[i]);
            
            
        }
    }
    else if (button == &anticlimacticButton){
        for (int i = 0; i < 6; i++){
            env1PointSliders[i].setValue(ACWaardes[i]);
            env1PointReleaseSliders[i].setValue(1-ExpWaardes[i]);
            
        }
    }
    else {
        for (int i = 0; i < 6; i++){
            env1PointSliders[i].setValue((ACWaardes[i] + ExpWaardes[i]) / 2);
            env1PointReleaseSliders[i].setValue( 1 - ((ACWaardes[i] + ExpWaardes[i]) / 2));
        }
    }
}

void EnvelopeEditor::sliderValueChanged(juce::Slider *slider){
    if (slider == &envGemiddeldeSlider) {
        float startValueExp = 0.01;
        float startValueAC = 0.01;
        double ACWaardes[6];
        double ExpWaardes[6];
        
        for (int i = 0; i < 6; i++){
            startValueExp = startValueExp * pow(100.0,1.0/7.0);
            ExpWaardes[i] = startValueExp;
            int b;
            b = (i - 5) * -1;
            startValueAC = startValueAC * pow(100.0,1.0/7.0);
            ACWaardes[b] = 1 - startValueAC;
        }
        for (int i = 0; i < 6; i++){
            env1PointSliders[i].setValue((ExpWaardes[i] * (1 - envGemiddeldeSlider.getValue())) + (ACWaardes[i] * envGemiddeldeSlider.getValue())) ;
            env1PointReleaseSliders[i].setValue(1 - ((ExpWaardes[i] * (envGemiddeldeSlider.getValue())) + (ACWaardes[i] * (1 - envGemiddeldeSlider.getValue()))) ) ;
            
        }
    }
}


void EnvelopeEditor::comboBoxChanged(ComboBox *comboBoxThatHasChanged){
    // set visible
    if (comboBoxThatHasChanged == &env1Menu){
        if (env1Menu.getSelectedId() == 2 || env1Menu.getSelectedId() == 3){
            env1AmpStartValue.setVisible(true);
        }
        else {
            env1AmpStartValue.setVisible(false);
        }
        
        if (env1Menu.getSelectedId() == 3){
            env1VerhoudingGemiddelde.setVisible(true);
        }
        else {
            env1VerhoudingGemiddelde.setVisible(false);
        }
        
        if (env1Menu.getSelectedId() == 5){
            for (int i = 0; i < 6; i++){
                env1PointSliders[i].setVisible(true);
            }
            smoothOnOffButton.setVisible(true);
            linearButton.setVisible(true);
            exponentialButton.setVisible(true);
            anticlimacticButton.setVisible(true);
            sShapeButton.setVisible(true);
            envGemiddeldeSlider.setVisible(true);
            env1Point0Sliders[0].setVisible(true);
            env1Point0Sliders[1].setVisible(true);
            env1SmoothAmountSlider.setVisible(true);
        }
        else {
            for (int i = 0; i < 6; i++){
                env1PointSliders[i].setVisible(false);
                env1SmoothAmountSlider.setVisible(false);
            }
            smoothOnOffButton.setVisible(false);
            linearButton.setVisible(false);
            exponentialButton.setVisible(false);
            anticlimacticButton.setVisible(false);
            sShapeButton.setVisible(false);
            envGemiddeldeSlider.setVisible(false);
            env1Point0Sliders[0].setVisible(false);
            env1Point0Sliders[1].setVisible(false);
        }
        
        
        if (env1Menu.getSelectedId() == 4){
            env1Point1XSlider.setVisible(true);
            env1Point2XSlider.setVisible(true);
            env1Point1YSlider.setVisible(true);
            env1Point2YSlider.setVisible(true);        }
        else{
            env1Point1XSlider.setVisible(false);
            env1Point2XSlider.setVisible(false);
            env1Point1YSlider.setVisible(false);
            env1Point2YSlider.setVisible(false);
        }
        
    }
}




 // PROCESSOR IDS
 // Env2
 //    string env2V1OnOffButtonsP [6] = {"onOffButtonID","onOffButtonLFOID","onOffButtonFilterID", "onOffButtonPanID", "onOffButtonFilterResonanceID", "onOffButtonReverbID"};
 //    string env2V2OnOffButtonsP [6] = {"onOffButtonV2ID","onOffButtonLFOV2ID","onOffButtonFilterV2ID", "onOffButtonPanV2ID", "onOffButtonFilterResonanceV2ID", "onOffButtonReverbV2ID"};
 //    string env2V3OnOffButtonsP [6] = {"onOffButtonV3ID","onOffButtonLFOV3ID","onOffButtonFilterV3ID", "onOffButtonPanV3ID", "onOffButtonFilterResonanceV3ID", "onOffButtonReverbV3ID"};
 //    string env2V1DopplerButtonsP [6] = {"dopplerButtonID","dopplerButtonLFOID", "dopplerButtonFilterID", "dopplerButtonPanID", "dopplerButtonFilterResonanceID", "dopplerButtonReverbID"};
 //    string env2V2DopplerButtonsP [6] = {"dopplerButtonV2ID","dopplerButtonLFOV2ID", "dopplerButtonFilterV2ID", "dopplerButtonPanV2ID", "dopplerButtonFilterResonanceV2ID", "dopplerButtonReverbV2ID"};
 //    string env2V3DopplerButtonsP [6] = {"dopplerButtonV3ID","dopplerButtonLFOV3ID", "dopplerButtonFilterV3ID", "dopplerButtonPanV3ID", "dopplerButtonFilterResonanceV3ID", "dopplerButtonReverbV3ID"};
 //    string env2V1AttackSlidersP [6] = {"env2AttackSliderID", "env2AttackSliderLFOID", "env2AttackSliderFilterID", "env2AttackSliderPanID", "env2AttackSliderFilterResonanceID", "env2AttackSliderReverbID"};
 //    string env2V2AttackSlidersP [6] = {"env2AttackSliderV2ID", "env2AttackSliderLFOV2ID", "env2AttackSliderFilterV2ID", "env2AttackSliderPanV2ID", "env2AttackSliderFilterResonanceV2ID", "env2AttackSliderReverbV2ID"};
 //    string env2V3AttackSlidersP [6] = {"env2AttackSliderV3ID", "env2AttackSliderLFOV3ID", "env2AttackSliderFilterV3ID", "env2AttackSliderPanV3ID", "env2AttackSliderFilterResonanceV3ID", "env2AttackSliderReverbV3ID"};
 //    string env2V1ReleaseSliderP [6] = {"env2ReleaseSliderID", "env2ReleaseSliderLFOID", "env2ReleaseSliderFilterID", "env2ReleaseSliderPanID", "env2ReleaseSliderFilterResonanceID", "env2ReleaseSliderReverbID"};
 //    string env2V2ReleaseSliderP [6] = {"env2ReleaseSliderV2ID", "env2ReleaseSliderLFOV2ID", "env2ReleaseSliderFilterV2ID", "env2ReleaseSliderPanV2ID", "env2ReleaseSliderFilterResonanceV2ID", "env2ReleaseSliderReverbV2ID"};
 //    string env2V3ReleaseSliderP [6] = {"env2ReleaseSliderV3ID", "env2ReleaseSliderLFOV3ID", "env2ReleaseSliderFilterV3ID", "env2ReleaseSliderPanV3ID", "env2ReleaseSliderFilterResonanceV3ID", "env2ReleaseSliderReverbV3ID"};
 
 
 double SynthVoice::setEnv1Parameters()
 {
 env[0].setAttackExp(env1Attack);
 env[0].setReleaseExp(env1Release);
 env[0].setAttackLin(env1Attack);
 env[0].setReleaseLin(env1Release);
 env[0].amplitudeStartValue = env1AmpStartValue;
 env[0].verhoudingGemiddelde = env1VerhoudingGemid;
 
 
 
 // check what value the envelope should be on (clean, distorted, enz)
 int signalValue;
 if (lfo1Selection == 0 && distortionOnOffToggle == 0){
 signalValue = 1;
 }
 else if (lfo1Selection == 0 && distortionOnOffToggle == 1) {
 signalValue = 2;
 }
 else {
 signalValue = 3;
 }
 
 // linear
 if (env1Selection == 0){
 return env[0].arLin(setSig(signalValue), env[0].trigger);
 //            return env[0].arLin(setChorus(), env[0].trigger);
 }
 // exponential
 if (env1Selection == 1){
 return env[0].arExp(setSig(signalValue), env[0].trigger);
 }
 // gemiddelde
 if (env1Selection == 2){
 return env[0].arGemiddelde(setSig(signalValue), env[0].trigger);
 }
 // 4 steps
 if (env1Selection == 3){
 env[0].setAttackLin4Steps(env1Attack, env1Point1Y, env1Point2Y, env1Point1X, env1Point2X);
 return env[0].arLin4Steps(setSig(signalValue), env[0].trigger);
 }
 // 12 steps
 if (env1Selection == 4){
 env[0].setReleaseLin5Steps(env1Attack, 0.8, 0.5, 0.3);
 env[0].setReleaseLin8Steps(env1Release, env1ReleasePoints[0], env1ReleasePoints[1], env1ReleasePoints[2], env1ReleasePoints[3], env1ReleasePoints[4], env1ReleasePoints[5]);
 env[0].setAttackLin8Steps(env1Attack, env1Points[0], env1Points[1], env1Points[2], env1Points[3], env1Points[4], env1Points[5]);
 
 
 return env[0].arLin12Steps(setSig(signalValue), env[0].trigger);
 //        double * arr = setSigStereo(signalValue);
 //
 //        double outvalue = env[0].arLin12Steps(arr[0], env[0].trigger);
 //        return outvalue;
 
 }
 // out of bounds
 else {
 std::cout << "envchoice out of bounds. Set to linear." << std::endl;
 return env[0].arLin(setSig(1), env[0].trigger);
 }
 }

 
 //==============================================================================
 double Envelopes::aLin(double input, int trigger)
 {
 //initialise
 if (trigger==1 && attackphase!=1 && holdphase!=1){
 holdcount=0;
 holdphase = 0;
 releasephase=0;
 attackphase=1;
 currentsample=0;
 amplitude = 0.0;
 }
 // go through attacks
 if (attackphase==1) {
 amplitude+=(1*attackLin);
 output=input*amplitude;
 }
 if (amplitude>=1) {
 amplitude=1;
 attackphase=0;
 holdphase = 1;
 }
 // if note is held down
 if (holdcount<holdtime && holdphase==1) {
 output=input;
 holdcount++;
 }
 if (holdcount==holdtime && trigger==1) {
 output=input;
 }
 if (holdcount==holdtime && trigger!=1) {
 holdphase=0;
 releasephase=1;
 }
 // release
 if (releasephase==1 && amplitude>0.) {
 amplitude-=(0);
 output = input*amplitude;
 }
 
 return output;
 }

*/
