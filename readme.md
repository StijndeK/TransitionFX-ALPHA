# TRANSITION FX

Transition FX is a synthesiser optimalised for creating transitional moving effects such as whooshes, risers and stingers. 
These effects can consist of samples and/or synthesis. I found that there was a lack of a intuitive synthesiser that is optimalised for these effects, which is why I started developing Transition FX.

```diff
- TFX is still under development, all features are subject to change.
```
## Code hierarchy 

### class diagram
TFX is divided in to two main parts that come together in the Processor. 
- The plugin editor handles all visuals and gui
- The processor handles the audio processing
![TFX_CLasses](https://user-images.githubusercontent.com/31696336/75609684-0bdca980-5b0b-11ea-8e0f-c8de5617e69a.png)

### Synth
The synthesiser consists out of 3 voices, consisting of subvoices. This hierarchy of voices allows the user to make high impact edits, with little effort.
![TFX_Synth](https://user-images.githubusercontent.com/31696336/75609793-b81e9000-5b0b-11ea-9a8d-ebb65640d3b7.png)

### Dataflow
![TFX_DataFlow](https://user-images.githubusercontent.com/31696336/75609831-05026680-5b0c-11ea-82bc-171d6a78d97c.png)

## Important features
A short overview of interesting features.
### Beat linking
When using transitional effects, the point of impact often is set while the attack and release time can be edited. It is suboptimal to have to reposition the audio file everytime the attack or release get changed. TFX offers beatlinking as a solution. Beatlinking offers the option to lock the point of impact. When the attack or release time get changed, the point of impact stays the same.
<img width="641" alt="TFX_LengthAndLinking" src="https://user-images.githubusercontent.com/31696336/75610139-d0dc7500-5b0e-11ea-81bc-f52f6d0ef0d9.png">

### Length
To be able to time the audio with visuals the length can be set in time, beat, frames and samples.
<img width="612" alt="TFX_timeOptions" src="https://user-images.githubusercontent.com/31696336/75610211-709a0300-5b0f-11ea-9959-bfb02e58bfe2.png">

### Multiply osc

### Effects

## Interface

## Improvements

