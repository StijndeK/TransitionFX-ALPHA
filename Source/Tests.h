/*
  ==============================================================================

    Tests.h
    Created: 26 Nov 2018 10:44:12am
    Author:  Stijn de Koning

  ==============================================================================
*/

#pragma once

/*
 
class EnvelopeEditor    : public Component, public ComboBox::Listener, public Slider::Listener
{
public:
    EnvelopeEditor(TransitionFxAudioProcessor&);
    ~EnvelopeEditor();
    
    void paint (Graphics&) override;
    void resized() override;
    
    void comboBoxChanged(ComboBox *comboBoxThatHasChanged) override;
    void sliderValueChanged(Slider *slider) override;
    void updateToggleState (Button* button);
    
private:
    
    //create comboBox + valuetree
    ComboBox env1Menu;
    ScopedPointer <AudioProcessorValueTreeState::ComboBoxAttachment> env1MenuTree;
    
    //create Sliders + valuetree
    Slider env1AttackSlider;
    Slider env1ReleaseSlider;
    Slider env1AmpStartValue;
    Slider env1VerhoudingGemiddelde;
    Slider env1Point1YSlider, env1Point2YSlider, env1Point1XSlider, env1Point2XSlider;
    Slider env1PointSliders[6];
    Slider env1Point0Sliders[3];
    Slider env1PointReleaseSliders[6];
    
    
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> env1AttackSliderTree;
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> env1ReleaseSliderTree;
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> env1AmpStartValueTree;
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> env1VerhoudingGemiddeldeTree;
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> env1Point1YSliderTree;
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> env1Point2YSliderTree;
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> env1Point1XSliderTree;
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> env1Point2XSliderTree;
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> env1PointSlidersTree[6];
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> env1PointReleaseSlidersTree[6];
    
    
    // slider for smoothing
    Slider env1SmoothAmountSlider;
    ScopedPointer <AudioProcessorValueTreeState::SliderAttachment> env1SmoothAmountSliderTree;
    
    // create buttons + valuetrees
    ToggleButton smoothOnOffButton {"smooth"};
    ScopedPointer<AudioProcessorValueTreeState::ButtonAttachment> smoothOnOffButtonTree;
    
    TextButton linearButton {"linear"};
    TextButton exponentialButton {"exponential"};
    TextButton anticlimacticButton {"anticlimactic"};
    TextButton sShapeButton {"s-shape"};
    Slider envGemiddeldeSlider;
    
    TransitionFxAudioProcessor& processor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EnvelopeEditor)
};

 
 
 */
