#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "MiddleDeck.h"
#include "PlaylistComponent.h"
#include "CSVtrack.h"
#include "TrackEntry.h"

//==============================================================================
// MainComponent is the primary GUI component for the application.
// It manages the layout of other components like DeckGUI, PlaylistComponent, etc.
// It also handles the audio processing by overriding methods from juce::AudioAppComponent.
class MainComponent : public juce::AudioAppComponent
{
public:
    // Constructor - sets up the main component.
    MainComponent();

    // Destructor - cleans up any resources when the component is destroyed.
    ~MainComponent() override;

    // Prepares the audio callback, typically called before playback starts.
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

    // Called repeatedly to process audio blocks.
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;

    // Releases resources when the audio processing stops.
    void releaseResources() override;

    // Paints the component, handling any custom GUI drawing.
    void paint(juce::Graphics& g) override;

    // Called when the component is resized, updates the layout of child components.
    void resized() override;

private:
    // AudioFormatManager is responsible for managing the available audio formats.
    AudioFormatManager formatManager;

    // AudioThumbnailCache is used to cache waveform thumbnails for faster drawing.
    AudioThumbnailCache thumbCache{ 100 };

    // PlaylistComponent manages the list of tracks and interaction with the DeckGUIs.
    // It references both deckGUI1 and deckGUI2 for controlling audio playback.
    PlaylistComponent playlistComponent{ &deckGUI1, &deckGUI2 };

    // Two DJAudioPlayer instances for managing audio playback logic.
    DJAudioPlayer player1{ formatManager };
    DJAudioPlayer player2{ formatManager };

    // Two DeckGUI instances that provide the GUI for interacting with the DJAudioPlayers.
    DeckGUI deckGUI1{ &player1, formatManager, thumbCache, &playlistComponent, juce::Colours::skyblue };
    DeckGUI deckGUI2{ &player2, formatManager, thumbCache, &playlistComponent, juce::Colours::pink };

    // MiddleDeck is a GUI component that manages crossfading between player1 and player2.
    MiddleDeck middleDeck{ &player1, &player2 };

    // MixerAudioSource is used to mix the audio output from both DJAudioPlayers.
    MixerAudioSource mixerSource;

    // This macro helps to prevent copying and memory leaks in JUCE components.
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
