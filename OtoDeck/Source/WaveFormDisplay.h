
/*
  ==============================================================================

    WaveFormDisplay.h
    Created: 29 Jul 2024 11:44:28am
    Author:  Admin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h> // JUCE framework header for audio and GUI functionalities
#include "DJAudioPlayer.h" // Custom class to control audio playback

//==============================================================================
// WaveFormDisplay is a GUI component that visualises the waveform of an audio file.
// It also responds to changes in the audio file or playback position.
class WaveFormDisplay : public juce::Component,      // Inherits from juce::Component to create a custom GUI component
    public juce::ChangeListener  // Implements ChangeListener to respond to changes from audioThumbnail
{
public:
    // Constructor: Takes references to AudioFormatManager and AudioThumbnailCache to initialize the audio thumbnail
    WaveFormDisplay(AudioFormatManager& formatManagerToUse,
        AudioThumbnailCache& cacheToUse);

    // Destructor: Cleans up resources
    ~WaveFormDisplay() override;

    // paint: Draws the waveform and other visual elements on the screen
    void paint(juce::Graphics&) override;

    // resized: Adjusts the layout of any subcomponents if the component size changes
    void resized() override;

    // changeListenerCallback: Called when there is a change in the audioThumbnail, such as loading a new audio file
    void changeListenerCallback(ChangeBroadcaster* source) override;

    // loadURL: Loads an audio file from a URL into the audio thumbnail for visualisation
    void loadURL(URL audioURL);

    // setPositionRelative: Updates the display to reflect the current playback position
    void setPositionRelative(double pos);

    // mouseDrag: Handles user dragging on the waveform to change the playback position
    void mouseDrag(const juce::MouseEvent& event);

    // setPlayer: Sets the DJAudioPlayer instance to control playback
    void setPlayer(DJAudioPlayer* playerToUse);

    // Flag to indicate if a file has been successfully loaded into the audio thumbnail
    bool fileLoaded;

private:
    // AudioThumbnail: Generates and stores a visual representation of the audio file
    AudioThumbnail audioThumb;

    // Stores the current playback position relative to the length of the audio file
    double position;

    // Pointer to a DJAudioPlayer instance for controlling playback
    DJAudioPlayer* player;

    // Macro to prevent copying and memory leaks
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveFormDisplay)
};

