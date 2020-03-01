# TRANSITION FX

Transition FX is a synthesiser optimalised for creating transitional moving effects such as whooshes, risers and stingers. 
These effects usually consist of samples and/or synthesis. I found that there was a lack of a intuitive synthesiser that is optimalised for these effects, which is why I started developing Transition FX.

```diff
- TFX IS STILL UNDER DEVELOPMENT, ALL FEATURES ARE SUBJECT TO CHANGE.
```

<img width="641" alt="TransitionFX" src="https://user-images.githubusercontent.com/31696336/75611662-c88b3680-5b1c-11ea-967d-c5151b20fc68.png">

## Code hierarchy 
TFX is made in C++ using the JUCE framework and Maximilian DSP library. 
NOTE: Some of the JUCE and Maximilian source code has been edited to allow for certain features, such as the JUCE synthesiser class.

### Classes
TFX is divided into two main components that come together in the Processor. 
- The plugin editor handles all visuals and GUI elements
- The processor handles the audio processing and GUI callback
- DSP effects are added from Maximilian, JUCE::DSP and my own DSP.cpp
![TFX_Classes (1)](https://user-images.githubusercontent.com/31696336/75627990-604c5b80-5bd5-11ea-977b-2523eaaf05de.png)

### Synth
The synthesiser consists of multiple voices, containing subvoices. This hierarchy of voices allows the user to make high impact changes, with little effort. ![TFX_Synth](https://user-images.githubusercontent.com/31696336/75609793-b81e9000-5b0b-11ea-9a8d-ebb65640d3b7.png)

### Dataflow
![TFX_DataFlow](https://user-images.githubusercontent.com/31696336/75609831-05026680-5b0c-11ea-82bc-171d6a78d97c.png)

## Important features
A short overview of some interesting features.
### Beat linking
When using transitional effects, the point of impact often is set in time (usually conected to image), while the attack and release time can be edited. It is suboptimal to have to reposition the audio file everytime the attack or release change. TFX offers 'beatlinking' as a solution. Beatlinking allows the user to lock the point of impact. When the attack or release time get changed, the point of impact stays the same.

<img width="641" alt="TFX_LengthAndLinking" src="https://user-images.githubusercontent.com/31696336/75610139-d0dc7500-5b0e-11ea-81bc-f52f6d0ef0d9.png">

### Length
To be able to time the audio with visuals the length can be set in time, beat, frames and samples.
<img width="612" alt="TFX_timeOptions" src="https://user-images.githubusercontent.com/31696336/75610211-709a0300-5b0f-11ea-9959-bfb02e58bfe2.png">

### Master Amplitude Envelope
The main amplitude envelope is one of the most important aspects of a transitional effect, which is why it can be edited in 14 steps. Smooth creates a smoother curve between the 14 points, with a certain amount. The middle slider bar automatically fades the points from a 'exponential' to 'anticlimactic' curve. The release offset serves to lower the volume of the release in contrary to the attack and impact. 

<img width="606" alt="TFX_Envelope" src="https://user-images.githubusercontent.com/31696336/75610274-13eb1800-5b10-11ea-9ab6-aa91d448a0c4.png">

### More envelopes
All envelope modulation is done using statemachines, mostly consisting of 3 phases(states): attackphase, holdphase, releasephase. Because transitional effects usually don't have a sustain or decay, the master audio envelope does not make use of these. However when the note is held down the sound should keep playing and the release shouldn't be triggerd, which is why the holdphase is added.

Transitional effects are all about modulated sounds, aspects as amplitude, pitch, filter cutoff should all be modulated in certain ways. For individual different aspects of the sound to modulate, different types of envelope modulation are more effective. Aspects such as amount of steps or linear vs exponential curves have to be considered for every parameter that can be modulated. Herefor I created multiple different types of envelopes within the Envelope class. 


```C++
class Envelopes {
    
public:
    // Envelope types
    // ar = attack, release and hold
    double arGemiddelde(double input, int trigger);     // ar average between exponential and linear
    double arExp(double input, int trigger);    // ar exponential
    double arLin(double input, int trigger);    // ar linear
    double arLin4Steps(double input, int trigger);  // ar linear, 4 steps with adjustable x and y
    double arLin12Steps(double input, int trigger);  // ar linear, 12 steps
    double dars(double input, int trigger);      // delay ar with optional release
```

The master amplitude envelope for example uses the arLin12Steps function, in which the user is able to create a exponential curve by drawing a exponential curve with these 12 linear points. To avoid hearing the curves created by conecting linear lines, the user can smooth the edges of the created exponential curve with the smooth and smooth amount functionalities.

## Improvements
TFX is still under heavy development. There are a lot of functionalities to be added. Right now my focus lies on optimalisation and converting to a more modular system.

Current MVP: 
- [ ] Restructure into a more modular system, that allows easier edits and optimalisation.
- [ ] Use listeners to remove as much as possible from the processblock
