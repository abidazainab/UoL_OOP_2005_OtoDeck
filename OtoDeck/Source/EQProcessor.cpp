
/*
  ==============================================================================

    EQProcessor.cpp
    Created: 30 Aug 2024 5:20:01pm
    Author:  Admin

  ==============================================================================
*/

#include "EQProcessor.h" // Include the header file that declares the EQProcessor class

// Constructor for the EQProcessor class
EQProcessor::EQProcessor()
{
    // Constructor body is empty because no initialisation logic is required here.
}

// Destructor for the EQProcessor class
EQProcessor::~EQProcessor()
{
    // Destructor body is also empty as no cleanup of resources is necessary.
}

// Prepare the processor before playback starts. This method is called to initialise the audio processing environment.
void EQProcessor::prepareToPlay(double _sampleRate, int samplesPerBlock)
{
    sampleRate = _sampleRate; // Store the sample rate, which is needed for filter coefficient calculations
    updateFilters(); // Update filter coefficients based on initial settings (slider values)
}

// Process audio samples block by block. This is where the filters are applied to the audio buffer.
void EQProcessor::processBlock(juce::AudioBuffer<float>& buffer)
{
    // Loop through each audio channel in the buffer (stereo or more)
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        // Apply the bass filter to the current channel's audio samples
        bassFilter.processSamples(buffer.getWritePointer(channel), buffer.getNumSamples());

        // Apply the mid filter to the current channel's audio samples
        midFilter.processSamples(buffer.getWritePointer(channel), buffer.getNumSamples());

        // Apply the treble filter to the current channel's audio samples
        trebleFilter.processSamples(buffer.getWritePointer(channel), buffer.getNumSamples());
    }
}

// Method to set the bass slider value (range between 0.0 and 1.0) and update the filter coefficients accordingly
void EQProcessor::setBassValue(float value)
{
    bassSliderValue = value; // Store the new value for the bass slider
    updateFilters(); // Recalculate the filter coefficients based on the new slider value
}

// Method to set the mid slider value (range between 0.0 and 1.0) and update the filter coefficients accordingly
void EQProcessor::setMidValue(float value)
{
    midSliderValue = value; // Store the new value for the mid slider
    updateFilters(); // Recalculate the filter coefficients based on the new slider value
}

// Method to set the treble slider value (range between 0.0 and 1.0) and update the filter coefficients accordingly
void EQProcessor::setTrebleValue(float value)
{
    trebleSliderValue = value; // Store the new value for the treble slider
    updateFilters(); // Recalculate the filter coefficients based on the new slider value
}

// Private method to update the filter coefficients based on the current slider values and the sample rate
void EQProcessor::updateFilters()
{
    // Convert the bass slider value (0.0 to 1.0) to a frequency range (30 Hz to 250 Hz)
    float bassFreq = juce::jmap(bassSliderValue, 30.0f, 250.0f);

    // Convert the mid slider value (0.0 to 1.0) to a frequency range (250 Hz to 4000 Hz)
    float midFreq = juce::jmap(midSliderValue, 250.0f, 4000.0f);

    // Convert the treble slider value (0.0 to 1.0) to a frequency range (4000 Hz to 18000 Hz)
    float trebleFreq = juce::jmap(trebleSliderValue, 4000.0f, 18000.0f);

    // Map the bass slider value to a gain range (0 dB to 1 dB)
    float bassGain = juce::jmap(bassSliderValue, 0.0f, 1.0f);

    // Map the mid slider value to a gain range (0 dB to 3 dB)
    float midGain = juce::jmap(midSliderValue, 0.0f, 3.0f);

    // Map the treble slider value to a gain range (0 dB to 10 dB)
    float trebleGain = juce::jmap(trebleSliderValue, 0.0f, 10.0f);

    // Set the bass filter coefficients using a low-shelf filter (boosts or cuts low frequencies)
    bassFilter.setCoefficients(juce::IIRCoefficients::makeLowShelf(
        sampleRate, bassFreq, 0.707f, juce::Decibels::decibelsToGain(bassGain)));

    // Set the mid filter coefficients using a peak filter (boosts or cuts a specific frequency range)
    midFilter.setCoefficients(juce::IIRCoefficients::makePeakFilter(
        sampleRate, midFreq, 0.707f, juce::Decibels::decibelsToGain(midGain)));

    // Set the treble filter coefficients using a high-shelf filter (boosts or cuts high frequencies)
    trebleFilter.setCoefficients(juce::IIRCoefficients::makeHighShelf(
        sampleRate, trebleFreq, 0.707f, juce::Decibels::decibelsToGain(trebleGain)));
}
