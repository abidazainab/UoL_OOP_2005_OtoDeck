
/*
  ==============================================================================

    EQProcessor.h
    Created: 30 Aug 2024 5:20:01pm
    Author:  Admin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

// Class definition for the EQProcessor, which will handle basic EQ (Bass, Mid, Treble) processing
class EQProcessor
{
public:
    // Constructor: Initializes the EQProcessor
    EQProcessor();

    // Destructor: Cleans up when the object is destroyed
    ~EQProcessor();

    // Prepares the processor for playback, setting sample rate and buffer size
    void prepareToPlay(double sampleRate, int samplesPerBlock);

    // Main processing function to apply the EQ to the audio buffer passed in
    void processBlock(juce::AudioBuffer<float>& buffer);

    // Set functions for slider values that control frequency and gain of each band
    void setBassValue(float value);   // Adjusts the bass band
    void setMidValue(float value);    // Adjusts the mid band
    void setTrebleValue(float value); // Adjusts the treble band

private:
    // IIR Filters for each frequency band (bass, mid, treble)
    juce::IIRFilter bassFilter;
    juce::IIRFilter midFilter;
    juce::IIRFilter trebleFilter;

    // Variables representing the slider values that control the filter frequency and gain
    float bassSliderValue = 0.5f;   // Initial bass value; range between 0.0 (min) and 1.0 (max)
    float midSliderValue = 0.5f;    // Initial mid value; range between 0.0 (min) and 1.0 (max)
    float trebleSliderValue = 0.5f; // Initial treble value; range between 0.0 (min) and 1.0 (max)

    // The current sample rate (defaulting to 48kHz); used to configure the filters
    double sampleRate = 48000.0;

    // Helper function that updates the filter coefficients based on the current slider values
    void updateFilters();
};
