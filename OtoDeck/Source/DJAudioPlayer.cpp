
/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 28 Jul 2024 3:48:29pm
    Author:  Admin

  ==============================================================================
*/

#include "DJAudioPlayer.h"  // Include the header file for DJAudioPlayer
#include <juce_core/juce_core.h> // Include this for DBG (debugging)

//==============================================================================
// Constructor: Initializes the DJAudioPlayer with the provided AudioFormatManager and prepares the EQProcessor.
DJAudioPlayer::DJAudioPlayer(AudioFormatManager& _formatManager)
    : formatManager(_formatManager), resampleSource(&transportSource, false)
{
    eqProcessor.prepareToPlay(44100, 512); // Prepare the EQProcessor with default sample rate and block size
}

// Destructor: Cleans up any resources used by the DJAudioPlayer.
DJAudioPlayer::~DJAudioPlayer()
{
}

// prepareToPlay: Prepares the audio player for playback by initializing buffers and sources.
// This method is called before playback starts to ensure everything is set up correctly.
void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    eqProcessor.prepareToPlay(sampleRate, samplesPerBlockExpected);
}

// getNextAudioBlock: Retrieves the next block of audio samples and processes it through the EQProcessor.
// This method is called continuously during playback to provide audio data to the output device.
void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    resampleSource.getNextAudioBlock(bufferToFill); // Get the audio block from the resampler
    eqProcessor.processBlock(*bufferToFill.buffer); // Apply EQ processing to the audio block

    // Calculate the RMS level for the left channel
    auto* channelData = bufferToFill.buffer->getReadPointer(0, bufferToFill.startSample);
    rmsLevel = calculateRMSLevel(channelData, bufferToFill.numSamples);
}

// releaseResources: Releases resources allocated during prepareToPlay.
// This method is called when playback stops or when the audio source is no longer needed.
void DJAudioPlayer::releaseResources()
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
}

// loadURL: Loads an audio file from the provided URL into the player.
// This method creates an AudioFormatReader for the file and sets it as the source for playback.
void DJAudioPlayer::loadURL(URL audioURL)
{
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader == nullptr) {
        DBG("Failed to create reader for the provided URL.");
        return;
    }
    std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader, true));
    transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
    readerSource.reset(newSource.release());
    loadedURL = audioURL;
}

// getLoadedURL: Returns the URL of the currently loaded audio file.
// @return The URL of the loaded audio file.
juce::URL DJAudioPlayer::getLoadedURL() const
{
    return loadedURL;
}

// setGain: Sets the gain (volume) of the audio playback.
// @param gain The gain value, should be between 0 and 1.
void DJAudioPlayer::setGain(double gain)
{
    if (gain < 0.0f) {
        transportSource.setGain(0.0f);
    }
    else if (gain > 1.0f) {
        transportSource.setGain(1.0f);
    }
    else {
        transportSource.setGain(gain);
    }
}

// setSpeed: Sets the playback speed by adjusting the resampling ratio.
// @param ratio The speed ratio, typically between 0.1 and 10.
void DJAudioPlayer::setSpeed(double ratio)
{
    if (ratio < 0.1 || ratio > 10.0) {
        std::cout << "DJAudioPlayer::setSpeed ratio should be between 0.1 to 10" << std::endl;
    }
    else {
        resampleSource.setResamplingRatio(ratio);
    }
}

// setPosition: Sets the playback position in seconds.
// @param posInSecs The position in seconds.
void DJAudioPlayer::setPosition(double posInSecs)
{
    transportSource.setPosition(posInSecs);
}

// setPositionRelative: Sets the playback position relative to the track length.
// @param pos The relative position, between 0 and 1.
void DJAudioPlayer::setPositionRelative(double pos)
{
    if (pos < 0 || pos > 1.0) {
        std::cout << "DJAudioPlayer::setPositionRelative pos should be between 0 to 1" << std::endl;
    }
    else {
        double posInSecs = pos * transportSource.getLengthInSeconds();
        setPosition(posInSecs);
    }
}

// start: Starts audio playback.
void DJAudioPlayer::start()
{
    transportSource.start();
}

// stop: Stops audio playback.
void DJAudioPlayer::stop()
{
    transportSource.stop();
}

// isPlaying: Checks if the audio is currently playing.
// @return True if playing, false otherwise.
bool DJAudioPlayer::isPlaying()
{
    return transportSource.isPlaying();
}

// getPositionRelative: Returns the current playback position relative to the track length.
// @return The relative position, between 0 and 1.
double DJAudioPlayer::getPositionRelative()
{
    double pos = transportSource.getLengthInSeconds() > 0 ? transportSource.getCurrentPosition() / transportSource.getLengthInSeconds() : 0;

    if (isnan(pos)) {
        return 0;
    }
    return pos;
}

// setBass: Sets the gain for the bass frequency band using the EQProcessor.
// @param value The gain value for the bass band in decibels.
void DJAudioPlayer::setBass(double value)
{
    eqProcessor.setBassValue(value);
}

// setMid: Sets the gain for the mid-frequency band using the EQProcessor.
// @param value The gain value for the mid band in decibels.

void DJAudioPlayer::setMid(double value)
{
    eqProcessor.setMidValue(value);
}

// setTreble: Sets the gain for the treble frequency band using the EQProcessor.
// @param value The gain value for the treble band in decibels.

void DJAudioPlayer::setTreble(double value)
{
    eqProcessor.setTrebleValue(value);
}

// setMixRatio: Sets the mix ratio for blending multiple audio sources.
// @param ratio The mix ratio, typically between 0 and 1.
void DJAudioPlayer::setMixRatio(double ratio)
{
    mixRatio = ratio;
    setGain(mixRatio);
}

// getCurrentPosition: Returns the current playback position in seconds.
// @return The current playback position in seconds.
double DJAudioPlayer::getCurrentPosition()
{
    return transportSource.getCurrentPosition();
}

// getTotalLength: Returns the total length of the loaded track in seconds.
// @return The total length in seconds.
double DJAudioPlayer::getTotalLength() const
{
    return transportSource.getLengthInSeconds();
}

// getTrackDuration: Returns the track duration formatted as "mm:ss".
// This method works only after the transportSource is initialized.
// @return The track duration as a string.
juce::String DJAudioPlayer::getTrackDuration()
{
    double tempLengthInMin = transportSource.getLengthInSeconds() / 60;
    double secToMin = (tempLengthInMin - floor(tempLengthInMin)) * 0.6;
    double _duration = (floor(tempLengthInMin) + secToMin);
    juce::String duration = juce::String(_duration).substring(0, 4).replaceCharacter('.', ':');
    return duration;
}

// getTrackDurationWithoutLoadingIntoDeck: Returns the track duration without initializing the transportSource.
// This method is useful for getting the duration without loading the track into the player.
// @param audioURL The URL of the audio file.
// @return The track duration as a string.
juce::String DJAudioPlayer::getTrackDurationWithoutLoadingIntoDeck(URL audioURL)
{
    juce::String duration;

    std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(audioURL.createInputStream(false)));

    if (reader != nullptr) // Good file!
    {
        double lengthInSeconds = reader->lengthInSamples / reader->sampleRate;
        int minutes = static_cast<int>(lengthInSeconds) / 60;
        int seconds = static_cast<int>(lengthInSeconds) % 60;

        juce::String formattedMinutes = juce::String(minutes);
        juce::String formattedSeconds = juce::String(seconds).paddedLeft('0', 2);

        duration = formattedMinutes + ":" + formattedSeconds;
    }
    else
    {
        DBG("Error: Could not load reader for the file.");
    }

    return duration;
}

// calculateRMSLevel: Calculates the RMS (Root Mean Square) level for a block of audio data.
// @param data Pointer to the audio data.
// @param numSamples Number of samples to process.
// @return The calculated RMS level.
float DJAudioPlayer::calculateRMSLevel(const float* data, int numSamples)
{
    float sumSquares = 0.0f;

    for (int i = 0; i < numSamples; ++i)
        sumSquares += data[i] * data[i];

    return std::sqrt(sumSquares / numSamples);
}

// getRMSLevel: Returns the last calculated RMS level.
// @return The RMS level.
float DJAudioPlayer::getRMSLevel() const
{
    return rmsLevel;
}
