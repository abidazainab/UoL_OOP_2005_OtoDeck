
/*
  ==============================================================================

    WaveFormDisplay.cpp
    Created: 29 Jul 2024 11:44:28am
    Author:  Admin

  ==============================================================================
*/

#include <JuceHeader.h>           // JUCE framework header for implementing audio and GUI functionalities
#include "WaveFormDisplay.h"      // Header file for the WaveFormDisplay class

//==============================================================================
// Constructor: Initializes the audio thumbnail and registers the ChangeListener for the thumbnail.
// The parameters are references to the AudioFormatManager and AudioThumbnailCache to handle audio formats and caching.
WaveFormDisplay::WaveFormDisplay(AudioFormatManager& formatManagerToUse,
    AudioThumbnailCache& cacheToUse)
    : audioThumb(1000, formatManagerToUse, cacheToUse),  // Initialize audio thumbnail with a resolution of 1000 samples
    fileLoaded(false),                                 // Set the fileLoaded flag to false initially
    position(0),                                       // Initialize playback position to 0
    player(nullptr)                                    // Initialize player pointer to null
{
    audioThumb.addChangeListener(this); // Register this class as a listener to changes in the audio thumbnail
}

// Destructor: No specific cleanup is required, as smart pointers handle the deletion of the player and audioThumbnail.
WaveFormDisplay::~WaveFormDisplay()
{
}

// setPlayer: Sets the DJAudioPlayer instance that will be used to control audio playback.
void WaveFormDisplay::setPlayer(DJAudioPlayer* playerToUse)
{
    player = playerToUse;  // Assign the provided player instance to the member variable
}

// paint: Draws the waveform, a red playhead line, or a placeholder message if no file is loaded.
void WaveFormDisplay::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);   // Fill the background with black colour

    g.setColour(juce::Colours::white); // Set drawing colour to white
    g.drawRect(getLocalBounds(), 1);   // Draw a white border around the component

    if (fileLoaded)
    {
        // Draw the audio waveform if a file is loaded
        audioThumb.drawChannel(g,
            getLocalBounds(),   // The area where the waveform should be drawn
            0,                  // Start time of the waveform to draw
            audioThumb.getTotalLength(), // End time of the waveform
            0,                  // Channel index (0 = left channel for stereo)
            1.0f);              // Vertical zoom level

        // Draw a red line representing the current playback position
        g.setColour(juce::Colours::red);
        g.drawRect(position * getWidth(), 0, 2, getHeight());
    }
    else
    {
        // Display a message if no file is loaded
        g.setFont(juce::Font(20.0f));
        g.drawText("File not loaded...", getLocalBounds(),
            juce::Justification::centred, true); // Centre the text within the component
    }
}

// resized: Called when the component is resized. Currently, this method is empty because there are no child components.
void WaveFormDisplay::resized()
{
    // Normally, this is where you would set the bounds of any child components.
}

// loadURL: Asynchronously loads an audio file from a URL into the audio thumbnail.
void WaveFormDisplay::loadURL(URL audioURL)
{
    // Ensures that the following operations run on the GUI thread
    juce::MessageManager::callAsync([this, audioURL]()
        {
            audioThumb.clear();  // Clear the current audio thumbnail
            fileLoaded = audioThumb.setSource(new URLInputSource(audioURL)); // Load the new audio file from the URL

            if (fileLoaded) // If the file is successfully loaded
            {
                std::cout << "WFD:: Loaded!" << std::endl;  // Print a success message to the console
                repaint();                                  // Trigger a repaint to display the waveform
            }
            else
            {
                std::cout << "WFD:: Not loaded!" << std::endl; // Print an error message to the console
            }
        });
}

// changeListenerCallback: Called when the audio thumbnail changes, typically when a new file is loaded. Triggers a repaint.
void WaveFormDisplay::changeListenerCallback(ChangeBroadcaster* source)
{
    std::cout << "Change listener callback triggered." << std::endl; // Log that the change listener was triggered
    repaint(); // Repaint the component to reflect changes in the waveform
}

// setPositionRelative: Updates the position of the playback head in the waveform display.
void WaveFormDisplay::setPositionRelative(double pos)
{
    if (pos != position)  // If the new position is different from the current one
    {
        position = pos;   // Update the position
        repaint();        // Repaint the component to move the red playback line
    }
}

// mouseDrag: Handles dragging on the waveform display to seek within the audio file.
void WaveFormDisplay::mouseDrag(const juce::MouseEvent& event)
{
    if (fileLoaded && player != nullptr)  // If a file is loaded and a player is set
    {
        // Calculate the new position based on the drag event's X position relative to the component's width
        double newPosition = event.position.x / getWidth();

        // Clamp the new position to ensure it's within the valid range [0, 1]
        newPosition = juce::jlimit(0.0, 1.0, newPosition);

        // Update the player's playback position
        player->setPositionRelative(newPosition);

        // Update the visual playback position
        setPositionRelative(newPosition);
    }
}

