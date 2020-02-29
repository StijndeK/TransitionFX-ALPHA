/*
  ==============================================================================

    MasterEnvelopeEditor.h
    Created: 18 Nov 2018 10:45:16am
    Author:  Stijn de Koning

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class MasterEnvelopeEditor    : public Component, public ComboBox::Listener
{
public:
    MasterEnvelopeEditor(TransitionFxAudioProcessor&);
    ~MasterEnvelopeEditor();

    void paint (Graphics&) override;
    void resized() override;
    
    void comboBoxChanged(ComboBox *comboBoxThatHasChanged) override;
    
    void updateToggleState (Button* button);
    
    // beat linking
    ToggleButton linkOnOffButton { "On" };
    ScopedPointer <AudioProcessorValueTreeState::ButtonAttachment> linkOnOffButtonTree;
    
    TextEditor linkTime;
    
    // create slider to be able to make a value tree
    Slider linkTimeSlider;
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> linkTimeSliderTree;
    
    // button to set linktime
    TextButton linkTimeButton {"set"};
    
    Slider masterReleaseSlider;
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> masterReleaseSliderTree;
    
    Slider masterReleaseBeatsNotesSlider;
    Slider masterReleaseSamplesSlider;
    Slider masterReleaseFramesSlider;
    
    // combobox
    ComboBox timeLinkMenu;
    ScopedPointer <AudioProcessorValueTreeState::ComboBoxAttachment> timeLinkMenuTree;

private:
    // sliders
    Slider masterAttackSlider;
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> masterAttackSliderTree;
    
    Slider masterAttackBeatsNotesSlider;
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> masterAttackBeatsNotesSliderTree;
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> masterReleaseBeatsNotesSliderTree;
    
    Slider masterAttackSamplesSlider;
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> masterAttackSamplesSliderTree;
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> masterReleaseSamplesSliderTree;
    
    Slider masterAttackFramesSlider;
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> masterAttackFramesSliderTree;
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> masterReleaseFramesSliderTree;

    // processor
    TransitionFxAudioProcessor& processor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MasterEnvelopeEditor)
};
