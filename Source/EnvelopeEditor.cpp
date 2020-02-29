/*
  ==============================================================================

    EnvelopeEditor.cpp
    Created: 31 Oct 2018 2:08:55pm
    Author:  Stijn de Koning

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "EnvelopeEditor.h"

//==============================================================================
EnvelopeEditor::EnvelopeEditor(TransitionFxAudioProcessor& p) : processor(p)
{
    // get IDs
    string pointSliderIDs[6];
    string pointReleaseSliderIDs[6];
    for (int i = 0; i < 6; i++) {
        pointSliderIDs[i] = processor.env1PointSliderIDs[i];
        pointReleaseSliderIDs[i] = processor.env1PointReleaseSliderIDs[i];
    }

    // Combobox
    env1Menu.addItem("Linear", 1);
    env1Menu.addItem("Exponential", 2);
    env1Menu.addItem("Gemiddelde", 3);
    env1Menu.addItem("4-steps", 4);
    env1Menu.addItem("8-steps, fixed time(X) positions", 5);
    env1Menu.setJustificationType(Justification::centred);
    env1MenuTree = new AudioProcessorValueTreeState::ComboBoxAttachment (processor.tree, "env1MenuID", env1Menu);
    
    // Sliders
    for (int i = 0; i < 6; i++){
        env1PointSliders[i].setSliderStyle(Slider::SliderStyle::LinearVertical);
        env1PointSliders[i].setTextBoxStyle(Slider::NoTextBox, false, 30, 20);
        addAndMakeVisible(&env1PointSliders[i]);
        env1PointSliders[i].addListener(this);
        env1PointSlidersTree[i] = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, pointSliderIDs[i], env1PointSliders[i]);
        
        env1PointReleaseSliders[i].setSliderStyle(Slider::SliderStyle::LinearVertical);
        env1PointReleaseSliders[i].setTextBoxStyle(Slider::NoTextBox, false, 30, 20);
        addAndMakeVisible(&env1PointReleaseSliders[i]);
        env1PointReleaseSliders[i].addListener(this);
        env1PointReleaseSlidersTree[i] = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, pointReleaseSliderIDs[i], env1PointReleaseSliders[i]);
    }
    
    // slider voor het zetten van 8 step envelope
    envGemiddeldeSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    envGemiddeldeSlider.setTextBoxStyle(Slider::NoTextBox, true, 60, 20);
    addAndMakeVisible(&envGemiddeldeSlider);
    envGemiddeldeSlider.addListener(this);
    envGemiddeldeSlider.setRange(0, 1);
    
    // slider voor amount of smooth
    env1SmoothAmountSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    env1SmoothAmountSlider.setTextBoxStyle(Slider::NoTextBox, true, 60, 20);
    addAndMakeVisible(&env1SmoothAmountSlider);
    env1SmoothAmountSliderTree = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "env1SmoothAmountSliderID", env1SmoothAmountSlider);

    // slider voor verhouding attack en release
    env1AttackReleaseRatioSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    env1AttackReleaseRatioSlider.setTextBoxStyle(Slider::NoTextBox, true , 60, 20);
    addAndMakeVisible(&env1AttackReleaseRatioSlider);
    env1AttackReleaseRatioSlider.addListener(this);
    env1AttackReleaseRatioSliderTree = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, "env1AttackReleaseRatioSliderID", env1AttackReleaseRatioSlider);
    env1AttackReleaseRatioSlider.setSkewFactor(1.5);
    
    // sliders op 0 en 1 voor visuals
    NormalisableRange<double> fixed0Range (0, 1, 10);
    NormalisableRange<double> fixed1Range (0, 1, 2);
    for (int i = 0; i < 3; i++){
        env1Point0Sliders[i].setSliderStyle(Slider::SliderStyle::LinearVertical);
        env1Point0Sliders[i].setTextBoxStyle(Slider::NoTextBox, false, 60, 20);
        addAndMakeVisible(&env1Point0Sliders[i]);
        if (i == 1) {
            env1Point0Sliders[i].setNormalisableRange(fixed1Range);
        }
        else {
            env1Point0Sliders[i].setNormalisableRange(fixed0Range);
        }
    }
    env1Point0Sliders[0].setValue(0);
    env1Point0Sliders[1].setValue(1);
    env1Point0Sliders[2].setValue(0);
    
    env1Point0Sliders[1].addListener(this);
    
    // Buttons
    addAndMakeVisible(&smoothOnOffButton);
    smoothOnOffButtonTree = new AudioProcessorValueTreeState::ButtonAttachment (processor.tree, "env1SmoothOnOffButtonID", smoothOnOffButton);
    
    addAndMakeVisible(&linearButton);
    linearButton.onClick        = [this] { updateToggleState (&linearButton);   };
    addAndMakeVisible(&exponentialButton);
    exponentialButton.onClick   = [this] { updateToggleState (&exponentialButton);   };
    addAndMakeVisible(&anticlimacticButton);
    anticlimacticButton.onClick = [this] { updateToggleState (&anticlimacticButton);   };
    addAndMakeVisible(&sShapeButton);
    sShapeButton.onClick        = [this] { updateToggleState (&sShapeButton);   };
    
    // Initialise
    for (int i = 0; i < 6; i++){
        env1PointAttackSliderValues[i] = *processor.tree.getRawParameterValue(processor.env1PointSliderIDs[i]);
        env1PointReleaseSliderValues[i] = *processor.tree.getRawParameterValue(processor.env1PointReleaseSliderIDs[i]);
        env1PointSliders[i].setValue(*processor.tree.getRawParameterValue(processor.env1PointSliderIDs[i]));
        env1PointReleaseSliders[i].setValue(*processor.tree.getRawParameterValue(processor.env1PointReleaseSliderIDs[i]));
    }
    
    sliderValueChanged(env1PointReleaseSliders);
}

EnvelopeEditor::~EnvelopeEditor()
{
    env1MenuTree.~ScopedPointer();
    smoothOnOffButtonTree.~ScopedPointer();
    env1SmoothAmountSliderTree.~ScopedPointer();
    for (int i = 0; i < 6; i ++) {
        env1PointReleaseSlidersTree[i].~ScopedPointer();
        env1PointSlidersTree[i].~ScopedPointer();
    }
}

void EnvelopeEditor::paint (Graphics& g)
{
    g.fillAll (Colours::black);
    
    getLookAndFeel().setColour(Slider::thumbColourId, Colours::deepskyblue);
    env1Point0Sliders[1].setColour(Slider::thumbColourId, Colours::royalblue);
    
    Rectangle<int> area = getLocalBounds().reduced(10);

    g.setColour (Colours::white);
    g.setFont (16.0f);
    g.drawText ("Envelope", area, Justification::centredTop);
    
    g.setColour (Colours::grey);
    Rectangle<int> areaRR = area.withTrimmedTop(25);
    g.drawRoundedRectangle(areaRR.getX(), areaRR.getY(), areaRR.getWidth(), areaRR.getHeight(), 8, 2.0);
    g.setColour(Colour(processor.red, processor.green, processor.blue));
    g.fillRoundedRectangle(areaRR.getX(), areaRR.getY(), areaRR.getWidth(), areaRR.getHeight(), 8);
    
    g.setColour (Colours::white);
    g.setFont (14.0f);
    Rectangle<int> area2 = getLocalBounds().reduced(40);
    Rectangle<int> area3 = area2.removeFromBottom(area2.getHeight() / 2);
    
    g.drawText ("smooth", area3.removeFromLeft(80).removeFromTop(40), Justification::centredBottom);
    g.drawText ("Amount", area3.removeFromLeft(80).removeFromTop(40), Justification::centredBottom);
    
    Rectangle<int> area4 = area3.removeFromRight(200).removeFromTop(40);
    g.drawText ("Release offset", area4, Justification::centredBottom);
}

void EnvelopeEditor::resized()
{
    Rectangle<int> area = getLocalBounds().reduced(40);
    // top menu
//    env1Menu.setBounds(area.removeFromTop(20));
    Rectangle<int> area3 = area;
    Rectangle<int> area4 = area3.removeFromTop(area3.getHeight() / 2);
    Rectangle<int> area7 = area3;
    
    int pointSliderLeftValue = area3.getWidth() / 15;

    // point sliders
    env1Point0Sliders[0].setBounds(area4.removeFromLeft(pointSliderLeftValue));
    for (int i = 0; i < 6; i++){
        env1PointSliders[i].setBounds(area4.removeFromLeft(pointSliderLeftValue));
    }
    env1Point0Sliders[1].setBounds(area4.removeFromLeft(pointSliderLeftValue));
    for (int i = 0; i < 6; i++){
        env1PointReleaseSliders[i].setBounds(area4.removeFromLeft(pointSliderLeftValue));
    }
    env1Point0Sliders[2].setBounds(area4.removeFromLeft(pointSliderLeftValue));

    // smoothing
    smoothOnOffButton.setBounds(area7.removeFromLeft(80).withTrimmedTop(40));
    env1SmoothAmountSlider.setBounds(area7.removeFromLeft(80).withTrimmedTop(40));

    Rectangle<int> area6 = area7.withTrimmedTop(10).withTrimmedRight(200);

    // presets
    linearButton.setBounds(area6.removeFromTop(20));
    exponentialButton.setBounds(area6.removeFromTop(20));
    anticlimacticButton.setBounds(area6.removeFromTop(20));
//    sShapeButton.setBounds(area6.removeFromTop(20));
    envGemiddeldeSlider.setBounds(area6);
    
    // attack release ratio
    env1AttackReleaseRatioSlider.setBounds(area7.removeFromRight(200).withTrimmedTop(40));
}

//==============================================================================
void EnvelopeEditor::updateToggleState (Button* button)
{
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
            env1PointAttackSliderValues[i] = startValue;
            env1PointReleaseSliderValues[i] = 1 - startValue;
        }
    }
    else if (button == &exponentialButton){
        for (int i = 0; i < 6; i++){
            env1PointAttackSliderValues[i] = ExpWaardes[i];
            env1PointReleaseSliderValues[i] = 1-ACWaardes[i];
        }
    }
    else if (button == &anticlimacticButton){
        for (int i = 0; i < 6; i++){
            env1PointAttackSliderValues[i] = ACWaardes[i];
            env1PointReleaseSliderValues[i] = 1 - ExpWaardes[i];
        }
    }
    else {
        for (int i = 0; i < 6; i++){
            env1PointAttackSliderValues[i] = (ACWaardes[i] + ExpWaardes[i]) / 2;
            env1PointReleaseSliderValues[i] = 1 - ((ACWaardes[i] + ExpWaardes[i]) / 2);
        }
    }
    
    // set values to sliders
    for (int i = 0; i < 6; i++){
        arRatio = env1AttackReleaseRatioSlider.getValue();
        env1PointSliders[i].setValue(env1PointAttackSliderValues[i]);
        env1PointReleaseSliders[i].setValue(env1PointReleaseSliderValues[i] * arRatio);
        std::cout << "release values: " << env1PointReleaseSliderValues[i] << std::endl;
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
            env1PointAttackSliderValues[i] = ((ExpWaardes[i] * (1 - envGemiddeldeSlider.getValue())) + (ACWaardes[i] * envGemiddeldeSlider.getValue()));
            env1PointReleaseSliderValues[i] = 1 - ((ExpWaardes[i] * (envGemiddeldeSlider.getValue())) + (ACWaardes[i] * (1 - envGemiddeldeSlider.getValue())));
            
            arRatio = env1AttackReleaseRatioSlider.getValue();
            env1PointSliders[i].setValue(env1PointAttackSliderValues[i]);
            env1PointReleaseSliders[i].setValue(env1PointReleaseSliderValues[i] * arRatio);
        }
    }
    if (slider == &env1AttackReleaseRatioSlider) {
        arRatio = env1AttackReleaseRatioSlider.getValue();
        for (int i = 0; i < 6; i++){
            env1PointReleaseSliders[i].setValue(env1PointReleaseSliderValues[i] * arRatio);
        }
    }
    
    // make sure the point1 slider cant move
    if (slider == &env1Point0Sliders[1]) {
        env1Point0Sliders[1].setValue(1);
    }
    
    // make sure point sliders cant pass each other
    for (int i = 0; i < 4; i ++) {
        if (slider == &env1PointSliders[i+1]) {
            if (env1PointSliders[i+1].getValue() >= env1PointSliders[i+2].getValue()) {
                env1PointSliders[i+1].setValue(env1PointSliders[i+2].getValue() - 0.001);
            }
            if (env1PointSliders[i+1].getValue() <= env1PointSliders[i].getValue()) {
                env1PointSliders[i+1].setValue(env1PointSliders[i].getValue() + 0.001);
            }
        }
        if (slider == &env1PointReleaseSliders[i+1]) {
            if (env1PointReleaseSliders[i+1].getValue() <= env1PointReleaseSliders[i+2].getValue()) {
                env1PointReleaseSliders[i+1].setValue(env1PointReleaseSliders[i+2].getValue() - 0.001);
            }
            if (env1PointReleaseSliders[i+1].getValue() >= env1PointReleaseSliders[i].getValue()) {
                env1PointReleaseSliders[i+1].setValue(env1PointReleaseSliders[i].getValue() + 0.001);
            }
        }

    }
    if (slider == &env1PointSliders[0]) {
        if (env1PointSliders[0].getValue() >= env1PointSliders[ 1].getValue()) {
            env1PointSliders[0].setValue(env1PointSliders[1].getValue() - 0.001);
        }
    }
    if (slider == &env1PointSliders[5]) {
        if (env1PointSliders[5].getValue() <= env1PointSliders[4].getValue()) {
            env1PointSliders[5].setValue(env1PointSliders[4].getValue() + 0.001);
        }
    }
    if (slider == &env1PointReleaseSliders[0]) {
        if (env1PointReleaseSliders[0].getValue() <= env1PointReleaseSliders[ 1].getValue()) {
            env1PointReleaseSliders[0].setValue(env1PointReleaseSliders[1].getValue() - 0.001);
        }
    }
    if (slider == &env1PointReleaseSliders[5]) {
        if (env1PointReleaseSliders[5].getValue() >= env1PointReleaseSliders[4].getValue()) {
            env1PointReleaseSliders[5].setValue(env1PointReleaseSliders[4].getValue() + 0.001);
        }
    }
}





