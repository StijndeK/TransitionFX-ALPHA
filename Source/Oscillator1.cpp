/*
  ==============================================================================

    Oscillator1.cpp
    Created: 29 Oct 2018 11:10:36am
    Author:  Stijn de Koning

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Oscillator1.h"

//==============================================================================
Oscillator1::Oscillator1(TransitionFxAudioProcessor& p, string (&IDS)[4], string (&Gains)[4], string (&Detunes)[4]) : processor(p)
{
    // Names
    componentName = IDS[0];
    componentName.replace(componentName.begin()+4, componentName.end()-4, " ");
    componentName.replace(componentName.begin()+7, componentName.end(), " ");
        
    string arrayMenus [4];
    string arrayDetunes [4];
    string arrayGains [4];

    // iterate through all oscillators (4)
    for(int i = 0; i < amountOfOscillators; i++) {
        arrayMenus  [i] = IDS[i];
        arrayDetunes[i] = Detunes[i];
        arrayGains  [i] = Gains[i];
                
        // Combobox
        osc1Menu[i].addItem("Off", 1);
        osc1Menu[i].addItem("Noise1 White", 2);
        osc1Menu[i].addItem("Noise2 Brown", 3);
        osc1Menu[i].addItem("Noise3 Pink", 4);
        osc1Menu[i].addItem("Saw", 5);
        osc1Menu[i].addItem("Sine", 6);
        osc1Menu[i].addItem("Square", 7);
        osc1Menu[i].addItem("Triangle", 8);
        osc1Menu[i].setJustificationType(Justification::centred);
        addAndMakeVisible(&osc1Menu[i]);
        osc1Menu[i].addListener(this);
        osc1MenuTree[i] = new AudioProcessorValueTreeState::ComboBoxAttachment (processor.tree, arrayMenus[i], osc1Menu[i]);
        
        // Sliders
        osc1Detune[i].setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
        osc1Detune[i].setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
        addAndMakeVisible(&osc1Detune[i]);
        osc1DetuneTree[i] = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, arrayDetunes[i], osc1Detune[i]);
    
        osc1Gain[i].setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
        osc1Gain[i].setTextBoxStyle(Slider::TextBoxBelow, false, 60, 20);
        addAndMakeVisible(&osc1Gain[i]);
        osc1GainTree[i] = new AudioProcessorValueTreeState::SliderAttachment (processor.tree, arrayGains[i], osc1Gain[i]);
        
        // Buttons
        addAndMakeVisible(&osc1DetuneUpSetter[i]);
        osc1DetuneUpSetter[i].setButtonText("Up");
        addAndMakeVisible(&osc1DetuneDownSetter[i]);
        osc1DetuneDownSetter[i].setButtonText("Down");
        
        // Initialise
        for (int i = 0; i < amountOfOscillators; i++) {
            comboBoxChanged(&osc1Menu[i]);
        }
    }
    
    // Buttons
    addAndMakeVisible(&randomButton);
    randomButton.onClick = [this] { updateToggleState (&randomButton);};
    
    osc1DetuneUpSetter[0]  .onClick = [this] { updateToggleState (&osc1DetuneUpSetter[0]);   };
    osc1DetuneUpSetter[1]  .onClick = [this] { updateToggleState (&osc1DetuneUpSetter[1]);   };
    osc1DetuneUpSetter[2]  .onClick = [this] { updateToggleState (&osc1DetuneUpSetter[2]);   };
    osc1DetuneUpSetter[3]  .onClick = [this] { updateToggleState (&osc1DetuneUpSetter[3]);   };
    osc1DetuneDownSetter[0]  .onClick = [this] { updateToggleState (&osc1DetuneDownSetter[0]);   };
    osc1DetuneDownSetter[1]  .onClick = [this] { updateToggleState (&osc1DetuneDownSetter[1]);   };
    osc1DetuneDownSetter[2]  .onClick = [this] { updateToggleState (&osc1DetuneDownSetter[2]);   };
    osc1DetuneDownSetter[3]  .onClick = [this] { updateToggleState (&osc1DetuneDownSetter[3]);   };
}

Oscillator1::~Oscillator1()
{
    for(int i = 0; i < amountOfOscillators; i++){
        osc1MenuTree  [i].~ScopedPointer();
        osc1DetuneTree[i].~ScopedPointer();
        osc1GainTree  [i].~ScopedPointer();
    }
}

void Oscillator1::paint (Graphics& g)
{
    g.fillAll(Colours::black);

    Rectangle<int> area = getLocalBounds().reduced(10);
    
    g.setColour (Colours::grey);
    Rectangle<int> areaRR = area.withTrimmedTop(20);
    g.drawRoundedRectangle(areaRR.getX(), areaRR.getY(), areaRR.getWidth(), areaRR.getHeight(), 8, 2.0);
    g.setColour(Colour(processor.red, processor.green, processor.blue));
    g.fillRoundedRectangle(areaRR.getX(), areaRR.getY(), areaRR.getWidth(), areaRR.getHeight(), 8);

    g.setColour (Colours::white);
    g.setFont (16.0f);
    g.drawText (componentName, area, Justification::centredTop);
    
    
    g.setColour (Colours::lightgrey);
    g.setFont (12.0f);
    Rectangle<int> textArea = getLocalBounds().reduced(40);
    Rectangle<int> topTextArea = textArea.removeFromTop(getLocalBounds().reduced(40).getHeight()/2).reduced(0, 20).removeFromTop(20);
    
    g.drawText ("Detune", topTextArea.removeFromLeft(getLocalBounds().reduced(40).getWidth()/4), Justification::centred);
    g.drawText ("Gain", topTextArea.removeFromLeft(getLocalBounds().reduced(40).getWidth()/4), Justification::centred);
    g.drawText ("Detune", topTextArea.removeFromLeft(getLocalBounds().reduced(40).getWidth()/4), Justification::centred);
    g.drawText ("Gain", topTextArea.removeFromLeft(getLocalBounds().reduced(40).getWidth()/4), Justification::centred);

    Rectangle<int> bottomTextArea = textArea.removeFromTop(getLocalBounds().reduced(40).getHeight()/2).reduced(0, 20).removeFromTop(20);

    g.drawText ("Detune", bottomTextArea.removeFromLeft(getLocalBounds().reduced(40).getWidth()/4), Justification::centred);
    g.drawText ("Gain", bottomTextArea.removeFromLeft(getLocalBounds().reduced(40).getWidth()/4), Justification::centred);
    g.drawText ("Detune", bottomTextArea.removeFromLeft(getLocalBounds().reduced(40).getWidth()/4), Justification::centred);
    g.drawText ("Gain", bottomTextArea.removeFromLeft(getLocalBounds().reduced(40).getWidth()/4), Justification::centred);
}

void Oscillator1::resized()
{
    // random button top right
    Rectangle<int> area5 = getLocalBounds().reduced(10).removeFromRight(80).removeFromTop(15);
    randomButton.setBounds(area5);

    Rectangle<int> area = getLocalBounds().reduced(40);
    Rectangle<int> topArea = area.removeFromTop(area.getHeight()/2);
    Rectangle<int> topSlidersArea = topArea.withTrimmedBottom(20).withTrimmedTop(20);
    Rectangle<int> topMenuArea = topArea.removeFromTop(20);
    Rectangle<int> topButtonArea = topArea.withTrimmedBottom(10).removeFromBottom(20);
    Rectangle<int> bottomArea = area;
    Rectangle<int> bottomSlidersArea = bottomArea.withTrimmedBottom(20).withTrimmedTop(20);
    Rectangle<int> bottomMenuArea = bottomArea.removeFromTop(20);
    Rectangle<int> bottomButtonArea = bottomArea.withTrimmedBottom(10).removeFromBottom(20);

    osc1Menu[0].setBounds(topMenuArea.removeFromLeft(area.getWidth() / 2).reduced(5,0));
    osc1Menu[1].setBounds(topMenuArea.reduced(5,0));
    
    osc1Detune[0].setBounds(topSlidersArea.removeFromLeft(area.getWidth()/4).reduced(25));
    osc1Gain[0].setBounds(topSlidersArea.removeFromLeft(area.getWidth()/4).reduced(25));
    osc1Detune[1].setBounds(topSlidersArea.removeFromLeft(area.getWidth()/4).reduced(25));
    osc1Gain[1].setBounds(topSlidersArea.reduced(25));
    
    osc1DetuneDownSetter[0].setBounds(topButtonArea.removeFromLeft(area.getWidth()/8).reduced(2));
    osc1DetuneUpSetter[0].setBounds(topButtonArea.removeFromLeft(area.getWidth()/8).reduced(2));
    topButtonArea.removeFromLeft(area.getWidth()/4);
    osc1DetuneDownSetter[1].setBounds(topButtonArea.removeFromLeft(area.getWidth()/8).reduced(2));
    osc1DetuneUpSetter[1].setBounds(topButtonArea.removeFromLeft(area.getWidth()/8).reduced(2));
    
    osc1Menu[2].setBounds(bottomMenuArea.removeFromLeft(area.getWidth() / 2).reduced(5,0));
    osc1Menu[3].setBounds(bottomMenuArea.reduced(5,0));
    
    osc1Detune[2].setBounds(bottomSlidersArea.removeFromLeft(area.getWidth()/4).reduced(25));
    osc1Gain[2].setBounds(bottomSlidersArea.removeFromLeft(area.getWidth()/4).reduced(25));
    osc1Detune[3].setBounds(bottomSlidersArea.removeFromLeft(area.getWidth()/4).reduced(25));
    osc1Gain[3].setBounds(bottomSlidersArea.reduced(25));
    
    osc1DetuneDownSetter[2].setBounds(bottomButtonArea.removeFromLeft(area.getWidth()/8).reduced(2));
    osc1DetuneUpSetter[2].setBounds(bottomButtonArea.removeFromLeft(area.getWidth()/8).reduced(2));
    bottomButtonArea.removeFromLeft(area.getWidth()/4);
    osc1DetuneDownSetter[3].setBounds(bottomButtonArea.removeFromLeft(area.getWidth()/8).reduced(2));
    osc1DetuneUpSetter[3].setBounds(bottomButtonArea.removeFromLeft(area.getWidth()/8).reduced(2));
}

//==============================================================================
void Oscillator1::updateToggleState(Button *button)
{
    // detune up and down setters
    for (int i = 0; i < 4; i++){
        if (button == &osc1DetuneUpSetter[i]) {
            // set numb
            currentnumb[i] = currentnumb[i] + 1;
            // check scope
            if (currentnumb[i] < 0){
                currentnumb[i] = currentnumb[i] + 1;
            }
            if ( currentnumb[i] > 18){
                currentnumb[i] = currentnumb[i] - 1;
            }

            osc1Detune[i].setValue(detuneValues[currentnumb[i]]);
        }
        if (button == &osc1DetuneDownSetter[i]) {
            // set numb
            currentnumb[i] = currentnumb[i] - 1;
            // check scope
            if (currentnumb[i] < 0){
                currentnumb[i] = currentnumb[i] + 1;
            }
            if ( currentnumb[i] > 18){
                currentnumb[i] = currentnumb[i] - 1;
            }

            osc1Detune[i].setValue(detuneValues[currentnumb[i]]);
        }
    }
    
    // random button
    if (button == &randomButton) {
        for (int i = 0; i < 4; i++) {
            osc1Menu[i].setSelectedId(rand()%7 + 1);
            osc1Detune[i].setValue((rand()%6)-3);
            osc1Gain[i].setValue(rand()%6 - 6);
        }
    }
}

void Oscillator1::comboBoxChanged(ComboBox *comboBoxThatHasChanged){
    // check if detune button should be visible
    for(int i = 0; i < amountOfOscillators; i++){
        if (comboBoxThatHasChanged == &osc1Menu[i]){
            if (osc1Menu[i].getSelectedId() == 1 || osc1Menu[i].getSelectedId() == 2 || osc1Menu[i].getSelectedId() == 3 || osc1Menu[i].getSelectedId() == 4){
                osc1Detune[i].setVisible(false);
                osc1DetuneDownSetter[i].setVisible(false);
                osc1DetuneUpSetter[i].setVisible(false);
            }
            else {
                osc1Detune[i].setVisible(true);
                osc1DetuneDownSetter[i].setVisible(true);
                osc1DetuneUpSetter[i].setVisible(true);
            }
        }
    }
}


