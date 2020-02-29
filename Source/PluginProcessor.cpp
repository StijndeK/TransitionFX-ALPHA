
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TransitionFxAudioProcessor::TransitionFxAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif

                       ),
tree (*this, nullptr)       // initialise valuetree
#endif
{
    //==============================================================================
    // setup format manager
    //==============================================================================

    formatManager.registerBasicFormats();
    
    // load default sample
    File myFile ("/Users/stijn/Documents/HKU/FilmFxPlugin/JUCE_files/TransitionFX/AudioFiles/Big_Impact.wav");
    auto* reader = formatManager.createReaderFor(myFile);
    if (reader != nullptr) {
        std::unique_ptr<AudioFormatReaderSource> tempSource (new AudioFormatReaderSource (reader, true));
        transport.setSource(tempSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(tempSource.release());
    }
    
    // ==============================================================================
    // Set Normalisalbe Ranges
    // ==============================================================================
    // all sliders require specific ranges
    
    // osc1
    NormalisableRange<float> osc1MenuRange                  (0,7);
    NormalisableRange<float> osc1DetuneRange                (-3.0,3.0, 0.001);

    // env1
    NormalisableRange<float> env1MenuRange                  (0,4);
    NormalisableRange<float> env1AttackSliderRange          (10.0,20000.0,1.0);
    NormalisableRange<float> env1ReleaseSliderRange         (10.0,20000.0,1.0);
    NormalisableRange<float> env1PointSlidersRange          (0.0,1.0,0.001);
    NormalisableRange<float> env1SmoothAmountSliderRange    (1,1000,1);
    NormalisableRange<float> env1AttackReleaseRatioSliderRange(0.1,1,0.01);
    
    // env2
    NormalisableRange<float> env2MenuRange                  (0,7);
    NormalisableRange<float> env2AttackSliderRange          (0.001, 1.0, 0.001);
    NormalisableRange<float> env2RangeSliderRange           (-10000,10000,0.001); // can be as much as i want, sliders arent visible anyways
    
    // master
    NormalisableRange<float> masterGainSliderRange          (-78.0,0.0, 0.01, 2.5);
    NormalisableRange<float> masterVoiceCountSliderRange    (1.0,5.0,1.0);
    NormalisableRange<float> voicePanSlidersRange           (-1,1,0.01);
    NormalisableRange<float> linkTimeRange                  (0.f, 100.f, 1.f);
    
    // master 2
    NormalisableRange<float> voicesSliderRange              (1,amountOfSubvoices,1);
    NormalisableRange<float> masterARSlidersBeatsNotesRange (1,16,1);
    NormalisableRange<float> timeLinkMenuRange              (0,5,1);
    // check if a samplerate is given, otherwise set value to standard
    float standardSamplesRange;
    if (getSampleRate() == 0) {
        standardSamplesRange = 44100;
    }
    else {
        standardSamplesRange = getSampleRate();
    }
    NormalisableRange<float> masterARSlidersSamplesRange    (1, standardSamplesRange * 4, 1);
    NormalisableRange<float> masterARSlidersFramesRange     (1, 24 * 10, 1);
    
    // lfo1
    NormalisableRange<float> lfo1FreqSliderRange            (0.5,20.0, 0.01);
    NormalisableRange<float> lfo1MenuRange                  (0,4);
    
    // filter
    NormalisableRange<float> filterResonanceSliderRange     (1.0f, 5.0f, 0.01);
    NormalisableRange<float> filterCutOffSliderRange        (20.0f, 20000.0f, 1.0, 0.5);
    
    // distortion
    NormalisableRange<float> distortionDriveSliderRange     (0.001, 1.0);
    NormalisableRange<float> distortionRangeSliderRange     (1.0, 3000.0);
    NormalisableRange<float> distortionBlendSliderRange     (0.001, 1.0);
    NormalisableRange<float> distortionGainSliderRange      (0.001, 1.0);
    
    // reverb
    NormalisableRange<float> reverbSliderRange              (0.0, 1.0);
    
    // Hits
    NormalisableRange<float> kickPitchSliderRange           (0.0, 400.0, 1.0);
    NormalisableRange<float> kickReleaseSliderRange         (0.0, 1000.0, 1.0);
    NormalisableRange<float> kickGainSliderRange            (0.0, 5.0, 0.01);
    NormalisableRange<float> kickTempoOffsetSliderRange     (0.0, 1.0, 0.001);

    // buttons
    NormalisableRange<float> buttonRange                    (0,1);
    
    // ==============================================================================
    // set Names, Labels
    // ==============================================================================
    // Names and labels need to be created to link to the valuetree.
    
    // loop for setting names and labels
    for (int i = 0; i < 6; i++) {
        if (i < 1) {
            // lfo1
            lfo1MenuNames[i] = lfo1MenuIDs[i];
            lfo1MenuNames[i].replace(lfo1MenuNames[i].end() - 2, lfo1MenuNames[i].end(), "Name");
            lfo1MenuLabels[i] = lfo1MenuIDs[i];
            lfo1MenuLabels[i].replace(lfo1MenuLabels[i].end() - 2, lfo1MenuLabels[i].end(), "Label");
            
            lfo1FreqSliderNames[i] = lfo1FreqSliderIds[i];
            lfo1FreqSliderNames[i].replace(lfo1FreqSliderNames[i].end() - 2, lfo1FreqSliderNames[i].end(), "Name");
            lfo1FreqSliderLabels[i] = lfo1FreqSliderIds[i];
            lfo1FreqSliderLabels[i].replace(lfo1FreqSliderLabels[i].end() - 2, lfo1FreqSliderLabels[i].end(), "Label");
        }
        if (i < 2) {
            // lfo 2
            for (int ii = 0; ii < amountOfSubvoices; ii++) {    // every voice
                lfo2MenusNames[ii][i] = lfo2Menus[ii][i];
                lfo2MenusNames[ii][i].replace(lfo2MenusNames[ii][i].end() - 2, lfo2MenusNames[ii][i].end(), "Name");
                lfo2MenusLabels[ii][i] = lfo2Menus[ii][i];
                lfo2MenusLabels[ii][i].replace(lfo2MenusLabels[ii][i].end() - 2, lfo2MenusLabels[ii][i].end(), "Label");
                
                lfo2FreqSlidersNames[ii][i] = lfo2FreqSliders[ii][i];
                lfo2FreqSlidersNames[ii][i].replace(lfo2FreqSlidersNames[ii][i].end() - 2, lfo2FreqSlidersNames[ii][i].end(), "Name");
                lfo2FreqSlidersLabels[ii][i] = lfo2FreqSliders[ii][i];
                lfo2FreqSlidersLabels[ii][i].replace(lfo2FreqSlidersLabels[ii][i].end() - 2, lfo2FreqSlidersLabels[ii][i].end(), "Label");
            }
        }
        
        if (i < amountOfSubvoices) {        // every voice
            // osc1
            osc1AmountOfOscVoicesNames[i] = osc1AmountOfOscVoicesIDS[i];
            osc1AmountOfOscVoicesLabels[i] = osc1AmountOfOscVoicesIDS[i];
            osc1AmountOfOscVoicesNames[i].replace(osc1AmountOfOscVoicesNames[i].end() - 2, osc1AmountOfOscVoicesNames[i].end(), "Name");
            osc1AmountOfOscVoicesLabels[i].replace(osc1AmountOfOscVoicesLabels[i].end() - 2, osc1AmountOfOscVoicesLabels[i].end(), "Label");
            
            osc1VoicesPitchOffsetNames[i] = osc1AmountOfOscVoicesIDS[i];
            osc1VoicesPitchOffsetLabels[i] = osc1AmountOfOscVoicesIDS[i];
            osc1VoicesPitchOffsetNames[i].replace(osc1VoicesPitchOffsetNames[i].end() - 2, osc1VoicesPitchOffsetNames[i].end(), "Name");
            osc1VoicesPitchOffsetLabels[i].replace(osc1VoicesPitchOffsetLabels[i].end() - 2, osc1VoicesPitchOffsetLabels[i].end(), "Label");
            
            // master
            voiceMixSliderNames[i] = voiceMixSliderIDsP[i];
            voiceMixSliderLabels[i] = voiceMixSliderIDsP[i];
            voiceMixSliderNames[i].replace(voiceMixSliderNames[i].end() - 2, voiceMixSliderNames[i].end(), "Name");
            voiceMixSliderLabels[i].replace(voiceMixSliderLabels[i].end() - 2, voiceMixSliderLabels[i].end(), "Label");
            
            voicePanSliderNames[i] = voicePanSliderIDsP[i];
            voicePanSliderLabels[i] = voicePanSliderIDsP[i];
            voicePanSliderNames[i].replace(voicePanSliderNames[i].end() - 2, voicePanSliderNames[i].end(), "Name");
            voicePanSliderLabels[i].replace(voicePanSliderLabels[i].end() - 2, voicePanSliderLabels[i].end(), "Label");
            
            voiceMuteButtonNames[i] = voiceMuteButtonIDsP[i];
            voiceMuteButtonLabels[i] = voiceMuteButtonIDsP[i];
            voiceMuteButtonNames[i].replace(voiceMuteButtonNames[i].end() - 2, voiceMuteButtonNames[i].end(), "Name");
            voiceMuteButtonLabels[i].replace(voiceMuteButtonLabels[i].end() - 2, voiceMuteButtonLabels[i].end(), "Label");
            
            voiceSoloButtonNames[i] = voiceSoloButtonIDsP[i];
            voiceSoloButtonLabels[i] = voiceSoloButtonIDsP[i];
            voiceSoloButtonNames[i].replace(voiceSoloButtonNames[i].end() - 2, voiceSoloButtonNames[i].end(), "Name");
            voiceSoloButtonLabels[i].replace(voiceSoloButtonLabels[i].end() - 2, voiceSoloButtonLabels[i].end(), "Label");
        }
        
        if (i < 4) {
            for (int ii = 0; ii < amountOfSubvoices; ii++) {    // every voice
                // osc1
                oscMenusNames[ii][i] = oscMenus[ii][i];
                oscMenusNames[ii][i].replace(oscMenusNames[ii][i].end() - 2, oscMenusNames[ii][i].end(), "Name");
                oscMenusLabels[ii][i] = oscMenus[ii][i];
                oscMenusLabels[ii][i].replace(oscMenusLabels[ii][i].end() - 2, oscMenusLabels[ii][i].end(), "Label");
                
                oscGainsNames[ii][i] = oscGains[ii][i];
                oscGainsNames[ii][i].replace(oscGainsNames[ii][i].end() - 2, oscGainsNames[ii][i].end(), "Name");
                oscGainsLabels[ii][i] = oscGains[ii][i];
                oscGainsLabels[ii][i].replace(oscGainsLabels[ii][i].end() - 2, oscGainsLabels[ii][i].end(), "Label");
                
                oscDetunesNames[ii][i] = oscDetunes[ii][i];
                oscDetunesNames[ii][i].replace(oscDetunesNames[ii][i].end() - 2, oscDetunesNames[ii][i].end(), "Name");
                oscDetunesLabels[ii][i] = oscDetunes[ii][i];
                oscDetunesLabels[ii][i].replace(oscDetunesLabels[ii][i].end() - 2, oscDetunesLabels[ii][i].end(), "Label");
                
                // env2
                env2AttackSlidersNames[ii][i] = env2AttackSlidersP[ii][i];
                env2AttackSlidersNames[ii][i].replace(env2AttackSlidersNames[ii][i].end() - 2, env2AttackSlidersNames[ii][i].end(), "Name");
                env2AttackSlidersLabels[ii][i] = env2AttackSlidersP[ii][i];
                env2AttackSlidersLabels[ii][i].replace(env2AttackSlidersLabels[ii][i].end() - 2, env2AttackSlidersLabels[ii][i].end(), "Label");
                
                env2ReleaseSlidersNames[ii][i] = env2ReleaseSlidersP[ii][i];
                env2ReleaseSlidersNames[ii][i].replace(env2ReleaseSlidersNames[ii][i].end() - 2, env2ReleaseSlidersNames[ii][i].end(), "Name");
                env2ReleaseSlidersLabels[ii][i] = env2ReleaseSlidersP[ii][i];
                env2ReleaseSlidersLabels[ii][i].replace(env2ReleaseSlidersLabels[ii][i].end() - 2, env2ReleaseSlidersLabels[ii][i].end(), "Label");
                
                env2DelaySlidersNames[ii][i] = env2DelaySlidersP[ii][i];
                env2DelaySlidersNames[ii][i].replace(env2DelaySlidersNames[ii][i].end() - 2, env2DelaySlidersNames[ii][i].end(), "Name");
                env2DelaySlidersLabels[ii][i] = env2DelaySlidersP[ii][i];
                env2DelaySlidersLabels[ii][i].replace(env2DelaySlidersLabels[ii][i].end() - 2, env2DelaySlidersLabels[ii][i].end(), "Label");
                
                env2OnOffButtonsNames[ii][i] = env2OnOffButtonsP[ii][i];
                env2OnOffButtonsNames[ii][i].replace(env2OnOffButtonsNames[ii][i].end() - 2, env2OnOffButtonsNames[ii][i].end(), "Name");
                env2OnOffButtonsLabels[ii][i] = env2OnOffButtonsP[ii][i];
                env2OnOffButtonsLabels[ii][i].replace(env2OnOffButtonsLabels[ii][i].end() - 2, env2OnOffButtonsLabels[ii][i].end(), "Label");
                
                env2DopplerButtonsNames[ii][i] = env2DopplerButtonsP[ii][i];
                env2DopplerButtonsNames[ii][i].replace(env2DopplerButtonsNames[ii][i].end() - 2, env2DopplerButtonsNames[ii][i].end(), "Name");
                env2DopplerButtonsLabels[ii][i] = env2DopplerButtonsP[ii][i];
                env2DopplerButtonsLabels[ii][i].replace(env2DopplerButtonsLabels[ii][i].end() - 2, env2DopplerButtonsLabels[ii][i].end(), "Label");
                
                envForwardsButtonsNames[ii][i] = env2ForwardsButtonsP[ii][i];
                envForwardsButtonsNames[ii][i].replace(envForwardsButtonsNames[ii][i].end() - 2, envForwardsButtonsNames[ii][i].end(), "Name");
                envForwardsButtonsLabels[ii][i] = env2ForwardsButtonsP[ii][i];
                envForwardsButtonsLabels[ii][i].replace(envForwardsButtonsLabels[ii][i].end() - 2, envForwardsButtonsLabels[ii][i].end(), "Label");
                
                env2sustainButtonsNames[ii][i] = env2SustainButtonsP[ii][i];
                env2sustainButtonsNames[ii][i].replace(env2sustainButtonsNames[ii][i].end() - 2, env2sustainButtonsNames[ii][i].end(), "Name");
                modulationVoiceLabels[ii][i] = env2SustainButtonsP[ii][i];
                modulationVoiceLabels[ii][i].replace(modulationVoiceLabels[ii][i].end() - 2, modulationVoiceLabels[ii][i].end(), "Label");
                
                for (int iii = 0; iii < 2; iii++) { // every range (low and high)
                    env2RangeSlidersNames[iii][ii][i] = env2RangeSlidersP[iii][ii][i];
                    env2RangeSlidersNames[iii][ii][i].replace(env2RangeSlidersNames[iii][ii][i].end() - 2, env2RangeSlidersNames[iii][ii][i].end(), "Name");
                    env2RangeSlidersLabels[iii][ii][i] = env2RangeSlidersP[iii][ii][i];
                    env2RangeSlidersLabels[iii][ii][i].replace(env2RangeSlidersLabels[iii][ii][i].end() - 2, env2RangeSlidersLabels[iii][ii][i].end(), "Label");
                }
                
                // modulation screen
                modulationVoiceNames[ii][i] = modulationVoiceTriggersP[ii][i];
                modulationVoiceNames[ii][i].replace(modulationVoiceNames[ii][i].end() - 2, modulationVoiceNames[ii][i].end(), "Name");
                env2sustainButtonsLabels[ii][i] = modulationVoiceTriggersP[ii][i];
                env2sustainButtonsLabels[ii][i].replace(env2sustainButtonsLabels[ii][i].end() - 2, env2sustainButtonsLabels[ii][i].end(), "Label");
            }
        }

        if (i < 5) {
            // reverb
            reverbSliderNames[i] = reverbSliderIDs[i];
            reverbSliderNames[i].replace(reverbSliderNames[i].end() - 2, reverbSliderNames[i].end(), "Name");
            reverbSliderLabels[i] = reverbSliderIDs[i];
            reverbSliderLabels[i].replace(reverbSliderLabels[i].end() - 2, reverbSliderLabels[i].end(), "Label");
        }
        
        // env3
        env3AttackSliderNames[i] = env3AttackSlidersP[i];
        env3AttackSliderNames[i].replace(env3AttackSliderNames[i].end() - 2, env3AttackSliderNames[i].end(), "Name");
        env3AttackSliderLabels[i] = env3AttackSlidersP[i];
        env3AttackSliderLabels[i].replace(env3AttackSliderLabels[i].end() - 2, env3AttackSliderLabels[i].end(), "Label");
        
        env3ReleaseSliderNames[i] = env3ReleaseSliderP[i];
        env3ReleaseSliderNames[i].replace(env3ReleaseSliderNames[i].end() - 2, env3ReleaseSliderNames[i].end(), "Name");
        env3ReleaseSliderLabels[i] = env3ReleaseSliderP[i];
        env3ReleaseSliderLabels[i].replace(env3ReleaseSliderLabels[i].end() - 2, env3ReleaseSliderLabels[i].end(), "Label");
        
        env3DelaySliderNames[i] = env3DelaySliderP[i];
        env3DelaySliderNames[i].replace(env3DelaySliderNames[i].end() - 2, env3DelaySliderNames[i].end(), "Name");
        env3DelaySliderLabels[i] = env3DelaySliderP[i];
        env3DelaySliderLabels[i].replace(env3DelaySliderLabels[i].end() - 2, env3DelaySliderLabels[i].end(), "Label");
        
        env3OnOffButtonNames[i] = env3OnOffButtonsP[i];
        env3OnOffButtonNames[i].replace(env3OnOffButtonNames[i].end() - 2, env3OnOffButtonNames[i].end(), "Name");
        env3OnOffButtonLabels[i] = env3OnOffButtonsP[i];
        env3OnOffButtonLabels[i].replace(env3OnOffButtonLabels[i].end() - 2, env3OnOffButtonLabels[i].end(), "Label");
        
        env3DopplerButtonNames[i] = env3DopplerButtonsP[i];
        env3DopplerButtonNames[i].replace(env3DopplerButtonNames[i].end() - 2, env3DopplerButtonNames[i].end(), "Name");
        env3DopplerButtonLabels[i] = env3DopplerButtonsP[i];
        env3DopplerButtonLabels[i].replace(env3DopplerButtonLabels[i].end() - 2, env3DopplerButtonLabels[i].end(), "Label");
        
        env3ForwardsButtonNames[i] = env3ForwardsButtonsP[i];
        env3ForwardsButtonNames[i].replace(env3ForwardsButtonNames[i].end() - 2, env3ForwardsButtonNames[i].end(), "Name");
        env3ForwardsButtonLabels[i] = env3ForwardsButtonsP[i];
        env3ForwardsButtonLabels[i].replace(env3ForwardsButtonLabels[i].end() - 2, env3ForwardsButtonLabels[i].end(), "Label");
        
        env3LowRangesNames[i] = env3LowRanges[i];
        env3LowRangesNames[i].replace(env3LowRangesNames[i].end() - 2, env3LowRangesNames[i].end(), "Name");
        env3LowRangesLabels[i] = env3LowRanges[i];
        env3LowRangesLabels[i].replace(env3LowRangesLabels[i].end() - 2, env3LowRangesLabels[i].end(), "Label");
        
        env3HighRangesNames[i] = env3HighRanges[i];
        env3HighRangesNames[i].replace(env3HighRangesNames[i].end() - 2, env3HighRangesNames[i].end(), "Name");
        env3HighRangesLabels[i] = env3HighRanges[i];
        env3HighRangesLabels[i].replace(env3HighRangesLabels[i].end() - 2, env3HighRangesLabels[i].end(), "Label");
        
        env3SustainButtonNames[i] = env3SustainButtonsP[i];
        env3SustainButtonNames[i].replace(env3SustainButtonNames[i].end() - 2, env3SustainButtonNames[i].end(), "Name");
        env3SustainButtonLabels[i] = env3SustainButtonsP[i];
        env3SustainButtonLabels[i].replace(env3SustainButtonLabels[i].end() - 2, env3SustainButtonLabels[i].end(), "Label");
        
        // env1 points
        env1PointSliderNames[i] = env1PointSliderIDs[i];
        env1PointSliderNames[i].replace(env1PointSliderNames[i].end() - 2, env1PointSliderNames[i].end(), "Name");
        env1PointSliderLabels[i] = env1PointSliderIDs[i];
        env1PointSliderLabels[i].replace(env1PointSliderLabels[i].end() - 2, env1PointSliderLabels[i].end(), "Label");
        
        env1PointReleaseSliderNames[i] = env1PointReleaseSliderIDs[i];
        env1PointReleaseSliderNames[i].replace(env1PointReleaseSliderNames[i].end() - 2, env1PointReleaseSliderNames[i].end(), "Name");
        env1PointReleaseSliderLabels[i] = env1PointReleaseSliderIDs[i];
        env1PointReleaseSliderLabels[i].replace(env1PointReleaseSliderLabels[i].end() - 2, env1PointReleaseSliderLabels[i].end(), "Label");
        
        // modulation screen
        modulationMasterNames[i] = modulationMasterTriggersP[i];
        modulationMasterNames[i].replace(modulationMasterNames[i].end() - 2, modulationMasterNames[i].end(), "Name");
        modulationMasterLabels[i] = modulationMasterTriggersP[i];
        modulationMasterLabels[i].replace(modulationMasterLabels[i].end() - 2, modulationMasterLabels[i].end(), "Label");
    }
    
    
    // ==============================================================================
    // set valuestrees recursive
    // ==============================================================================
    // Add IDs Names Labels Ranges and startpoints to valuetrees
    
    // startvalue for env 1 points
    float startValueEnv1Points = 0.0;
    // loop for setting valuetreest
    for (int i = 0; i < 6; i++){
        // env1 (8 steps)
        startValueEnv1Points = startValueEnv1Points + (1.0/7.0);
        tree.createAndAddParameter(env1PointSliderIDs[i], env1PointSliderNames[i], env1PointSliderLabels[i], env1PointSlidersRange, startValueEnv1Points, nullptr, nullptr);
        tree.createAndAddParameter(env1PointReleaseSliderIDs[i], env1PointReleaseSliderNames[i], env1PointReleaseSliderLabels[i], env1PointSlidersRange, 1 - startValueEnv1Points, nullptr, nullptr);
        
        if (i < 1){
            // lfo 1
            tree.createAndAddParameter(lfo1FreqSliderIds[i], lfo1FreqSliderNames[i], lfo1FreqSliderLabels[i], lfo1FreqSliderRange, 15.0, nullptr, nullptr);
            tree.createAndAddParameter(lfo1MenuIDs[i], lfo1MenuNames[i], lfo1MenuLabels[i], lfo1MenuRange, 0.0, nullptr, nullptr);
        }
        
        if (i < 2) {
            // lfo 2
            for (int ii = 0; ii < 3; ii++) {
                tree.createAndAddParameter(lfo2Menus[ii][i], lfo2MenusNames[ii][i], lfo2MenusLabels[ii][i], lfo1MenuRange, 0, nullptr, nullptr);
                tree.createAndAddParameter(lfo2FreqSliders[ii][i], lfo2FreqSlidersNames[ii][i], lfo2FreqSlidersLabels[ii][i], lfo1FreqSliderRange, 15, nullptr, nullptr);
            }
        }
        
        if (i < 4) {
            for (int ii = 0; ii < amountOfSubvoices; ii++) {    // every voice
                // env 2
                tree.createAndAddParameter(env2OnOffButtonsP[ii][i], env2OnOffButtonsNames[ii][i], env2OnOffButtonsLabels[ii][i], buttonRange, 0.0, nullptr, nullptr);
                tree.createAndAddParameter(env2DopplerButtonsP[ii][i], env2DopplerButtonsNames[ii][i], env2DopplerButtonsLabels[ii][i], buttonRange, 0.0, nullptr, nullptr);
                tree.createAndAddParameter(env2AttackSlidersP[ii][i], env2AttackSlidersNames[ii][i], env2AttackSlidersLabels[ii][i], env2AttackSliderRange, 1.0, nullptr, nullptr);
                tree.createAndAddParameter(env2ReleaseSlidersP[ii][i], env2ReleaseSlidersNames[ii][i], env2ReleaseSlidersLabels[ii][i], env2AttackSliderRange, 1.0, nullptr, nullptr);
                tree.createAndAddParameter(env2DelaySlidersP[ii][i], env2DelaySlidersNames[ii][i], env2DelaySlidersLabels[ii][i], env2AttackSliderRange, 0.001, nullptr, nullptr);
                tree.createAndAddParameter(env2ForwardsButtonsP[ii][i], envForwardsButtonsNames[ii][i], envForwardsButtonsLabels[ii][i], buttonRange, 1.0, nullptr, nullptr);
                tree.createAndAddParameter(env2SustainButtonsP[ii][i], env2sustainButtonsNames[ii][i], env2sustainButtonsLabels[ii][i], buttonRange, 1.0, nullptr, nullptr);

                for (int iii = 0; iii < 2; iii++) {             // every range (low and high)
                    tree.createAndAddParameter(env2RangeSlidersP[iii][ii][i], env2RangeSlidersNames[iii][ii][i], env2RangeSlidersLabels[iii][ii][i], env2RangeSliderRange, 0.5, nullptr, nullptr);
                }
                
                
                // modulation screen
                tree.createAndAddParameter(modulationVoiceTriggersP[ii][i], modulationVoiceNames[ii][i], modulationVoiceLabels[ii][i], buttonRange, 0.0, nullptr, nullptr);
                
                // osc1
                tree.createAndAddParameter(oscMenus[ii][i], oscMenusNames[ii][i], oscMenusLabels[ii][i], osc1MenuRange, oscMenuDefaultValues[ii][i], nullptr, nullptr);
                tree.createAndAddParameter(oscGains[ii][i], oscGainsNames[ii][i], oscGainsLabels[ii][i], masterGainSliderRange, 0.0, nullptr, nullptr);
                tree.createAndAddParameter(oscDetunes[ii][i], oscDetunesNames[ii][i], oscDetunesLabels[ii][i], osc1DetuneRange, 0.0, nullptr, nullptr);
            }
        }
        
        if (i < amountOfSubvoices) {
            // osc1
            tree.createAndAddParameter(osc1AmountOfOscVoicesIDS[i], osc1AmountOfOscVoicesNames[i], osc1AmountOfOscVoicesLabels[i], masterVoiceCountSliderRange, 1.0, nullptr, nullptr);
            tree.createAndAddParameter(osc1VoicesPitchOffsetIDS[i], osc1VoicesPitchOffsetNames[i], osc1VoicesPitchOffsetLabels[i], osc1DetuneRange, 1.0, nullptr, nullptr);
            
            // master
            tree.createAndAddParameter(voiceMixSliderIDsP[i], voiceMixSliderNames[i], voiceMixSliderLabels[i], masterGainSliderRange, 0, nullptr, nullptr);
            tree.createAndAddParameter(voicePanSliderIDsP[i], voicePanSliderNames[i], voicePanSliderLabels[i], voicePanSlidersRange, 0, nullptr, nullptr);
            tree.createAndAddParameter(voiceMuteButtonIDsP[i], voiceMuteButtonNames[i], voiceMuteButtonLabels[i], buttonRange, 0, nullptr, nullptr);
            tree.createAndAddParameter(voiceSoloButtonIDsP[i], voiceSoloButtonNames[i], voiceSoloButtonLabels[i], buttonRange, 0, nullptr, nullptr);
        }

        // env 3
        if (i < 5){
            // reverb
            tree.createAndAddParameter(reverbSliderIDs[i], reverbSliderNames[i], reverbSliderLabels[i], reverbSliderRange, 0.5, nullptr, nullptr);
        }
        
        // env3
        tree.createAndAddParameter(env3AttackSlidersP[i], env3AttackSliderNames[i], env3AttackSliderLabels[i], env2AttackSliderRange, 1.0, nullptr, nullptr);
        tree.createAndAddParameter(env3ReleaseSliderP[i], env3ReleaseSliderNames[i], env3ReleaseSliderLabels[i], env2AttackSliderRange, 1.0, nullptr, nullptr);
        tree.createAndAddParameter(env3DelaySliderP[i], env3DelaySliderNames[i], env3DelaySliderLabels[i], env2AttackSliderRange, 0.001, nullptr, nullptr);
        tree.createAndAddParameter(env3OnOffButtonsP[i], env3OnOffButtonNames[i], env3OnOffButtonLabels[i], buttonRange, 0.0, nullptr, nullptr);
        tree.createAndAddParameter(env3DopplerButtonsP[i], env3DopplerButtonNames[i], env3DopplerButtonLabels[i], buttonRange, 0.0, nullptr, nullptr);
        tree.createAndAddParameter(env3ForwardsButtonsP[i], env3ForwardsButtonNames[i], env3ForwardsButtonLabels[i], buttonRange, 1.0, nullptr, nullptr);
        tree.createAndAddParameter(env3LowRanges[i], env3LowRangesNames[i], env3LowRangesLabels[i], lfo1FreqSliderRange, 1.0, nullptr, nullptr);
        tree.createAndAddParameter(env3HighRanges[i], env3HighRangesNames[i], env3HighRangesLabels[i], lfo1FreqSliderRange, 15.0, nullptr, nullptr);
        tree.createAndAddParameter(env3SustainButtonsP[i], env3SustainButtonNames[i], env3SustainButtonLabels[i], buttonRange, 0.0, nullptr, nullptr);
        
        // modulation menu
        tree.createAndAddParameter(modulationMasterTriggersP[i], modulationMasterNames[i], modulationMasterLabels[i], buttonRange, 0.0, nullptr, nullptr);
    }
    
    
    // ==============================================================================
    // set valuetrees manual
    // ==============================================================================
    // Add IDs Names Labels Ranges and startpoints to valuetrees
    
    // hits
    tree.createAndAddParameter("kickPitchSliderID", "kickPitchSliderName", "kickPitchSliderLabel", kickPitchSliderRange, 200.0, nullptr, nullptr);
    tree.createAndAddParameter("kickReleaseSliderID", "kickReleaseSliderName", "kickReleaseSliderLabel", kickReleaseSliderRange, 500.0, nullptr, nullptr);
    tree.createAndAddParameter("kickGainSliderID", "kickGainSliderName", "kickGainSliderLabel", kickGainSliderRange, 1, nullptr, nullptr);
    tree.createAndAddParameter("kickTempoOffsetSliderID", "kickTempoOffsetSliderName", "kickTempoOffsetSliderLabel", kickTempoOffsetSliderRange, 0, nullptr, nullptr);
    tree.createAndAddParameter("kickOnOffButtonID", "kickOnOffButtonName", "kickOnOffButtonLabel", buttonRange, 1, nullptr, nullptr);
    tree.createAndAddParameter("kickDistortionOnOffButtonID", "kickDistortionOnOffButtonName", "kickDistortionOnOffButtonLabel", buttonRange, 0, nullptr, nullptr);
    tree.createAndAddParameter("kickSampleGainSliderID", "kickSampleGainSliderName", "kickSampleGainSliderName", masterGainSliderRange, 0, nullptr, nullptr);

    // env1
    tree.createAndAddParameter("env1MenuID", "Env1MenuName", "env1MenuLabel", env1MenuRange, 4, nullptr, nullptr);
    tree.createAndAddParameter("env1SmoothOnOffButtonID", "env1SmoothOnOffButtonName", "env1SmoothOnOffButtonLabel", buttonRange, 0, nullptr, nullptr);
    tree.createAndAddParameter("env1SmoothAmountSliderID", "env1SmoothAmountSliderName", "env1SmoothAmountSliderLabel", env1SmoothAmountSliderRange, 500, nullptr, nullptr);
    tree.createAndAddParameter("env1AttackReleaseRatioSliderID", "env1AttackReleaseRatioSliderName", "env1AttackReleaseRatioSliderLabel", env1AttackReleaseRatioSliderRange, 1, nullptr, nullptr);
    
    // master 2
    tree.createAndAddParameter("masterAttackSliderID", "masterAttackSliderName", "masterAttackSliderLabel", env1AttackSliderRange, 1000.0, nullptr, nullptr);
    tree.createAndAddParameter("masterReleaseSliderID", "masterReleaseSliderName", "masterReleaseSliderLabel", env1ReleaseSliderRange, 1000.0, nullptr, nullptr);
    tree.createAndAddParameter("masterAttackBeatsNotesSliderID", "masterAttackBeatsNotesSliderName", "masterAttackBeatsNotesSliderLabel", masterARSlidersBeatsNotesRange, 2, nullptr, nullptr);
    tree.createAndAddParameter("masterReleaseBeatsNotesSliderID", "masterReleaseBeatsNotesSliderName", "masterReleaseBeatsNotesSliderLabel", masterARSlidersBeatsNotesRange, 2, nullptr, nullptr);
    tree.createAndAddParameter("masterAttackSamplesSliderID", "masterAttackSamplesSliderName", "masterAttackSamplesSliderLabel", masterARSlidersSamplesRange, standardSamplesRange, nullptr, nullptr);
    tree.createAndAddParameter("masterReleaseSamplesSliderID", "masterReleaseSamplesSliderName", "masterReleaseSamplesSliderLabel", masterARSlidersSamplesRange, standardSamplesRange, nullptr, nullptr);
    tree.createAndAddParameter("masterAttackFramesSliderID", "masterAttackFramesSliderName", "masterAttackFramesSliderLabel", masterARSlidersFramesRange, 24, nullptr, nullptr);
    tree.createAndAddParameter("masterReleaseFramesSliderID", "masterReleaseFramesSliderName", "masterReleaseFramesSliderLabel", masterARSlidersFramesRange, 24, nullptr, nullptr);
    tree.createAndAddParameter("voicesSliderID", "voicesSliderName", "voicesSliderLabel", voicesSliderRange, 1, nullptr, nullptr);
    tree.createAndAddParameter("voiceAmountSliderID", "voiceAmountSliderName", "voiceAmountSliderLabel", voicesSliderRange, 1, nullptr, nullptr);
    tree.createAndAddParameter("timeLinkMenuID", "timeLinkMenuName", "timeLinkMenuLabel", timeLinkMenuRange, 0, nullptr, nullptr);

    // env2
    tree.createAndAddParameter("env2MenuID", "Env2MenuName", "env2MenuLabel", env2MenuRange, 1, nullptr, nullptr);
    
    // master
    tree.createAndAddParameter("masterGainSliderID", "MasterGainSliderName", "masterGainSliderLabel", masterGainSliderRange, 0.0, nullptr, nullptr);
    tree.createAndAddParameter("masterPanSliderID", "masterPanSliderName", "masterPanSliderLabel", voicePanSlidersRange, 0.0, nullptr, nullptr);
    
    tree.createAndAddParameter("linkOnOffButtonID", "linkOnOffButtonName", "linkOnOffButtonLabel", buttonRange, 0.0, nullptr, nullptr);
    tree.createAndAddParameter("LinkTimeSliderID", "LinkTimeSliderName", "LinkTimeSliderLabel", linkTimeRange, 1.f, nullptr, nullptr);
    
    // filter
    tree.createAndAddParameter("filterCutOffSliderID", "FilterCutoffSliderName", "filterCutOffSliderLabel", filterCutOffSliderRange, 600.0f, nullptr, nullptr);
    tree.createAndAddParameter("filterResonanceSliderID", "FilterResonanceSliderName", "filterResonanceSliderLabel", filterResonanceSliderRange, 1.0f, nullptr, nullptr);
    tree.createAndAddParameter("filterOnOffButtonID", "FilterOnOffButtonName", "filterOnOffButtonLabel", buttonRange, 0, nullptr, nullptr);
    
    // distortion
    tree.createAndAddParameter("distortionDriveSliderID", "distortionDriveSliderName", "distortionDriveSliderLabel", distortionDriveSliderRange, 0.5f, nullptr, nullptr);
    tree.createAndAddParameter("distortionRangeSliderID", "distortionRangeSliderName", "distortionRangeSliderLabel", distortionRangeSliderRange, 500.0f, nullptr, nullptr);
    tree.createAndAddParameter("distortionBlendSliderID", "distortionBlendSliderName", "distortionBlendSliderLabel", distortionBlendSliderRange, 0.5f, nullptr, nullptr);
    tree.createAndAddParameter("distortionGainSliderID", "distortionGainSliderName", "distortionGainSliderLabel", distortionGainSliderRange, 0.5f, nullptr, nullptr);
    tree.createAndAddParameter("distortionOnOffButtonID", "distortionOnOffButtonName", "distortionOnOffButtonLabel", buttonRange, 0, nullptr, nullptr);
    tree.createAndAddParameter("distortionMenuID", "distortionMenuName", "distortionMenuLabel", buttonRange, 0, nullptr, nullptr);
    
    // chorus
    tree.createAndAddParameter("chorusOnOffButtonID", "chorusOnOffButtonName", "chorusOnOffButtonLabel", buttonRange, 0, nullptr, nullptr);
    
    // reverb
    tree.createAndAddParameter("reverbOnOffButtonID", "reverbOnOffButtonName", "reverbOnOffButtonLabel", buttonRange, 1, nullptr, nullptr);
    
    // safe for xml
    tree.state = ValueTree("savedParameters");
    
    
    //==============================================================================
    // Juce Synthesiser
    //==============================================================================
    
    // clear old voices
    mySynth.clearVoices();
    // add voices
    for (int i = 0; i < numVoices; i++) {
        mySynth.addVoice(new SynthVoice);
    }
    // clear sound
    mySynth.clearSounds();
    // add sounds
    mySynth.addSound(new SynthSound());
    
    // set samplerate
    if (getSampleRate() != 0){
        mySynth.setCurrentPlaybackSampleRate(getSampleRate());
    }
    else{
        mySynth.setCurrentPlaybackSampleRate(44100);
    }
    
    //==============================================================================
    // Reverb
    //==============================================================================

    auto& verb = fxChain.template get<reverbIndex>();
    verb.setParameters(reverbParameters);
}

TransitionFxAudioProcessor::~TransitionFxAudioProcessor()
{
    
}

//==============================================================================
const String TransitionFxAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TransitionFxAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TransitionFxAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool TransitionFxAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double TransitionFxAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TransitionFxAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int TransitionFxAudioProcessor::getCurrentProgram()
{
    return 0;
}

void TransitionFxAudioProcessor::setCurrentProgram (int index)
{
}

const String TransitionFxAudioProcessor::getProgramName (int index)
{
    return {};
}

void TransitionFxAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void TransitionFxAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Initialise JUCE::DSP code (reverb and filter)
    lastSampleRate = sampleRate;
    
    dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    
    fxChain.reset();
    fxChain.prepare (spec);
 
    lowPassFilter.reset();
    lowPassFilter.prepare(spec);
    
    // sample playing
    transport.prepareToPlay(samplesPerBlock, sampleRate);
}

void TransitionFxAudioProcessor::releaseResources()
{

}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TransitionFxAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

// sample loading and playing
void TransitionFxAudioProcessor::getEditorInfo(int buttonPlay, int buttonLoad)
{
    if (playButton != buttonPlay) {
        std::cout << "play" << std::endl;
        transportStateChanged(Stopping);
        transportStateChanged(Stopped);
        transportStateChanged(Starting);
    }
    if (loadButton != buttonLoad) {
        DBG("load");
        FileChooser chooser ("Choose a file", {}, "*.wav");
        
        if (chooser.browseForFileToOpen()) {
            File myFile = chooser.getResult();
            
            auto* reader = formatManager.createReaderFor(myFile);
            
            if (reader != nullptr) {
                std::unique_ptr<AudioFormatReaderSource> tempSource (new AudioFormatReaderSource (reader, true));
                transport.setSource(tempSource.get(), 0, nullptr, reader->sampleRate);
                readerSource.reset(tempSource.release());
            }
        }
    }
}

void TransitionFxAudioProcessor::transportStateChanged(TransportState state)
{
    // if state changed
    if (state != transState)
    {
        transState = state;
        
        switch (transState) {
            case Stopped:
                transport.setPosition(0.0);
                break;
                
            case Starting:
                transport.start();
                // enable stop button
                break;
            case Stopping:
                transport.stop();
                break;
                // enable start button
            default:
                break;
        }
    }
}

//==============================================================================
// processing
//==============================================================================
void TransitionFxAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    // clear buffer
    buffer.clear();
    
    // sample playing
    if (playSampleHit == true) {
        if (readerSource.get() == nullptr)
        {
            ASCI.clearActiveBufferRegion();
            return;
        }
        
        // set buffer
        ASCI.buffer = &buffer;
        ASCI.startSample = 0;
        ASCI.numSamples = buffer.getNumSamples();
        
        // set gain
        transport.setGain(pow(10, *tree.getRawParameterValue("kickSampleGainSliderID") / 20));

        transport.getNextAudioBlock(ASCI);
    }
    else {
        ASCI.clearActiveBufferRegion();
    }

    // manual trigger button
    
    // stop note
    if (manualTrigger == 2) {
        manualTrigger = 0;
        mySynth.manualTrigger(0);
    }
    // start note
    else if (manualTrigger == 1) {
        manualTrigger = 2;
        mySynth.manualTrigger(1);
    }
    
    //==============================================================================
    // get and send DAW information
    //==============================================================================
    // used for beatlinking
    AudioPlayHead* phead = getPlayHead();
    if (phead != nullptr)           // if there is a host
    {
        AudioPlayHead::CurrentPositionInfo playposinfo;
        phead->getCurrentPosition(playposinfo);
        
        // use info from playposinfo
        bpm = &playposinfo.bpm;
        framerate = playposinfo.frameRate;
        timeSigNum = playposinfo.timeSigNumerator;
        timeSigDenom = playposinfo.timeSigDenominator;
        currentPlayheadPosition = playposinfo.ppqPosition;
        isDawPlaying = playposinfo.isPlaying;
        hostLinked = true;

        // give linking data to synth
        mySynth.getLinkingData(isDawPlaying, currentPlayheadPosition, *tree.getRawParameterValue("linkOnOffButtonID"));

        // set note off
        if (isNoteOff == 1) {
            mySynth.getWaitingNote(0);
            isNoteOff = 0;
        }
        
        // set start position
        double minusLinkValue;
        timeLinkSelection = *tree.getRawParameterValue("timeLinkMenuID");

        // check timelink selection and set the minus link value (in beats)
        if (timeLinkSelection == 0){
            minusLinkValue = (*tree.getRawParameterValue("masterAttackSliderID") / noteTimeValue);
        }
        if (timeLinkSelection == 1 || timeLinkSelection == 2 || timeLinkSelection == 3) {
            if (timeLinkSelection == 1) {
                minusLinkValue = *tree.getRawParameterValue("masterAttackBeatsNotesSliderID") / 2;
            }
            else if (timeLinkSelection == 2) {
                minusLinkValue = *tree.getRawParameterValue("masterAttackBeatsNotesSliderID");
            }
            else {
                minusLinkValue = *tree.getRawParameterValue("masterAttackBeatsNotesSliderID") * timeSigNum;
            }
        }
        else if (timeLinkSelection == 4) {                                                         // samples
            minusLinkValue = (*tree.getRawParameterValue("masterAttackSamplesSliderID") / sRate * 1000 / noteTimeValue);
        }
        else {
            minusLinkValue = (*tree.getRawParameterValue("masterAttackFramesSliderID") / usedFramerate * 1000 / noteTimeValue);
        }

        linkStartPositionInBeats = *tree.getRawParameterValue("LinkTimeSliderID") - minusLinkValue;
        
        // set note on
        if (currentPlayheadPosition >= linkStartPositionInBeats && notePlayed == false) {
            mySynth.getWaitingNote(1);
            isNoteOff = 1;
            notePlayed = true;
        }

        // check if delayed note should be played
        if (mySynth.dawPlayingLinkingTrigger != oldEnvTrigger) {
            oldEnvTrigger = mySynth.dawPlayingLinkingTrigger;
            // if trigger = 1
            if (mySynth.dawPlayingLinkingTrigger == 1) {
                notePlayed = false;
                DBG("noteplayed");
            }
        }
    }
    
    
    //==============================================================================
    // set/convert envelope time values
    //==============================================================================
    
    // check if there's a host, otherwise set parameters maunally
    if (hostLinked == false) {
        noteTimeValue = 60000 / 120;
        timeSigNum = 4;
        timeSigDenom = 4;
        sRate = 44100;
    }
    else {
        noteTimeValue = 60000 / *bpm;
        sRate = getSampleRate();
        // set framerate
        if (framerate == 0){
            usedFramerate = 23.976;
        }
        else if (framerate == 1) {
            usedFramerate = 24;
        }
        else if (framerate == 2) {
            usedFramerate = 25;
        }
        else if (framerate == 3) {
            usedFramerate = 29.97;
        }
        else if (framerate == 4) {
            usedFramerate = 30;
        }
        else {
            usedFramerate = 24;
        }
    }
    
    // set the timelinkselection
    timeLinkSelection = *tree.getRawParameterValue("timeLinkMenuID");
    
    // check timelink selection and set attack and release
    if (timeLinkSelection == 0){                                                            // ms
        masterAttack = *tree.getRawParameterValue("masterAttackSliderID");
        masterRelease = *tree.getRawParameterValue("masterReleaseSliderID");
    }
    else if (timeLinkSelection == 1 || timeLinkSelection == 2 || timeLinkSelection == 3) {  // beats
        // set wholevalue value based on denominator
        float wholeValue;
        if (timeSigDenom == 8) {
            wholeValue = noteTimeValue / 2;
        }
        else if (timeSigDenom == 2) {
            wholeValue = noteTimeValue * 2;
        }
        else if (timeSigDenom == 4) {
            wholeValue = noteTimeValue;
        }
        else {  // denom not recognised
            wholeValue = noteTimeValue;
        }
        // set lengths based on wholevalue
        if (timeLinkSelection == 1) {
            masterAttack = *tree.getRawParameterValue("masterAttackBeatsNotesSliderID") * (wholeValue / 2);
            masterRelease = *tree.getRawParameterValue("masterReleaseBeatsNotesSliderID") * (wholeValue / 2);
        }
        else if (timeLinkSelection == 2) {
            masterAttack = *tree.getRawParameterValue("masterAttackBeatsNotesSliderID") * (wholeValue);
            masterRelease = *tree.getRawParameterValue("masterReleaseBeatsNotesSliderID") * (wholeValue);
        }
        else {
            masterAttack = *tree.getRawParameterValue("masterAttackBeatsNotesSliderID") * (wholeValue * timeSigNum);
            masterRelease = *tree.getRawParameterValue("masterReleaseBeatsNotesSliderID") * (wholeValue * timeSigNum);
        }
    }
    else if (timeLinkSelection == 4) {                                                         // samples
        masterAttack = (*tree.getRawParameterValue("masterAttackSamplesSliderID") / sRate) * 1000;
        masterRelease = (*tree.getRawParameterValue("masterReleaseSamplesSliderID") / sRate) * 1000;
    }
    else {                                                                                     // framerate
        masterAttack = (*tree.getRawParameterValue("masterAttackFramesSliderID") / usedFramerate) * 1000;
        masterRelease = (*tree.getRawParameterValue("masterReleaseFramesSliderID") / usedFramerate) * 1000;
    }
    
    
    //==============================================================================
    // if voice is cast as synt voice, relay information (set values from input via valuetreestate class)
    for (int i = 0; i < mySynth.getNumVoices(); i++) {
        // check which voice is being edited
        if ((myVoice = dynamic_cast<SynthVoice*>(mySynth.getVoice(i)))){
            
            // get samplerates
            myVoice->setSamplerate(getSampleRate());
            
            // setup processors in voices
            myVoice->setupProcessor(this);
            
            // initialise settings for voices (set parameters for every voice)
            if (firstInit[i] == true) {
                initVoiceValue[i] = initVoiceValue[i] + 1;
                
                // set voicevalue
                voiceValue = &initVoiceValue[i];
                voiceAmountValue = &initVoiceValue[i];;
                
                // if every voice is initialised, set firstinit to false and exit if statement
                if (initVoiceValue[i] > 2) {
                    firstInit[i] = false;
                }
            }
            // if initialisation phase is done
            else {
                voiceValue = tree.getRawParameterValue("voicesSliderID");
                voiceAmountValue = tree.getRawParameterValue("voiceAmountSliderID");
            }
            
            // master 2
            myVoice->getSynthMasterParameters(voiceAmountValue, voiceValue);
            
            // VOICE SPECIFIC VALUES
            // lfo2
            if (*voiceValue == 1){
                for (int ii = 0; ii < 2; ii++){
                    lfoMenuBIDs[ii] = *tree.getRawParameterValue(lfo2Menus[0][ii]);
                    lfoFreqSliderBIDs[ii] = *tree.getRawParameterValue(lfo2FreqSliders[0][ii]);
                }
            }
            else if (*voiceValue == 2){
                for (int ii = 0; ii < 2; ii++){
                    lfoMenuBIDs[ii] = *tree.getRawParameterValue(lfo2Menus[1][ii]);
                    lfoFreqSliderBIDs[ii] = *tree.getRawParameterValue(lfo2FreqSliders[1][ii]);
                }
            }
            else {
                for (int ii = 0; ii < 2; ii++){
                    lfoMenuBIDs[ii] = *tree.getRawParameterValue(lfo2Menus[2][ii]);
                    lfoFreqSliderBIDs[ii] = *tree.getRawParameterValue(lfo2FreqSliders[2][ii]);
                }
            }
            myVoice->getLfo1VoicesParameters(lfoMenuBIDs[0], lfoFreqSliderBIDs[0]);
            myVoice->getLfo1VoicesParametersDetune(lfoMenuBIDs[1], lfoFreqSliderBIDs[1]);

            
            if (*voiceValue == 1){
                // osc
                myVoice->getOscPitchOffsetVoices(tree.getRawParameterValue(osc1AmountOfOscVoicesIDS[0]), tree.getRawParameterValue(osc1VoicesPitchOffsetIDS[0]));
            }
            else if (*voiceValue == 2){
                // osc
                myVoice->getOscPitchOffsetVoices(tree.getRawParameterValue(osc1AmountOfOscVoicesIDS[1]), tree.getRawParameterValue(osc1VoicesPitchOffsetIDS[1]));
            }
            else {
                // osc
                myVoice->getOscPitchOffsetVoices(tree.getRawParameterValue(osc1AmountOfOscVoicesIDS[2]), tree.getRawParameterValue(osc1VoicesPitchOffsetIDS[2]));
            }
            
            // create arrays to give to voices
            int vn = *voiceValue - 1; // voicenumber
            
            // osc
            float* oscMenuSelections[4] = {tree.getRawParameterValue(oscMenus[vn][0]), tree.getRawParameterValue(oscMenus[vn][1]), tree.getRawParameterValue(oscMenus[vn][2]), tree.getRawParameterValue(oscMenus[vn][3])};
            float* oscDetuneSelections[4] = {tree.getRawParameterValue(oscDetunes[vn][0]), tree.getRawParameterValue(oscDetunes[vn][1]), tree.getRawParameterValue(oscDetunes[vn][2]), tree.getRawParameterValue(oscDetunes[vn][3])};
            float* oscGainSelections[4] = {tree.getRawParameterValue(oscGains[vn][0]), tree.getRawParameterValue(oscGains[vn][1]), tree.getRawParameterValue(oscGains[vn][2]), tree.getRawParameterValue(oscGains[vn][3])};
            myVoice->getAllOsc1Parameters(oscMenuSelections, oscDetuneSelections, oscGainSelections);
            
            // env2
            myVoice->getEnv2ParametersDetuneV(tree.getRawParameterValue(env2AttackSlidersP[vn][0]), tree.getRawParameterValue(env2ReleaseSlidersP[vn][0]), tree.getRawParameterValue(env2DopplerButtonsP[vn][0]), tree.getRawParameterValue(env2OnOffButtonsP[vn][0]), tree.getRawParameterValue(env2DelaySlidersP[vn][0]), tree.getRawParameterValue(env2ForwardsButtonsP[vn][0]), tree.getRawParameterValue(env2SustainButtonsP[vn][0]));
            myVoice->getEnv2ParametersOVDV(tree.getRawParameterValue(env2AttackSlidersP[vn][1]), tree.getRawParameterValue(env2ReleaseSlidersP[vn][1]), tree.getRawParameterValue(env2DopplerButtonsP[vn][1]), tree.getRawParameterValue(env2OnOffButtonsP[vn][1]), tree.getRawParameterValue(env2DelaySlidersP[vn][1]), tree.getRawParameterValue(env2ForwardsButtonsP[vn][1]), tree.getRawParameterValue(env2SustainButtonsP[vn][1]));
            myVoice->getEnv2ParametersPanV(tree.getRawParameterValue(env2AttackSlidersP[vn][2]), tree.getRawParameterValue(env2ReleaseSlidersP[vn][2]), tree.getRawParameterValue(env2DopplerButtonsP[vn][2]), tree.getRawParameterValue(env2OnOffButtonsP[vn][2]), tree.getRawParameterValue(env2DelaySlidersP[vn][2]), tree.getRawParameterValue(env2ForwardsButtonsP[vn][2]), tree.getRawParameterValue(env2SustainButtonsP[vn][2]));
            myVoice->getEnv2ParametersGainV(tree.getRawParameterValue(env2AttackSlidersP[vn][3]), tree.getRawParameterValue(env2ReleaseSlidersP[vn][3]), tree.getRawParameterValue(env2DopplerButtonsP[vn][3]), tree.getRawParameterValue(env2OnOffButtonsP[vn][3]), tree.getRawParameterValue(env2DelaySlidersP[vn][3]), tree.getRawParameterValue(env2ForwardsButtonsP[vn][3]));

            // GENERAL VALUES
            // env1
            myVoice->getEnv1Parameters(tree.getRawParameterValue("env1MenuID"), &masterAttack, &masterRelease, tree.getRawParameterValue("env1SmoothOnOffButtonID"), tree.getRawParameterValue("env1SmoothAmountSliderID"));
            
            // create arrays to give to voices
            float* env1Attacks[6] = {tree.getRawParameterValue("env1PointSlider1ID"), tree.getRawParameterValue("env1PointSlider2ID"), tree.getRawParameterValue("env1PointSlider3ID"), tree.getRawParameterValue("env1PointSlider4ID"), tree.getRawParameterValue("env1PointSlider5ID"),tree.getRawParameterValue("env1PointSlider6ID")};
            float* env1Releases [6] = {tree.getRawParameterValue("env1PointReleaseSlider1ID"), tree.getRawParameterValue("env1PointReleaseSlider2ID"), tree.getRawParameterValue("env1PointReleaseSlider3ID"), tree.getRawParameterValue("env1PointReleaseSlider4ID"), tree.getRawParameterValue("env1PointReleaseSlider5ID"),tree.getRawParameterValue("env1PointReleaseSlider6ID")};
            myVoice->getEnv1AR8StepsParameters(env1Attacks, env1Releases);
            
            // give correct value
            float LFORange[2] = {*tree.getRawParameterValue(env3LowRanges[0]),*tree.getRawParameterValue(env3HighRanges[0])};
            
            // env 3
            myVoice->getEnv2ParametersLFO( tree.getRawParameterValue(env3AttackSlidersP[0]), tree.getRawParameterValue(env3ReleaseSliderP[0]), tree.getRawParameterValue(env3DopplerButtonsP[0]), tree.getRawParameterValue(env3OnOffButtonsP[0]), tree.getRawParameterValue(env3DelaySliderP[0]), tree.getRawParameterValue(env3ForwardsButtonsP[0]), LFORange, tree.getRawParameterValue(env3SustainButtonsP[0]));
            myVoice->getEnv2ParametersPan( tree.getRawParameterValue(env3AttackSlidersP[2]), tree.getRawParameterValue(env3ReleaseSliderP[2]), tree.getRawParameterValue(env3DopplerButtonsP[2]), tree.getRawParameterValue(env3OnOffButtonsP[2]), tree.getRawParameterValue(env3DelaySliderP[2]), tree.getRawParameterValue(env3ForwardsButtonsP[2]), tree.getRawParameterValue(env3SustainButtonsP[2]));
            myVoice->getEnv2ParametersDetune(tree.getRawParameterValue(env3AttackSlidersP[5]), tree.getRawParameterValue(env3ReleaseSliderP[5]), tree.getRawParameterValue(env3DopplerButtonsP[5]), tree.getRawParameterValue(env3OnOffButtonsP[5]), tree.getRawParameterValue(env3DelaySliderP[5]), tree.getRawParameterValue(env3ForwardsButtonsP[5]), tree.getRawParameterValue(env3SustainButtonsP[5]));
            
            // effects
            myVoice->getDistortionParameters(tree.getRawParameterValue("distortionOnOffButtonID"), tree.getRawParameterValue("distortionDriveSliderID"), tree.getRawParameterValue("distortionRangeSliderID"), tree.getRawParameterValue("distortionBlendSliderID"), tree.getRawParameterValue("distortionGainSliderID"), tree.getRawParameterValue("distortionMenuID"));
            // master
            // create arrays to give to voices
            float* masterVoiceGains[3] = {tree.getRawParameterValue(voiceMixSliderIDsP[0]), tree.getRawParameterValue(voiceMixSliderIDsP[1]), tree.getRawParameterValue(voiceMixSliderIDsP[2])};
            float* masterVoiceMutes[3] = {tree.getRawParameterValue(voiceMuteButtonIDsP[0]), tree.getRawParameterValue(voiceMuteButtonIDsP[1]), tree.getRawParameterValue(voiceMuteButtonIDsP[2])};
            float* masterVoiceSolos[3] = {tree.getRawParameterValue(voiceSoloButtonIDsP[0]), tree.getRawParameterValue(voiceSoloButtonIDsP[1]), tree.getRawParameterValue(voiceSoloButtonIDsP[2])};
            float* masterVoicePans[3]  = {tree.getRawParameterValue(voicePanSliderIDsP[0]), tree.getRawParameterValue(voicePanSliderIDsP[1]), tree.getRawParameterValue(voicePanSliderIDsP[2])};
            
            myVoice->getMasterParameters2(tree.getRawParameterValue("masterGainSliderID"), tree.getRawParameterValue("masterPanSliderID"), masterVoiceGains, masterVoiceMutes, masterVoiceSolos, masterVoicePans);
            
            // lfo
            myVoice->getLfo1Parameters(tree.getRawParameterValue("lfo1MenuID"),tree.getRawParameterValue("lfo1FreqSliderID"));
            // hits
            myVoice->getHitsParameters(tree.getRawParameterValue("kickPitchSliderID"), tree.getRawParameterValue("kickReleaseSliderID"), tree.getRawParameterValue("kickOnOffButtonID"), tree.getRawParameterValue("kickGainSliderID"), tree.getRawParameterValue("kickTempoOffsetSliderID"), tree.getRawParameterValue("kickDistortionOnOffButtonID"));
        }
    } // end forloop walktrough voices
    
    
    //==============================================================================
    
    // cal proccesor located in SyntVoice.h
    mySynth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    
    //==============================================================================
    // set samplerates
    env3RWet.setSampleRate(getSampleRate());
    env1FCutOff.setSampleRate(getSampleRate());
    env2FResonance.setSampleRate(getSampleRate());
    delayEnv.setSampleRate(getSampleRate());
    
    //==============================================================================
    // note on check
    
    // set a delay to give a trigger when the sound is done playing
    delayEnv.attackLength = masterAttack + masterRelease;
    
    for (int sample = 0; sample < buffer.getNumSamples(); sample++) {   // for every sample
        delayedTrigger = delayEnv.delay(1., envtrig);
        // if dawplaying isnt true, otherwise activate linking and only start when note actually starts
        if (isDawPlaying != true) {
            if (delayedTrigger == 1) {
                isNoteOn = false;
                mySynth.setNoteIsPlaying(isNoteOn);
            }
            if (envtrig == 1) {
                isNoteOn = true;
                mySynth.setNoteIsPlaying(isNoteOn);
            }
        }
        else {
            isNoteOn = false;
            mySynth.setNoteIsPlaying(isNoteOn);
        }
    }
    
    //==============================================================================
    // sample trigger
    if (playSampleHit == true) {
        sampleHitDelay.attackLength = masterAttack + (*tree.getRawParameterValue("kickTempoOffsetSliderID") * masterRelease);
        bool playSample = false;
        for (int sample = 0; sample < buffer.getNumSamples(); sample++) {   // for every sample
            sampleDelayedTrigger = sampleHitDelay.delay(1.0, envtrig);
            if (sampleDelayedTrigger == 1) {
                playSample = true;
            }
        }
        if (playSample == true) {
            transportStateChanged(Stopped);
            transportStateChanged(Starting);
        }
    }
    
    //==============================================================================
    // if reverb is on
    if (*tree.getRawParameterValue("reverbOnOffButtonID") == 1) {

        // get parameters env
        reverbEnvOnOffButton = *tree.getRawParameterValue(env3OnOffButtonsP[4]);
        reverbDopplerButton  = *tree.getRawParameterValue(env3DopplerButtonsP[4]);
        
        // get parameters reverb
        float verbRoomSize  = *tree.getRawParameterValue(reverbSliderIDs[0]);
        float verbDry       = *tree.getRawParameterValue(reverbSliderIDs[2]);
        float verbDamping   = *tree.getRawParameterValue(reverbSliderIDs[3]);
        float verbWidth     = *tree.getRawParameterValue(reverbSliderIDs[4]);
        verbWet = *tree.getRawParameterValue(reverbSliderIDs[1]);
        
        // check if envelope in on
        if (reverbEnvOnOffButton == 1) {
            float verbDelay = *tree.getRawParameterValue(env3DelaySliderP[4]);

            // check if link toggle is on
            if (reverbDopplerButton == 1){             // link is on
                env3RWet.setAttackLinDelay(masterAttack, 0);
                env3RWet.setReleaseLin(masterRelease);
            }
            else {                                     // link is off
                env3RWet.setAttackLinDelay((*tree.getRawParameterValue(env3AttackSlidersP[4]) * masterAttack) - verbDelay, verbDelay);
                env3RWet.setReleaseLin(*tree.getRawParameterValue(env3ReleaseSliderP[4]) * masterRelease);
                env3RWet.attackLength = masterAttack;
            }
            env3RWet.darsRelease = false;

            // for every sample check trigger and calculate enveloped sound
            for (int sample = 0; sample < buffer.getNumSamples(); sample++) {
                verbWetOut = env3RWet.dars(verbWet, envtrig);
            }
        }
        // no envelope
        else {
            verbWetOut = verbWet;
        }

        // set parameters
        reverbParameters.roomSize = verbRoomSize;
        reverbParameters.wetLevel = verbWetOut;
        reverbParameters.dryLevel = verbDry;
        reverbParameters.damping  = verbDamping;
        reverbParameters.width    = verbWidth;
        
        // give parameters to reverb for every sample
        for (int sample = 0; sample < buffer.getNumSamples(); sample++) {
            auto& verb = fxChain.template get<reverbIndex>();
            verb.setParameters(reverbParameters);
        }

        // set output
        auto verbBlock = juce::dsp::AudioBlock<float> (buffer);
        auto blockToUse = verbBlock.getSubBlock (0, buffer.getNumSamples());
        auto contextToUse = juce::dsp::ProcessContextReplacing<float> (blockToUse);
        fxChain.process(contextToUse);
    }
    
    // if reverb is off, reset
    else {
        fxChain.reset();
    }
    
    //==============================================================================
    // set parameters
    filterCutOff = *tree.getRawParameterValue("filterCutOffSliderID");
    filterOnOffButton = *tree.getRawParameterValue("filterOnOffButtonID");
    filterDopplerButton = *tree.getRawParameterValue(env3DopplerButtonsP[1]);
    filterEnvOnOffButton = *tree.getRawParameterValue(env3OnOffButtonsP[1]);

    filterResonance = *tree.getRawParameterValue("filterResonanceSliderID");
    filterResonanceDopplerButton = *tree.getRawParameterValue(env3DopplerButtonsP[3]);
    filterResonanceOnOffButton = *tree.getRawParameterValue(env3OnOffButtonsP[3]);
    
    // set filter if filter is on
    if (filterOnOffButton == 1.0f){
        // cutoff envelope
        if (filterEnvOnOffButton == 1){
            // set delay
            double delayF = *tree.getRawParameterValue(env3DelaySliderP[1]) * (*tree.getRawParameterValue(env3AttackSlidersP[1]) * masterAttack);

            // check if link toggle is on
            if (filterDopplerButton == 1){             // link is on
                env1FCutOff.setAttackLinDelay(masterAttack, 0);
                env1FCutOff.setReleaseLin(masterRelease);
            }
            else {                                      // link is off
                env1FCutOff.setAttackLinDelay((*tree.getRawParameterValue(env3AttackSlidersP[1]) * masterAttack) - delayF, delayF);
                env1FCutOff.setReleaseLin(*tree.getRawParameterValue(env3ReleaseSliderP[1]) * masterRelease);
                env1FCutOff.attackLength = masterAttack;
            }
            
            // set release on or off
            env1FCutOff.darsRelease = false;
            
            // add enveloped sound for every sample (has to be for every sample, because envelope works with samples)
            for (int sample = 0; sample < buffer.getNumSamples(); sample++) {
                filterCutOffOut = env1FCutOff.dars(filterCutOff, envtrig) + 0.0001;
            }
        }
        // no envelope
        else {
            filterCutOffOut = filterCutOff;
        }
        
        // check to make sure that that cutoff wont go below 20
        if (filterCutOffOut < 20) {
            filterCutOffOut = 20;
        }
        
        // resonance envelope
        if (filterResonanceOnOffButton){
            // check if link toggle is on
            if (filterResonanceDopplerButton == 1){       // link is on
                env2FResonance.setAttackLin(masterAttack);
                env2FResonance.setReleaseLin(masterRelease);
            }
            else     {                                                                      // link is off
                env2FResonance.setAttackLin(*tree.getRawParameterValue(env3AttackSlidersP[3]) * masterAttack);
                env2FResonance.setReleaseLin(*tree.getRawParameterValue(env3ReleaseSliderP[3]) * masterRelease);
                env2FResonance.attackLength =  masterAttack;
            }
            // set release on or off
            env2FResonance.darsRelease = false;
            
            // add enveloped sound for every sample
            for (int sample = 0; sample < buffer.getNumSamples(); sample++) {
                filterResonanceOut = env2FResonance.dars(filterResonance, envtrig) + 0.0001;
            }
        }
        // no envelope
        else {
            filterResonanceOut = filterResonance;
        }
        
        // check to make sure that resonance wont go below 20
        if (filterResonanceOut < 0){
            filterResonanceOut = 0;
        }

        // set state for every sample
        for (int sample = 0; sample < buffer.getNumSamples(); sample++) {
            *lowPassFilter.state = *dsp::IIR::Coefficients<float>::makeLowPass(lastSampleRate, filterCutOffOut, filterResonanceOut);
        }
        
        // create block from audio and put it through the filter
        dsp::AudioBlock<float> block (buffer);
        lowPassFilter.process(dsp::ProcessContextReplacing <float> (block));
    }
}   // end of processBlock

//==============================================================================
void TransitionFxAudioProcessor::setEnvTrigger(int trigger)
{
    envtrig = trigger;
    std::cout << "Trigger: " << envtrig << std::endl;
}

double TransitionFxAudioProcessor::getEnvTrigger()
{
    return envtrig;
}

//==============================================================================
bool TransitionFxAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* TransitionFxAudioProcessor::createEditor()
{
    return new TransitionFxAudioProcessorEditor (*this);
}

//==============================================================================
void TransitionFxAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    ScopedPointer<XmlElement> xml (tree.state.createXml());
    copyXmlToBinary(*xml, destData);
}

void TransitionFxAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    if (saveXml == true){
        //get xml from binary
        ScopedPointer<XmlElement> parameters (getXmlFromBinary(data, sizeInBytes));
        //errorcheck
        if (parameters != nullptr){
            if (parameters -> hasTagName(tree.state.getType())){
                tree.state = ValueTree::fromXml(*parameters);
            }
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TransitionFxAudioProcessor();
}
