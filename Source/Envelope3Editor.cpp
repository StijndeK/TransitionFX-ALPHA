/*
  ==============================================================================

    Envelope3Editor.cpp
    Created: 27 Nov 2018 10:55:24am
    Author:  Stijn de Koning

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Envelope3Editor.h"

//==============================================================================
Envelope3Editor::Envelope3Editor(TransitionFxAudioProcessor& p, string (&OnOffButtons)[6], string (&DopplerButtons)[6], string (&AttackSliders)[6], string (&ReleaseSliders)[6], string (&DelaySliders)[6], string (&ForwardsButtons)[6], string (&LowRanges)[6], string (&HighRanges)[6], string (&SustainButtons)[6]) : processor(p)
{
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
    env3Menu.addItem("Off", 1);
    env3Menu.addItem("LFO Rate", 2);
    env3Menu.addItem("Filter CutOff", 3);
    env3Menu.addItem("Pan", 4);
    env3Menu.addItem("Filter Resonance", 5);
    env3Menu.addItem("Reverb Dry/Wet", 6);
    env3Menu.addItem("Gain", 7);
    env3Menu.addItem("Detune", 8);
    env3Menu.setJustificationType(Justification::centred);
    addAndMakeVisible(&env3Menu);
    env3Menu.addListener(this);
    env3Menu.setSelectedId(2);
    env3Menu.setVisible(false);
    
    env3FilterMenu.addItem("cutoff", 1);
    env3FilterMenu.addItem("resonance", 2);
    env3FilterMenu.setJustificationType(Justification::centred);
    addAndMakeVisible(&env3FilterMenu);
    env3FilterMenu.addListener(this);
    env3FilterMenu.setSelectedId(1);
    
    // Shapes
    pathForwardsShape.addArrow(Line<float> (0,0,10,0), 1, 2, 2);
    pathShape.addArrow(Line<float> (10,0,0,0), 1, 2, 2);
    
    for (int i = 0; i < amountOfOptions; i++){
        // Names
        arrayOnOffs[i]          = OnOffButtons[i];
        arrayDopplers[i]        = DopplerButtons[i];
        arrayAttacks[i]         = AttackSliders[i];
        arrayReleases[i]        = ReleaseSliders[i];
        arrayDelays[i]          = DelaySliders[i];
        arrayForwardsToggles[i] = ForwardsButtons[i];
        arrayLowRanges[i]       = LowRanges[i];
        arrayHighRanges[i]      = HighRanges[i];
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
        
        env2RangeSlider[i].setSliderStyle(Slider::SliderStyle::TwoValueVertical);
        env2RangeSlider[i].setTextBoxStyle(Slider::TextBoxBelow, true, 60, 20);
        env2RangeSlider[i].addListener(this);
        addAndMakeVisible(&env2RangeSlider[i]);
        
        env2LowRangeSlider[i].setSliderStyle(Slider::SliderStyle::LinearVertical);
        addAndMakeVisible(&env2LowRangeSlider[i]);
        env2LowRangeSliderTree[i] = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, arrayLowRanges[i], env2LowRangeSlider[i]);
        env2LowRangeSlider[i].setVisible(false);
        env2HighRangeSlider[i].setSliderStyle(Slider::SliderStyle::LinearVertical);
        addAndMakeVisible(&env2HighRangeSlider[i]);
        env2HighRangeSliderTree[i] = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, arrayHighRanges[i], env2HighRangeSlider[i]);
        env2HighRangeSlider[i].setVisible(false);
        
        // Buttons
        dopplerButton[i].setButtonText("Link to Master");
        addAndMakeVisible(dopplerButton[i]);
        dopplerButtonTree[i] = new AudioProcessorValueTreeState::ButtonAttachment (processor.tree, arrayDopplers[i], dopplerButton[i]);
        
        onOffButton[i].setButtonText("On/Off");
        addAndMakeVisible(onOffButton[i]);
        onOffButtonTree[i] = new AudioProcessorValueTreeState::ButtonAttachment (processor.tree, arrayOnOffs[i], onOffButton[i]);
        
        sustainButton[i].setButtonText("Sustain");
        addAndMakeVisible(sustainButton[i]);
        sustainButtonTree[i] = new AudioProcessorValueTreeState::ButtonAttachment (processor.tree, arraySustains[i], sustainButton[i]);
        
        // dont draw direction buttons for filter and reverb
        if (i != 1 && i != 3 && i != 4) {
            env3BackwardsButtons[i].setShape(pathShape, true, true, true);
            addAndMakeVisible(env3BackwardsButtons[i]);
            
            env3ForwardsButtons[i].setShape(pathForwardsShape, true, true, true);
            addAndMakeVisible(env3ForwardsButtons[i]);
        }
        
        FBToggleButton[i].setButtonText("Forward / backward");
        addAndMakeVisible(FBToggleButton[i]);
        FBToggleButtonTree[i] = new AudioProcessorValueTreeState::ButtonAttachment (processor.tree, arrayForwardsToggles[i], FBToggleButton[i]);
    }
    
    // set ranges
    env2RangeSlider[0].setRange(0.0, 20.0, 0.001); // lfo rate
    env2RangeSlider[0].setMaxValue(15.0);
    env2RangeSlider[0].setMinValue(1.0);

    // Buttons
    dopplerButton[0].onClick = [this] { updateToggleState (&dopplerButton[0]);   };
    dopplerButton[1].onClick = [this] { updateToggleState (&dopplerButton[1]);   };
    dopplerButton[2].onClick = [this] { updateToggleState (&dopplerButton[2]);   };
    dopplerButton[3].onClick = [this] { updateToggleState (&dopplerButton[3]);   };
    dopplerButton[4].onClick = [this] { updateToggleState (&dopplerButton[4]);   };
    dopplerButton[5].onClick = [this] { updateToggleState (&dopplerButton[5]);   };
    
    sustainButton[0].onClick = [this] { updateToggleState (&sustainButton[0]);   };
    sustainButton[1].onClick = [this] { updateToggleState (&sustainButton[1]);   };
    sustainButton[2].onClick = [this] { updateToggleState (&sustainButton[2]);   };
    sustainButton[3].onClick = [this] { updateToggleState (&sustainButton[3]);   };
    sustainButton[4].onClick = [this] { updateToggleState (&sustainButton[4]);   };
    sustainButton[5].onClick = [this] { updateToggleState (&sustainButton[5]);   };
    
    env3BackwardsButtons[0].onClick = [this] { updateToggleState (&env3BackwardsButtons[0]);   };
    env3BackwardsButtons[1].onClick = [this] { updateToggleState (&env3BackwardsButtons[1]);   };
    env3BackwardsButtons[2].onClick = [this] { updateToggleState (&env3BackwardsButtons[2]);   };
    env3BackwardsButtons[3].onClick = [this] { updateToggleState (&env3BackwardsButtons[3]);   };
    env3BackwardsButtons[4].onClick = [this] { updateToggleState (&env3BackwardsButtons[4]);   };
    env3BackwardsButtons[5].onClick = [this] { updateToggleState (&env3BackwardsButtons[5]);   };
    
    env3ForwardsButtons[0].onClick = [this] { updateToggleState (&env3ForwardsButtons[0]);   };
    env3ForwardsButtons[1].onClick = [this] { updateToggleState (&env3ForwardsButtons[1]);   };
    env3ForwardsButtons[2].onClick = [this] { updateToggleState (&env3ForwardsButtons[2]);   };
    env3ForwardsButtons[3].onClick = [this] { updateToggleState (&env3ForwardsButtons[3]);   };
    env3ForwardsButtons[4].onClick = [this] { updateToggleState (&env3ForwardsButtons[4]);   };
    env3ForwardsButtons[5].onClick = [this] { updateToggleState (&env3ForwardsButtons[5]);   };
    
    // Initialise
    selectGUI();
    
    // TODO: get these values automatically
    left = 5;
    right = 585;
    middlePoint = (right - left) / 2;

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

Envelope3Editor::~Envelope3Editor()
{
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


void Envelope3Editor::paint (Graphics& g)
{
    g.fillAll (Colour(processor.red, processor.green, processor.blue));   // clear the background
    
    Rectangle<int> area = getLocalBounds().reduced(10);
    
    g.setColour (Colours::white);
    g.setFont (14.0f);
//    g.drawText ("MasterEnv", area, Justification::centredTop);
    
    // ENV DRAWING
    g.setColour (Colours::grey);
    Rectangle<int> area1 = getLocalBounds();
    Rectangle<int> drawArea2 = area1.removeFromBottom(120);
    g.drawRect(drawArea2,2);
    
    // impact line
    g.setColour (Colours::white);
    g.drawLine(middlePoint, drawArea2.getBottom(), middlePoint, drawArea2.getY());
    
    // env path
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

void Envelope3Editor::resized()
{
    Rectangle<int> area = getLocalBounds().reduced(40).withTrimmedLeft(120);
    
    // menu
    env3FilterMenu.setBounds(area.removeFromTop(20));
    
    // forwards / backwards
    Rectangle<int> area8 = getLocalBounds().removeFromTop(40).withTrimmedLeft(180).removeFromLeft(120);
    Rectangle<int> area10 = area8.removeFromRight(60);
    Rectangle<int> area9 = area8.removeFromRight(60);
    for (int i = 0; i < amountOfOptions; i++){
        // dont draw direction buttons for filter and reverb
        if (i != 1 && i != 3 && i != 4) {
            env3ForwardsButtons[i].setBounds(area10);
            env3BackwardsButtons[i].setBounds(area9);
        }
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
void Envelope3Editor::selectGUI() {
    // Check which gui is selected, and set the gui
    for (int ii = 0; ii < amountOfOptions; ii++){
        onOffButton[ii].setVisible(false);
        dopplerButton[ii].setVisible(false);
        env2AttackSlider[ii].setVisible(false);
        env2ReleaseSlider[ii].setVisible(false);
        env2DelaySlider[ii].setVisible(false);
        env2RangeSlider[ii].setVisible(false);
        env3BackwardsButtons[ii].setVisible(false);
        env3ForwardsButtons[ii].setVisible(false);
        sustainButton[ii].setVisible(false);
        // select gui based on menu selection
        if ((ii + 2) == env3Menu.getSelectedId()){
            // if selection is 3 go to else, to select gui based on filter menu
            if (ii + 2 != 3){
                
                if (ii == 0) {
                    env2RangeSlider[ii].setVisible(true);
                }
                if (ii != 4) {
                    sustainButton[ii].setVisible(true);
                    endPoint = (sustainButton[ii].getToggleState() == 1) ? 1 : 0;
                }
                else {
                    endPoint = 1;
                }
                env3FilterMenu.setVisible(false);
                onOffButton[ii].setVisible(true);
                dopplerButton[ii].setVisible(true);
                env2AttackSlider[ii].setVisible(true);
                env2ReleaseSlider[ii].setVisible(true);
                env2DelaySlider[ii].setVisible(true);
                env3BackwardsButtons[ii].setVisible(true);
                env3ForwardsButtons[ii].setVisible(true);
                // set correct env drawing values
                startAttack = (env2DelaySlider[ii].getValue() * (middlePoint - left)) + left;
                endAttack = (env2AttackSlider[ii].getValue() * (middlePoint - left)) + left;
                endRelease = ((1 - env2ReleaseSlider[ii].getValue()) * (middlePoint - right)) + right;
                myPath.clear();
                repaint();
            }
            // select gui based on filter menu
            else {
                env3FilterMenu.setVisible(true);
                if (env3FilterMenu.getSelectedId() == 1){
                    onOffButton[ii].setVisible(true);
                    dopplerButton[ii].setVisible(true);
                    env2AttackSlider[ii].setVisible(true);
                    env2ReleaseSlider[ii].setVisible(true);
                    env2DelaySlider[ii].setVisible(true);
                    //                    env2RangeSlider[ii].setVisible(true);
                    env3BackwardsButtons[ii].setVisible(true);
                    env3ForwardsButtons[ii].setVisible(true);
                    //                    sustainButton[ii].setVisible(true);
                    // set correct env drawing values
                    startAttack = (env2DelaySlider[ii].getValue() * (middlePoint - left)) + left;
                    endAttack = (env2AttackSlider[ii].getValue() * (middlePoint - left)) + left;
                    endRelease = ((1 - env2ReleaseSlider[ii].getValue()) * (middlePoint - right)) + right;
                    endPoint = 1;
                    myPath.clear();
                    repaint();
                }
                else {
                    onOffButton[3].setVisible(true);
                    dopplerButton[3].setVisible(true);
                    env2AttackSlider[3].setVisible(true);
                    env2ReleaseSlider[3].setVisible(true);
                    env2DelaySlider[3].setVisible(true);
                    //                    env2RangeSlider[3].setVisible(true);
                    env3BackwardsButtons[3].setVisible(true);
                    env3ForwardsButtons[3].setVisible(true);
                    //                    sustainButton[3].setVisible(true);
                    // set correct env drawing values
                    startAttack = (env2DelaySlider[3].getValue() * (middlePoint - left)) + left;
                    endAttack = (env2AttackSlider[3].getValue() * (middlePoint - left)) + left;
                    endRelease = ((1 - env2ReleaseSlider[3].getValue()) * (middlePoint - right)) + right;
                    endPoint = 1;
                    myPath.clear();
                    repaint();
                }
            }
        }
    }
}

//==============================================================================
void Envelope3Editor::updateToggleState(juce::Button *button)
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
            
            // draw sliders based on toggle on or off and set env drawing
            if (dopplerOn == 1) {
                env2AttackSlider[i].setVisible(false);
                env2ReleaseSlider[i].setVisible(false);
                env2DelaySlider[i].setVisible(false);
                
                startAttack = (0 * (middlePoint - left)) + left;
                endAttack = (1 * (middlePoint - left)) + left;
                endRelease = (0 * (middlePoint - right)) + right;
                myPath.clear();
                repaint();
            }
            else {
                env2AttackSlider[i].setVisible(true);
                env2ReleaseSlider[i].setVisible(true);
                env2DelaySlider[i].setVisible(true);
                
                startAttack = (env2DelaySlider[i].getValue() * (middlePoint - left)) + left;
                endAttack = (env2AttackSlider[i].getValue() * (middlePoint - left)) + left;
                endRelease = ((1 - env2ReleaseSlider[i].getValue()) * (middlePoint - right)) + right;
                myPath.clear();
                repaint();
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

void Envelope3Editor::sliderValueChanged(juce::Slider *slider)
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

void Envelope3Editor::comboBoxChanged(ComboBox *comboBoxThatHasChanged)
{
    // select gui based on the menu selection
    if (comboBoxThatHasChanged == &env3Menu){
        selectGUI();
    }
    // select filter gui based on the filter menu selection
    if (comboBoxThatHasChanged == &env3FilterMenu) {
        if (env3Menu.getSelectedId() == 3){
            if (env3FilterMenu.getSelectedId() == 1){
                onOffButton[3].setVisible(false);
                dopplerButton[3].setVisible(false);
                env2AttackSlider[3].setVisible(false);
                env2ReleaseSlider[3].setVisible(false);
                env2DelaySlider[3].setVisible(false);
                env3BackwardsButtons[3].setVisible(false);
                env3ForwardsButtons[3].setVisible(false);
                
                onOffButton[1].setVisible(true);
                dopplerButton[1].setVisible(true);
                env2AttackSlider[1].setVisible(true);
                env2ReleaseSlider[1].setVisible(true);
                env2DelaySlider[1].setVisible(true);
                env3BackwardsButtons[1].setVisible(true);
                env3ForwardsButtons[1].setVisible(true);
            }
            else {
                onOffButton[3].setVisible(true);
                dopplerButton[3].setVisible(true);
                env2AttackSlider[3].setVisible(true);
                env2ReleaseSlider[3].setVisible(true);
                env2DelaySlider[3].setVisible(true);
                env3BackwardsButtons[3].setVisible(true);
                env3ForwardsButtons[3].setVisible(true);
                
                onOffButton[1].setVisible(false);
                dopplerButton[1].setVisible(false);
                env2AttackSlider[1].setVisible(false);
                env2ReleaseSlider[1].setVisible(false);
                env2DelaySlider[1].setVisible(false);
                env3BackwardsButtons[1].setVisible(false);
                env3ForwardsButtons[1].setVisible(false);
            }
        }
    }
}

