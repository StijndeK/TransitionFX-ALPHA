/*
  ==============================================================================

    SynthSound.h
    Created: 29 Oct 2018 10:57:41am
    Author:  Stijn de Koning

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "maximilian.h"



// class to describe the sounds the Synth can play
class SynthSound :  public SynthesiserSound
{
public:
    
    //Returns true if this sound should be played when a given midi note is pressed.
    bool appliesToNote (int /*midiNoteNumber*/)
    {
        return true;
    }
    
    //Returns true if the sound should be triggered by midi events on a given channel.
    bool appliesToChannel (int /*midiChannel*/)
    {
        return true;
    }
    
};
