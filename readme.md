# TRANSITION FX

The goal of Transition FX is to create a synthesiser optimalised for creating transitional moving effects such as whooshes, risers and stingers. These effects usually consist of samples and/or synthesis. I found that there was a lack of a intuitive synthesiser that is optimalised for these effects, which is why I started developing Transition FX. The goal is to remove the need for automation. The user should be able to create the entire sound from within the synthesiser.

```diff
- TFX IS STILL UNDER DEVELOPMENT, ALL FEATURES ARE SUBJECT TO CHANGE.
```

<img width="641" alt="TransitionFX" src="https://user-images.githubusercontent.com/31696336/75611662-c88b3680-5b1c-11ea-967d-c5151b20fc68.png">

## Code hierarchy 
TFX is written in C++ using the JUCE framework and Maximilian DSP library.
NOTE: Some of the JUCE and Maximilian source code has been altered to allow for certain features, such as the JUCE synthesiser class, wich has been altered to allow for the 'beatlinking' functionality.

### Classes
TFX is divided into two main components, following the standard JUCE plugin structure. 
- The plugin editor handles all visuals and GUI elements
- The processor handles the audio processing and GUI callback
![TFX_Classes (1)](https://user-images.githubusercontent.com/31696336/75627990-604c5b80-5bd5-11ea-977b-2523eaaf05de.png)
DSP effects are added from Maximilian, JUCE::DSP and my own DSP class.

### Synth
The synthesiser consists of multiple voices, containing subvoices. This hierarchy of voices allows the user to make high impact decisions, with little effort. The graph visualizes the signal chain through the synthesiser.
![TFX_Synth](https://user-images.githubusercontent.com/31696336/75609793-b81e9000-5b0b-11ea-9a8d-ebb65640d3b7.png)

### Dataflow
The audio and interface are output seperatly.
![TFX_DataFlow](https://user-images.githubusercontent.com/31696336/75609831-05026680-5b0c-11ea-82bc-171d6a78d97c.png)

## Important features
A short overview of some interesting features.

### Beatlinking
When using transitional effects, the point of impact often is set in time (usually connected to image), while the attack and release time can be edited. It is suboptimal to have to reposition the audio file everytime the attack or release change. TFX offers 'beatlinking' as a solution. Beatlinking allows the user to lock the point of impact. When the attack or release time get changed, the point of impact stays the same.

<img width="641" alt="TFX_LengthAndLinking" src="https://user-images.githubusercontent.com/31696336/75610139-d0dc7500-5b0e-11ea-81bc-f52f6d0ef0d9.png">

This is done by triggering the note when the daw starts playback and calculating the delay time untill the attack needs to be played.

### Length
To be able to time the audio with visuals the length can be set in time, beat, frames and samples. 
<img width="612" alt="TFX_timeOptions" src="https://user-images.githubusercontent.com/31696336/75610211-709a0300-5b0f-11ea-9959-bfb02e58bfe2.png">

NOTE: Values such as framerate and samplerate are obtained from the DAW on which the plugin is running, however not every DAW allows acces to this information. In the future a manual preferences interface should be added as a solution.

### Master Amplitude Envelope
The main amplitude envelope is one of the most important aspects of a transitional effect, which is why it can be edited in 12 steps.

<img width="606" alt="TFX_Envelope" src="https://user-images.githubusercontent.com/31696336/75610274-13eb1800-5b10-11ea-9ab6-aa91d448a0c4.png">

Smooth creates a smoother curve between the 15 points, with a certain amount. The middle slider bar automatically fades the points from a 'exponential' to 'anticlimactic' curve. The release offset serves to lower the volume of the release in contrast to the attack and impact.

### More envelopes
Transitional effects are all about modulated sounds over time, aspects such as amplitude, pitch and filter cutoff should all be modulated in certain specific ways. For individual different aspects of the sound to modulate, different types of envelope modulation are more effective. Aspects such as amount of steps or linear vs exponential curves have to be considered for every parameter that can be modulated. Herefore the TFX Envelopes class contains multiple different types of envelopes.

```C++
class Envelopes {
    
public:
    // Envelope types
    // ar = attack, release and hold
    double arGemiddelde(double input, int trigger);     // ar average between exponential and linear
    double arExp(double input, int trigger);    // ar exponential
    double arLin(double input, int trigger);    // ar linear
    double arLinSteps(double input, int trigger);  // ar linear, 12 steps with adjustable x and y
    double dars(double input, int trigger);      // delay ar with optional release
```

The master amplitude envelope for example uses the arLinSteps function, in which the user is able to create a exponential curve by drawing a exponential curve with 12 linear points. To avoid being able to hear the corners created by connecting linear lines, the user can smooth the edges of the created exponential curve with the smooth and smooth amount functionalities.

All envelope modulation is done using statemachines, mostly consisting of 4 phases(states): attack, hold, release, stop. Because transitional effects usually don't have a sustain or decay, the master audio envelope does not make use of these. However, when the note is held down the sound should keep playing and the release shouldn't be triggerd, which is why the holdphase is added. A basic linear envelope:

```C++
double Envelopes::arLin(double input, int trigger)
{
    switch(currentEnvState) {
        case ATTACK:
            amplitude += (1 * attackLin);
            if (amplitude>=1) {
                amplitude=1;
                currentEnvState = HOLD;
            }
            break;
        case HOLD:
            amplitude = 1;
            if (trigger != 1) {
                currentEnvState = RELEASE;
            }
            break;
        case RELEASE:
            amplitude -= (1 * releaseLin);
            if (amplitude <= 0) {
                amplitude = 0;
                currentEnvState = STOP;
            }
            break;
        case STOP:
            amplitude = 0.0;
            if (trigger == 1) {
                currentEnvState = ATTACK;
            }
            break;
    }
    
    output = input * amplitude;
    return output;
}
```

The value by which the amplitude goes up or down during the attack and release is calculated in a different function.
```C+
    // Basic linear curve
    attackLin = ((1.0 / samplerate) * (1.0 / (attackMS / 1000.0)));

    // basic exponential curve
    attackExp = pow((1.0 / amplitudeStartValue), 1.0 / (samplerate * (attackMS / 1000.0)));
```


## Improvements
TFX is still under development. There are a lot of functionalities to be added. Right now the focus is on the optimalisation of the audio thread and converting to a more modular system to eventually be able to add and edit components easier.

Current MVP: 
- [ ] Complete refactor
- [ ] Restructure into a more modular system that allows easier edits and optimalisation
- [ ] Use listeners to remove as much as possible from the processblock

Important future points:
- [ ] Construct a more user friendly interface 
- [ ] Add more components, effects, waveforms, etc

## Notes
TFX Alpha is the first public version of TFX. 
