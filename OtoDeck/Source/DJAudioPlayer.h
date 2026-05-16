
/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 28 Jul 2024 3:48:29pm
    Author:  Admin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>             // JUCE framework header for audio and utilities
#include "Filter.h"                 // Custom filter class (presumably for audio effects)
#include "EQProcessor.h"            // Custom equalizer class for processing audio EQ

//==============================================================================
// DJAudioPlayer is a custom class that handles audio playback with additional features like resampling, EQ adjustments, and track management.
// It implements the AudioSource interface to integrate with the JUCE audio system.
class DJAudioPlayer : public AudioSource // Inherits from AudioSource to manage audio playback
{
public:
    // Constructor: Initializes the DJAudioPlayer with a reference to an AudioFormatManager.
    // @param _formatManager Reference to an AudioFormatManager for handling audio file formats.
    DJAudioPlayer(AudioFormatManager& _formatManager);

    // Destructor: Cleans up resources used by the DJAudioPlayer.
    ~DJAudioPlayer() override;

    // prepareToPlay: Prepares the audio source for playback by setting up buffers and initializing components.
    // @param samplesPerBlockExpected Expected number of samples per block.
    // @param sampleRate The sample rate of the audio stream.
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

    // getNextAudioBlock: Retrieves the next block of audio samples to be played.
    // @param bufferToFill Reference to the AudioSourceChannelInfo object containing the buffer to be filled.
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;

    // releaseResources: Releases any resources that were allocated during prepareToPlay.
    void releaseResources() override;

    // loadURL: Loads an audio file from a given URL into the player.
    // @param audioURL The URL of the audio file to load.
    void loadURL(URL audioURL);

    // getLoadedURL: Returns the URL of the currently loaded audio file.
    // @return The URL of the loaded audio file.
    juce::URL getLoadedURL() const;

    // setGain: Sets the gain (volume) of the audio playback.
    // @param gain The gain value, should be between 0 and 1.
    void setGain(double gain);

    // setSpeed: Sets the playback speed by adjusting the resampling ratio.
    // @param ratio The speed ratio, typically between 0.1 and 10.
    void setSpeed(double ratio);

    // setPosition: Sets the playback position in seconds.
    // @param posInSecs The position in seconds.
    void setPosition(double posInSecs);

    // setPositionRelative: Sets the playback position relative to the track length.
    // @param pos The relative position, between 0 and 1.
    void setPositionRelative(double pos);

    // setBass: Sets the gain for the bass frequency band using the EQProcessor.
    // @param value The gain value for the bass band in decibels.
    void setBass(double value);

    // setMid: Sets the gain for the mid-frequency band using the EQProcessor.
    // @param value The gain value for the mid band in decibels.
    void setMid(double value);

    // setTreble: Sets the gain for the treble frequency band using the EQProcessor.
    // @param value The gain value for the treble band in decibels.
    void setTreble(double value);

    // getPositionRelative: Returns the current playback position relative to the track length.
    // @return The relative position, between 0 and 1.
    double getPositionRelative();

    // setMixRatio: Sets the mix ratio for blending multiple audio sources.
    // @param ratio The mix ratio, typically between 0 and 1.
    void setMixRatio(double ratio);

    // start: Starts audio playback.
    void start();

    // stop: Stops audio playback.
    void stop();

    // isPlaying: Checks if the audio is currently playing.
    // @return True if playing, false otherwise.
    bool isPlaying();

    // getCurrentPosition: Returns the current playback position in seconds.
    // @return The current playback position in seconds.
    double getCurrentPosition();

    // getTotalLength: Returns the total length of the loaded track in seconds.
    // @return The total length in seconds.
    double getTotalLength() const;

    // getTrackDuration: Returns the track duration formatted as "mm:ss".
    // This method works only after the transportSource is initialized.
    // @return The track duration as a string.
    juce::String getTrackDuration();

    // getTrackDurationWithoutLoadingIntoDeck: Returns the track duration without initializing the transportSource.
    // This method is useful for getting the duration without loading the track into the player.
    // @param audioURL The URL of the audio file.
    // @return The track duration as a string.
    juce::String getTrackDurationWithoutLoadingIntoDeck(URL audioURL);

    // calculateRMSLevel: Calculates the RMS (Root Mean Square) level for a block of audio data.
    // @param data Pointer to the audio data.
    // @param numSamples Number of samples to process.
    // @return The calculated RMS level.
    float calculateRMSLevel(const float* data, int numSamples);

    // getRMSLevel: Returns the last calculated RMS level.
    // @return The RMS level.
    float getRMSLevel() const;

private:
    AudioFormatManager& formatManager;  // Reference to the AudioFormatManager for handling audio formats
    std::unique_ptr<AudioFormatReaderSource> readerSource; // Smart pointer to manage the audio file reader
    AudioTransportSource transportSource; // Handles the playback of the audio
    ResamplingAudioSource resampleSource{ &transportSource , false, 2 }; // Resampling source to adjust playback speed

    juce::URL loadedURL;  // Stores the URL of the currently loaded audio file
    EQProcessor eqProcessor; // Handles the equalization of the audio signal

    double mixRatio; // The mix ratio for blending audio sources
    double gain; // The current gain (volume) level
    float rmsLevel = 0.0f; // The last calculated RMS level

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DJAudioPlayer) // Prevent copying and memory leaks
};
