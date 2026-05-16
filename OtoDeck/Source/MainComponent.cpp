
#include "MainComponent.h"

//==============================================================================
// Constructor - initializes the main component.
MainComponent::MainComponent()
{
    // Set the size of the component.
    setSize(800, 600);

    // Request permission to access the microphone if required.
    if (juce::RuntimePermissions::isRequired(juce::RuntimePermissions::recordAudio)
        && !juce::RuntimePermissions::isGranted(juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request(juce::RuntimePermissions::recordAudio,
            [&](bool granted) { setAudioChannels(granted ? 2 : 0, 2); });
    }
    else
    {
        // Open audio channels (0 input channels, 2 output channels).
        setAudioChannels(0, 2);
    }

    // Add the DeckGUI components to the main component.
    addAndMakeVisible(deckGUI1);
    addAndMakeVisible(deckGUI2);
    addAndMakeVisible(middleDeck);

    // Add the PlaylistComponent to the main component.
    addAndMakeVisible(playlistComponent);

    // Register basic audio formats (e.g., WAV, MP3) with the AudioFormatManager.
    formatManager.registerBasicFormats();
}

// Destructor - shuts down audio and cleans up resources.
MainComponent::~MainComponent()
{
    shutdownAudio(); // Stops the audio processing and releases resources.
}

//==============================================================================
// Called before playback starts to prepare the audio callback.
void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    // Prepare the mixer source and add both players as input sources.
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);
}

// Called repeatedly to process audio blocks.
void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill); // Mix the audio from both players into the output buffer.
}

// Called when playback stops to release audio resources.
void MainComponent::releaseResources()
{
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();
}

//==============================================================================
// Paints the component (e.g., background, text).
void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId)); // Fill background.
    g.setFont(20.0f); // Set font size for any custom text drawing.
}

// Updates the layout of child components when the main component is resized.
void MainComponent::resized()
{
    int unit = getWidth() / 5;

    // Set the bounds of DeckGUI1, DeckGUI2, and MiddleDeck.
    deckGUI1.setBounds(0, 0, unit * 2, getHeight() * 3 / 4);
    deckGUI2.setBounds(unit * 3, 0, unit * 2, getHeight() * 3 / 4);
    middleDeck.setBounds(unit * 2, 0, unit, getHeight() * 3 / 4);

    // Set the bounds of the PlaylistComponent.
    playlistComponent.setBounds(0, getHeight() * 3 / 4, getWidth(), getHeight() * 1 / 4);
}
