
/*
  ==============================================================================

    JogWheel.h
    Created: 1 Aug 2024 9:28:14pm
    Author:  Admin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>          // JUCE framework header for GUI components and utilities
#include "DJAudioPlayer.h"       // Custom class for audio playback control
#include "CircularMeter.h"       // Custom component for displaying audio levels

//==============================================================================
// JogWheel is a custom component representing a jog wheel interface for controlling audio playback.
// It allows for scrubbing through audio tracks and provides a visual representation of the current audio state.
class JogWheel : public juce::AnimatedAppComponent // Inherits from AnimatedAppComponent to allow animation updates
{
public:
    // Constructor: Initializes the JogWheel with the given player and background color.
    // @param player Pointer to the DJAudioPlayer instance for controlling playback.
    // @param backgroundColor The background color of the JogWheel component.
    JogWheel(DJAudioPlayer* player, juce::Colour backgroundColor);

    // Destructor: Cleans up any resources used by the JogWheel.
    ~JogWheel() override;

    // paint: Handles the drawing of the JogWheel component.
    // This method is responsible for rendering the jog wheel, including its background, circles, and animated segments.
    void paint(juce::Graphics&) override;

    // resized: Adjusts the layout of the JogWheel and its child components when the component size changes.
    void resized() override;

    // update: Called by the animation system to update the component's state.
    // Currently, this method does not perform any actions but is required by the AnimatedAppComponent.
    void update() override;

    // mouseDown: Called when the mouse button is pressed within the component.
    // Stops the audio playback when the jog wheel is clicked.
    void mouseDown(const juce::MouseEvent& event) override;

    // mouseUp: Called when the mouse button is released within the component.
    // Starts the audio playback when the jog wheel is released.
    void mouseUp(const juce::MouseEvent& event) override;

    // mouseDrag: Called when the mouse is dragged within the component.
    // Allows the user to scrub through the audio track by rotating the jog wheel.
    void mouseDrag(const juce::MouseEvent& event) override;

private:
    DJAudioPlayer* player;            // Pointer to the DJAudioPlayer instance used to control audio playback
    juce::Colour backgroundColor;     // Background color of the JogWheel component
    juce::Point<float> point;         // Stores the current position of the scrubber point
    CircularMeter circularMeter;      // Circular meter component used to display the audio level

    double margin = 10;               // Margin around the jog wheel for aesthetic spacing

    // calculateSize: Determines the size of the jog wheel based on the component's dimensions.
    // The size is adjusted to fit within the component while maintaining the defined margin.
    double calculateSize();

    // drawAnimatedSegments: Draws the animated segments on the outer circle of the jog wheel.
    // @param g Reference to the Graphics object used for drawing.
    // @param center The center position of the jog wheel.
    // @param radius The radius of the jog wheel.
    void drawAnimatedSegments(juce::Graphics& g, double center, int radius);

    // secondsToString: Converts a time value in seconds to a string in "MM:SS" format.
    // @param seconds The time value in seconds.
    // @return A JUCE String representing the time in "MM:SS" format.
    static juce::String secondsToString(double seconds)
    {
        int minutes = static_cast<int>(seconds) / 60;
        int secs = static_cast<int>(seconds) % 60;
        return juce::String::formatted("%02d:%02d", minutes, secs);
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(JogWheel) // Macro to prevent copying and memory leaks
};

