# TRANSITION FX

Transition FX is a synthesiser optimalised for creating transitional moving effects such as whooshes, risers and stingers. 
These effects usually consist of samples and/or synthesis. I found that there was a lack of a intuitive synthesiser that is optimalised for these effects, which is why I started developing Transition FX.

```diff
- TFX IS STILL UNDER DEVELOPMENT, ALL FEATURES ARE SUBJECT TO CHANGE.
```
## Code hierarchy 
TFX is made in C++ using the JUCE framework and Maximilian DSP library. 
NOTE: Some of the JUCE and Maximilian source code has been edited to allow for certain features, such as the JUCE synthesiser class.

### class diagram
TFX is divided into two main components that come together in the Processor. 
- The plugin editor handles all visuals and GUI elements
- The processor handles the audio processing and GUI callback
![TFX_CLasses](https://user-images.githubusercontent.com/31696336/75609684-0bdca980-5b0b-11ea-8e0f-c8de5617e69a.png)

### Synth
The synthesiser consists out of 3 voices, consisting of subvoices. This hierarchy of voices allows the user to make high impact changes, with little effort. ![TFX_Synth](https://user-images.githubusercontent.com/31696336/75609793-b81e9000-5b0b-11ea-9a8d-ebb65640d3b7.png)

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

## Improvements
TFX is still under heavy development. There are a lot of functionalities to be added. Right now my focus lies on optimalisation and converting to a more modular system.

Current MVP: 
- [ ] Restructure into a more modular system, that allows easier edits and optimalisation.
- [ ] Use listeners to remove as much as possible from the processblock
