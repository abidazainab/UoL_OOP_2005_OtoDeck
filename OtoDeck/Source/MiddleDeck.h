
/*
  ==============================================================================

    MiddleDeck.h
    Created: 1 Aug 2024 8:43:59pm
    Author:  Admin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>         // JUCE framework header for GUI components and utilities
#include "DJAudioPlayer.h"      // Custom class for handling audio playback
#include "CustomLookAndFeel.h"  // Custom LookAndFeel for styling the sliders and labels

//==============================================================================
// MiddleDeck is a custom component that acts as the central control deck for a DJ application.
// It allows for controlling two audio players via sliders for volume, speed, and crossfading.
class MiddleDeck : public juce::Component,         // Inherits from JUCE Component to create a custom GUI component
    public juce::Slider::Listener  // Implements Slider::Listener to handle slider value changes
    
{
public:
    // Constructor: Initializes the MiddleDeck with two DJAudioPlayer instances.
    // @param player1 Pointer to the first DJAudioPlayer instance.
    // @param player2 Pointer to the second DJAudioPlayer instance.
    MiddleDeck(DJAudioPlayer* player1, DJAudioPlayer* player2);

    // Destructor: Cleans up resources used by the MiddleDeck.
    ~MiddleDeck() override;

    // paint: Handles the drawing of the MiddleDeck component.
    // This method draws the background, images, and text labels.
    void paint(juce::Graphics&) override;

    // resized: Adjusts the layout of the MiddleDeck's sliders and labels when the component size changes.
    void resized() override;

    // sliderValueChanged: Called when a slider's value changes.
    // Updates the corresponding DJAudioPlayer or the displayed value based on which slider was changed.
    void sliderValueChanged(Slider* slider) override;


private:
    DJAudioPlayer* player1;  // Pointer to the first DJAudioPlayer instance
    DJAudioPlayer* player2;  // Pointer to the second DJAudioPlayer instance

    // Sliders for controlling volume, speed, and crossfading between the two players
    Slider crossfader;
    Slider vol1;
    Slider vol2;
    Slider speed1;
    Slider speed2;

    // Labels for displaying text next to the sliders
    Label vol1Label;
    Label vol2Label;
    Label crossfaderLabel;
    Label speed1Label;
    Label speed2Label;
    Label speed1ValueLabel;  // Label to display the current speed for player 1
    Label speed2ValueLabel;  // Label to display the current speed for player 2

    juce::Image image;  // Image displayed in the MiddleDeck component

    // Unique pointer to a custom LookAndFeel instance for styling the sliders and labels
    std::unique_ptr<CustomLookAndFeel> customLookAndFeel;

    // Macro to prevent copying and memory leaks
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MiddleDeck)
};
