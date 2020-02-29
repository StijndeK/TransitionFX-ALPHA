/*
  ==============================================================================

    Envelope2Editor.cpp
    Created: 6 Nov 2018 10:21:43am
    Author:  Stijn de Koning

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Envelope2Editor.h"

//==============================================================================
Envelope2Editor::Envelope2Editor(TransitionFxAudioProcessor& p, string (&OnOffButtons)[4], string (&DopplerButtons)[4], string (&AttackSliders)[4], string (&ReleaseSliders)[4], string (&DelaySliders)[4], string (&ForwardsButtons)[4], string (&LowRangeSliders)[4], string (&HighRangeSliders)[4], string (&SustainButtons)[4]) : processor(p)
{    
    // Names
    componentName = AttackSliders[0];
    componentName.replace(componentName.begin()+4, componentName.end()-4, " ");
    
    // Arrays
    string arrayOnOffs          [6];
    string arrayDopplers        [6];
    string arrayAttacks         [6];
    string arrayReleases        [6];
    string arrayDelays          [6];
    string arrayForwardsToggles [6];
    string arrayLowRanges       [6];
    string arrayHighRanges      [6];
    string arraySustains        [6];

    // Combobox
    env2Menu.addItem("Off", 1);
    env2Menu.addItem("Detune", 2);
    env2Menu.addItem("OscVoices Detune", 3);
    env2Menu.addItem("Pan", 4);
    env2Menu.addItem("Gain", 5);
    env2Menu.setJustificationType(Justification::centred);
    addAndMakeVisible(&env2Menu);
    env2Menu.addListener(this);
    env2Menu.setVisible(false);
    env2Menu.setSelectedId(1);
    env2MenuTree = new AudioProcessorValueTreeState::ComboBoxAttachment (processor.tree, "env2MenuID", env2Menu);
    
    // Shapes
    pathForwardsShape.addArrow (Line<float> (0,0,10,0), 1, 2, 2);
    pathShape.addArrow         (Line<float> (10,0,0,0), 1, 2, 2);

    // iterate through all options
    for (int i = 0; i < amountOfOptions; i++){
        // Names
        arrayOnOffs[i]          = OnOffButtons[i];
        arrayDopplers[i]        = DopplerButtons[i];
        arrayAttacks[i]         = AttackSliders[i];
        arrayReleases[i]        = ReleaseSliders[i];
        arrayDelays[i]          = DelaySliders[i];
        arrayForwardsToggles[i] = ForwardsButtons[i];
        arrayLowRanges[i]       = LowRangeSliders[i];
        arrayHighRanges[i]      = HighRangeSliders[i];
        arraySustains[i]        = SustainButtons[i];
                
        // Sliders
        env2AttackSlider[i].setSliderStyle(Slider::SliderStyle::LinearVertical);
        env2AttackSlider[i].setRange(0.0, 1.0);
        env2AttackSlider[i].setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
        addAndMakeVisible(&env2AttackSlider[i]);
        env2AttackSlider[i].addListener(this);
        env2AttackSliderTree[i] = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, arrayAttacks[i], env2AttackSlider[i]);
        
        env2ReleaseSlider[i].setSliderStyle(Slider::SliderStyle::LinearVertical);
        env2ReleaseSlider[i].setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
        addAndMakeVisible(&env2ReleaseSlider[i]);
        env2ReleaseSlider[i].addListener(this);
        env2ReleaseSliderTree[i] = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, arrayReleases[i], env2ReleaseSlider[i]);
        
        env2DelaySlider[i].setSliderStyle(Slider::SliderStyle::LinearVertical);
        env2DelaySlider[i].setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
        addAndMakeVisible(&env2DelaySlider[i]);
        env2DelaySlider[i].addListener(this);
        env2DelaySliderTree[i] = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, arrayDelays[i], env2DelaySlider[i]);
        
        env2RangeSlider[i].setSliderStyle(Slider::SliderStyle::ThreeValueVertical);
        env2RangeSlider[i].setTextBoxStyle(Slider::TextBoxBelow, true, 60, 20);
        env2RangeSlider[i].addListener(this);
        addAndMakeVisible(&env2RangeSlider[i]);
        
        env2LowRangeSlider[i].setSliderStyle(Slider::SliderStyle::LinearVertical);
        addAndMakeVisible(&env2LowRangeSlider[i]);
//        env2LowRangeSliderTree[i] = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, arrayLowRanges[i], env2LowRangeSlider[i]);
        env2LowRangeSlider[i].setVisible(false);
        env2HighRangeSlider[i].setSliderStyle(Slider::SliderStyle::LinearVertical);
        addAndMakeVisible(&env2HighRangeSlider[i]);
//        env2HighRangeSliderTree[i] = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, arrayHighRanges[i], env2HighRangeSlider[i]);
        env2HighRangeSlider[i].setVisible(false);
        
        // Buttons
        dopplerButton[i].setButtonText("Link to master");
        addAndMakeVisible(dopplerButton[i]);
        dopplerButtonTree[i] = new AudioProcessorValueTreeState::ButtonAttachment (processor.tree, arrayDopplers[i], dopplerButton[i]);
        
        onOffButton[i].setButtonText("On/Off");
        onOffButton[i].onClick = [&] { updateToggleState (&onOffButton[i]);   };
        addAndMakeVisible(onOffButton[i]);
        onOffButtonTree[i] = new AudioProcessorValueTreeState::ButtonAttachment (processor.tree, arrayOnOffs[i], onOffButton[i]);
        
        sustainButton[i].setButtonText("Sustain");
        addAndMakeVisible(sustainButton[i]);
        sustainButtonTree[i] = new AudioProcessorValueTreeState::ButtonAttachment (processor.tree, arraySustains[i], sustainButton[i]);
        
        // dont draw direction buttons gains
        if (i != 3) {
            env3BackwardsButtons[i].setShape(pathShape, true, true, true);
            addAndMakeVisible(env3BackwardsButtons[i]);
            
            env3ForwardsButtons[i].setShape(pathForwardsShape, true, true, true);
            addAndMakeVisible(env3ForwardsButtons[i]);
        }
        
        FBToggleButton[i].setButtonText("Forward / backward");
        addAndMakeVisible(FBToggleButton[i]);
        FBToggleButtonTree[i] = new AudioProcessorValueTreeState::ButtonAttachment (processor.tree, arrayForwardsToggles[i], FBToggleButton[i]);
    }
    
    env2RangeSlider[0].setRange(-3.0, 3.0,0.001); // detune
    env2RangeSlider[1].setRange(-3.0, 3.0,0.001); // OVD
    env2RangeSlider[2].setRange(-1.0, 1,0.001); // Pan
    env2RangeSlider[3].setRange(-78.0, 0,0.001); // Gain

    dopplerButton       [0].onClick = [this] { updateToggleState (&dopplerButton        [0]);   };
    dopplerButton       [1].onClick = [this] { updateToggleState (&dopplerButton        [1]);   };
    dopplerButton       [2].onClick = [this] { updateToggleState (&dopplerButton        [2]);   };
    dopplerButton       [3].onClick = [this] { updateToggleState (&dopplerButton        [2]);   };
    
    sustainButton       [0].onClick = [this] { updateToggleState (&sustainButton        [0]);   };
    sustainButton       [1].onClick = [this] { updateToggleState (&sustainButton        [1]);   };
    sustainButton       [2].onClick = [this] { updateToggleState (&sustainButton        [2]);   };
    sustainButton       [3].onClick = [this] { updateToggleState (&sustainButton        [3]);   };
    
    env3BackwardsButtons[0].onClick = [this] { updateToggleState (&env3BackwardsButtons [0]);   };
    env3BackwardsButtons[1].onClick = [this] { updateToggleState (&env3BackwardsButtons [1]);   };
    env3BackwardsButtons[2].onClick = [this] { updateToggleState (&env3BackwardsButtons [2]);   };
    env3BackwardsButtons[3].onClick = [this] { updateToggleState (&env3BackwardsButtons [2]);   };
    
    env3ForwardsButtons [0].onClick = [this] { updateToggleState (&env3ForwardsButtons  [0]);   };
    env3ForwardsButtons [1].onClick = [this] { updateToggleState (&env3ForwardsButtons  [1]);   };
    env3ForwardsButtons [2].onClick = [this] { updateToggleState (&env3ForwardsButtons  [2]);   };
    env3ForwardsButtons [3].onClick = [this] { updateToggleState (&env3ForwardsButtons  [2]);   };


    // INITIIALISE
    // TODO: get these values automatically
    left = 5;
    right = 585;
    middlePoint = (right - left) / 2;
    
    // Check which gui is selected, and set the gui
    selectGUI();
    
    for (int i = 0; i < amountOfOptions; i++) {
        // set selected buttons
        env3BackwardsButtons[i].setColours(Colours::darkgrey, Colours::blue, Colours::black);
        env3BackwardsButtons[i].setToggleState(false, juce::NotificationType::dontSendNotification);
        env3ForwardsButtons[i].setColours(Colours::deepskyblue, Colours::blue, Colours::black);
        env3ForwardsButtons[i].setToggleState(true, juce::NotificationType::dontSendNotification);
        
        // set range slider values
        env2RangeSlider[i].setMinValue(env2LowRangeSlider[i].getValue());
        env2RangeSlider[i].setMaxValue(env2HighRangeSlider[i].getValue());
    }
}

Envelope2Editor::~Envelope2Editor()
{
    env2MenuTree.~ScopedPointer();
    for (int i = 0; i < amountOfOptions; i++){
        onOffButtonTree[i].~ScopedPointer();
        dopplerButtonTree[i].~ScopedPointer();
        env2AttackSliderTree[i].~ScopedPointer();
        env2ReleaseSliderTree[i].~ScopedPointer();
        env2DelaySliderTree[i].~ScopedPointer();
        env2LowRangeSliderTree[i].~ScopedPointer();
        env2HighRangeSliderTree[i].~ScopedPointer();
        FBToggleButtonTree[i].~ScopedPointer();
        sustainButtonTree[i].~ScopedPointer();
    }
}

void Envelope2Editor::paint (Graphics& g)
{
    g.fillAll (Colour(processor.red, processor.green, processor.blue));   // clear the background
    
    g.setColour (Colours::white);
    g.setFont (14.0f);
    
    // ENV DRAWING
    g.setColour (Colours::grey);
    Rectangle<int> area1 = getLocalBounds();
    Rectangle<int> drawArea2 = area1.removeFromBottom(120);
    g.drawRect(drawArea2,2);

    // impact line
    g.setColour (Colours::white);
    g.drawLine(middlePoint, drawArea2.getBottom(), middlePoint, drawArea2.getY());
    
    Rectangle<int> drawArea = drawArea2.reduced(4);
    g.setColour (Colours::white);
    myPath.startNewSubPath(drawArea.getX(), drawArea.getBottom());
    // delay
    myPath.lineTo(startAttack, drawArea.getBottom());
    // attack
    myPath.lineTo(endAttack, drawArea.getY());
    // middle
    myPath.lineTo(middlePoint, drawArea.getY());
    // release
    if (endPoint == 1) { // sustain
        myPath.lineTo(drawArea.getWidth(), drawArea.getY());
    }                    // release
    else {
        myPath.lineTo(endRelease, drawArea.getBottom());
    }
    g.strokePath(myPath, PathStrokeType (2.0f));
    
    // SLIDER NAMES
    Rectangle<int> areaSliderNames = getLocalBounds().reduced(40).withTrimmedLeft(200).withTrimmedTop(25).removeFromTop(20);
    g.drawText ("Delay", areaSliderNames.removeFromLeft(80), Justification::centredTop);
    g.drawText ("Attack", areaSliderNames.removeFromLeft(80), Justification::centredTop);
    g.drawText ("Release", areaSliderNames.removeFromLeft(80), Justification::centredTop);
    g.drawText ("Range", areaSliderNames.removeFromLeft(80), Justification::centredTop);
}

void Envelope2Editor::resized()
{
    Rectangle<int> area = getLocalBounds().reduced(40).withTrimmedLeft(120);
    
    // drawMenu
    env2Menu.setBounds(area.removeFromTop(20));
    
    // forwards / backwards
    Rectangle<int> area8 = getLocalBounds().removeFromTop(40).withTrimmedLeft(180).removeFromLeft(120);
    Rectangle<int> area10 = area8.removeFromRight(60);
    Rectangle<int> area9 = area8.removeFromRight(60);
    for (int i = 0; i < amountOfOptions; i++){
            env3ForwardsButtons[i].setBounds(area10);
            env3BackwardsButtons[i].setBounds(area9);
    }
    
    // sliders and buttons
    Rectangle<int> area2 = area.removeFromTop(120).withTrimmedTop(20);
    Rectangle<int> area6 = area2.removeFromLeft(80);
    int removeFromTopValue = area6.getHeight() / 3;
    Rectangle<int> area6a = area6.removeFromTop(removeFromTopValue);
    Rectangle<int> area6b = area6.removeFromTop(removeFromTopValue);
    Rectangle<int> area6c = area6;
    Rectangle<int> area3 = area2.removeFromLeft(80);
    Rectangle<int> area4 = area2.removeFromLeft(80);
    Rectangle<int> area5 = area2.removeFromLeft(80);
    Rectangle<int> area12 = area2.removeFromLeft(80);
    
    for (int i = 0; i < amountOfOptions; i++){
        env2DelaySlider[i].setBounds(area3);
        env2AttackSlider[i].setBounds(area4);
        env2ReleaseSlider[i].setBounds(area5);
        onOffButton[i].setBounds(area6a);
        dopplerButton[i].setBounds(area6b);
        sustainButton[i].setBounds(area6c);
        env2RangeSlider[i].setBounds(area12);
    }
}

//==============================================================================
void Envelope2Editor::selectGUI() {
    for (int i = 0; i < amountOfOptions; i++) {
        onOffButton[i].setVisible(false);
        dopplerButton[i].setVisible(false);
        env2AttackSlider[i].setVisible(false);
        env2ReleaseSlider[i].setVisible(false);
        env2DelaySlider[i].setVisible(false);
        env2RangeSlider[i].setVisible(false);
        env3BackwardsButtons[i].setVisible(false);
        env3ForwardsButtons[i].setVisible(false);
        sustainButton[i].setVisible(false);
    }
    // Check which gui is selected, and set the gui
    for (int ii = 0; ii < amountOfOptions; ii++) {
        if (ii+2 == env2Menu.getSelectedId()) {

            sustainButton[ii].setVisible(true);
            onOffButton[ii].setVisible(true);
            dopplerButton[ii].setVisible(true);
            env2AttackSlider[ii].setVisible(true);
            env2ReleaseSlider[ii].setVisible(true);
            env2DelaySlider[ii].setVisible(true);
            // env2RangeSlider[ii].setVisible(true);
            env3BackwardsButtons[ii].setVisible(true);
            env3ForwardsButtons[ii].setVisible(true);
            // set correct env drawing values
            startAttack = (env2DelaySlider[ii].getValue() * (middlePoint - left)) + left;
            endAttack = (env2AttackSlider[ii].getValue() * (middlePoint - left)) + left;
            endRelease = ((1 - env2ReleaseSlider[ii].getValue()) * (middlePoint - right)) + right;
            endPoint = (sustainButton[ii].getToggleState() == 1) ? 1 : 0;
            myPath.clear();
            repaint();
        }
    }
}

//==============================================================================
void Envelope2Editor::updateToggleState (Button* button)
{
    for (int i = 0; i < amountOfOptions; i ++) {
        if (button == &dopplerButton[i]) {
            // check if toggle is on or off
            if (dopplerOn == 0){
                dopplerOn = dopplerOn + 1;
            }
            else {
                dopplerOn = dopplerOn - 1;
            }
            
            // draw sliders based on toggle on or off
            if (dopplerOn == 1) {
                env2AttackSlider[i].setVisible(false);
                env2ReleaseSlider[i].setVisible(false);
                env2DelaySlider[i].setVisible(false);
            }
            else {
                env2AttackSlider[i].setVisible(true);
                env2ReleaseSlider[i].setVisible(true);
                env2DelaySlider[i].setVisible(true);
            }
        }
        
        // ----- draw envelope shapes ------
        if (button == &sustainButton[i]) {
            if (sustainButton[i].getToggleState() == 1) {
                myPath.clear();
                endPoint = 1;
            }
            else {
                myPath.clear();
                endPoint = 0;
            }
            repaint();
        }
        
        // set button colors based on selection
        if (button == &env3BackwardsButtons[i]) {
            if (backwardsOn == 0){
                backwardsOn = backwardsOn + 1;
                forwardsOn = forwardsOn - 1;
            }
            if (backwardsOn == 1){
                env3BackwardsButtons[i].setColours(Colours::deepskyblue, Colours::blue, Colours::black);
                env3BackwardsButtons[i].setToggleState(true, juce::NotificationType::dontSendNotification);
                env3ForwardsButtons[i].setColours(Colours::darkgrey, Colours::blue, Colours::black);
                env3ForwardsButtons[i].setToggleState(false, juce::NotificationType::dontSendNotification);
                FBToggleButton[i].setToggleState(false, NotificationType::sendNotificationSync);
            }
        }
        if (button == &env3ForwardsButtons[i]) {
            if (forwardsOn == 0){
                forwardsOn = forwardsOn + 1;
                backwardsOn = backwardsOn - 1;
            }
            if (forwardsOn == 1){
                env3BackwardsButtons[i].setColours(Colours::darkgrey, Colours::blue, Colours::black);
                env3BackwardsButtons[i].setToggleState(false, juce::NotificationType::dontSendNotification);
                env3ForwardsButtons[i].setColours(Colours::deepskyblue, Colours::blue, Colours::black);
                env3ForwardsButtons[i].setToggleState(true, juce::NotificationType::dontSendNotification);
                FBToggleButton[i].setToggleState(true, NotificationType::sendNotificationSync);
            }
        }
    }
}

void Envelope2Editor::comboBoxChanged(ComboBox *comboBoxThatHasChanged)
{
    // select gui based on the menu selection
    if (comboBoxThatHasChanged == &env2Menu) {
        // select gui
        selectGUI();
    }
}

void Envelope2Editor::sliderValueChanged(juce::Slider *slider)
{
    for (int i = 0; i < amountOfOptions; i++) {
        // set slider ranges based on 2 range slider
        if (slider == &env2RangeSlider[i]) {
            env2LowRangeSlider[i].setValue(env2RangeSlider[i].getMinValue());
            env2HighRangeSlider[i].setValue(env2RangeSlider[i].getMaxValue());
            std::cout << env2LowRangeSlider[i].getValue() << " <-low | high-> " << env2HighRangeSlider[i].getValue() << std::endl;
        }
        
        // check delay and attack cant pass each other
        if (slider == &env2DelaySlider[i] || slider == &env2AttackSlider[i]) {
            if (env2DelaySlider[i].getValue() > env2AttackSlider[i].getValue()) {
                env2DelaySlider[i].setValue(env2AttackSlider[i].getValue() - 0.01);
            }
        }
        
        // ----- draw envelope shapes ------
        // delay
        if (slider == &env2DelaySlider[i]) {
            // set new values
            startAttack = (env2DelaySlider[i].getValue() * (middlePoint - left)) + left;
            // clear old path
            myPath.clear();
            // repaint
            repaint();
        }
        // attack
        if (slider == &env2AttackSlider[i]) {
            // set new values
            endAttack = (env2AttackSlider[i].getValue() * (middlePoint - left)) + left;
            // clear old path
            myPath.clear();
            // repaint
            repaint();
        }
        // release
        if (slider == &env2ReleaseSlider[i]) {
            // set new values
            endRelease = ((1 - env2ReleaseSlider[i].getValue()) * (middlePoint - right)) + right;
            // clear old path
            myPath.clear();
            // repaint
            repaint();
        }
    }
}
