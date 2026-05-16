
/*
  ==============================================================================

    DeckGUI.h
    Created: 28 Jul 2024 9:36:40pm
    Author:  Admin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveFormDisplay.h"
#include "CustomLookAndFeel.h"
#include "JogWheel.h"
#include "TrackEntry.h"
#include "LevelMeter.h"
#include "LevelMeterBulb.h"

//==============================================================================
// DeckGUI is a custom JUCE component class that provides a graphical interface for controlling audio playback and manipulation.
// This class integrates various controls like buttons, sliders, and visual components such as waveform display and level meters.
class PlaylistComponent;  // Forward declaration of PlaylistComponent

class DeckGUI : public juce::Component,
    public juce::Button::Listener,
    public Slider::Listener,
    public juce::FileDragAndDropTarget,
    public juce::Timer
{
public:
    // Constructor: Initializes the DeckGUI with a DJAudioPlayer and other dependencies.
    // @param player Pointer to the DJAudioPlayer instance for controlling audio playback.
    // @param formatManagerToUse Reference to an AudioFormatManager for managing audio formats.
    // @param cacheToUse Reference to an AudioThumbnailCache for caching waveform data.
    // @param _playlistComponent Pointer to the associated PlaylistComponent for managing track entries.
    // @param backgroundColor Optional parameter to set the background color of the DeckGUI.
    DeckGUI(DJAudioPlayer* player,
        AudioFormatManager& formatManagerToUse,
        AudioThumbnailCache& cacheToUse,
        PlaylistComponent* _playlistComponent,
        juce::Colour backgroundColor = juce::Colours::skyblue);

    // Destructor: Cleans up resources used by the DeckGUI.
    ~DeckGUI() override;

    // paint: Renders the GUI elements of the DeckGUI.
    // @param g The Graphics context used for drawing.
    void paint(juce::Graphics&) override;

    // resized: Lays out the child components within the DeckGUI when resized.
    void resized() override;

    // buttonClicked: Handles button click events.
    // @param button Pointer to the clicked Button.
    void buttonClicked(Button*) override;

    // sliderValueChanged: Handles slider value change events.
    // @param slider Pointer to the Slider whose value changed.
    void sliderValueChanged(Slider*) override;

    // isInterestedInFileDrag: Determines if the DeckGUI is interested in the files being dragged.
    // @param files List of files being dragged.
    // @return True if interested, otherwise false.
    bool isInterestedInFileDrag(const StringArray& files) override;

    // filesDropped: Handles files being dropped onto the DeckGUI.
    // @param files List of dropped files.
    // @param x The x-coordinate of the drop.
    // @param y The y-coordinate of the drop.
    void filesDropped(const StringArray& files, int x, int y) override;

    // timerCallback: Periodically updates the GUI elements, such as waveform position and time display.
    void timerCallback() override;

    // clearDeck: Resets the DeckGUI, clearing the track information and resetting controls to their default states.
    void clearDeck();

    LevelMeter levelMeter;  // Displays the audio level using a meter
    LevelMeterBulb levelMeterBulb;  // Displays the audio level using a series of bulbs
    WaveFormDisplay waveFormDisplay;  // Displays the waveform of the currently loaded track
    DJAudioPlayer* player;  // Pointer to the DJAudioPlayer instance for controlling audio playback
    JogWheel jogWheel;  // Custom jog wheel for controlling playback position
    CustomLookAndFeel customLookAndFeel;  // Custom look and feel for the UI components

    TextButton loadButton{ "LOAD" };  // Button for loading a track
    TextButton playButton{ "Play" };  // Button for playing or stopping the track
    TextButton loopButton{ "Loop" };  // Button for enabling or disabling loop mode
    TextButton frevButton{ "Rewind" };  // Button for rewinding the track
    TextButton ffwdButton{ "Forward" };  // Button for fast-forwarding the track
    TextButton prevButton{ "Prev" };  // Button for loading the previous track

    Label trackName;  // Displays the name of the currently loaded track
    Label timeDisplay;  // Displays the current playback time and total duration
    bool deckCleared = false;  // Flag indicating whether the deck has been cleared

private:

    juce::FileChooser fChooser{ "Select a file..." };  // File chooser for loading tracks

    PlaylistComponent* playlistComponent;  // Pointer to the associated PlaylistComponent
    std::vector<TrackEntry>* trackEntries;  // Pointer to the list of track entries
    juce::Colour backgroundColour;  // Background color of the DeckGUI
    juce::String secondsToString(double seconds);  // Converts seconds to a formatted string "mm:ss"
    void togglePlay();  // Toggles between play and stop states

    Slider bassSlider;  // Slider for controlling the bass EQ
    Slider midSlider;  // Slider for controlling the mid EQ
    Slider trebSlider;  // Slider for controlling the treble EQ

    Label bassLabel;  // Label for the bass slider
    Label midLabel;  // Label for the mid slider
    Label trebLabel;  // Label for the treble slider

    bool shuffle;  // Flag for enabling shuffle mode (not implemented in the provided code)
    bool isLooping = false;  // Flag for indicating whether loop mode is active

    TextButton setCueButton1;  // Button for setting the first cue point
    TextButton playCueButton1;  // Button for playing from the first cue point
    TextButton clearCueButton1;  // Button for clearing the first cue point
    TextButton setCueButton2;  // Button for setting the second cue point
    TextButton playCueButton2;  // Button for playing from the second cue point
    TextButton clearCueButton2;  // Button for clearing the second cue point

    double cueStartPosition1 = -1.0;  // Position of the first cue point, initialized to -1 (indicating no cue set)
    double cueStartPosition2 = -1.0;  // Position of the second cue point, initialized to -1 (indicating no cue set)

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeckGUI)  // Prevents copying and memory leaks
};
