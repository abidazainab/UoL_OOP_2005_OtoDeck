

/*
  ==============================================================================

    JogWheel.cpp
    Created: 1 Aug 2024 9:28:14pm
    Author:  Admin

  ==============================================================================
*/

#include <JuceHeader.h>           // JUCE framework header
#include "JogWheel.h"             // Header file for the JogWheel class
#include <cmath>                  // Standard library header for mathematical functions

#ifndef M_PI                      // Check if M_PI is defined; if not, define it
#define M_PI 3.14159265358979323846
#endif

//==============================================================================
// Constructor: Initializes the JogWheel with the provided DJAudioPlayer and background color.
// The circular meter is also initialized with a function to retrieve the current RMS level from the player.
JogWheel::JogWheel(DJAudioPlayer* _player, juce::Colour backgroundColor)
    : player(_player),
    backgroundColor(backgroundColor),
    circularMeter([_player] { return _player->getRMSLevel(); }, juce::Colours::skyblue)
{
    setFramesPerSecond(60);             // Set the animation frame rate to 60 FPS
    addAndMakeVisible(circularMeter);   // Add the circular meter as a visible child component
}

// Destructor: Cleans up any resources used by the JogWheel.
JogWheel::~JogWheel() {}

// paint: Handles the drawing of the JogWheel component.
// This method draws the background, the jog wheel's various circles, and the animated segments.
void JogWheel::paint(juce::Graphics& g)
{
    // Fill the component's background with the specified background color
    g.fillAll(backgroundColor);

    // Calculate the size and center of the jog wheel
    double size = calculateSize();
    double center = size / 2 + margin;
    int radius = size / 2;

    // Draw the outer silver circle with a gradient from dark grey to light grey
    juce::ColourGradient outerCircleGradient(
        juce::Colours::darkgrey, center, center,
        juce::Colours::lightgrey, center + radius, center + radius,
        true
    );
    g.setGradientFill(outerCircleGradient);
    g.fillEllipse(margin, margin, size, size);

    // Draw the middle white circle with an orange gradient border
    double innerSize = size * 0.85;
    juce::ColourGradient middleCircleGradient(
        juce::Colours::orange, center, center,
        juce::Colours::white, center + innerSize / 2, center + innerSize / 2,
        true
    );
    g.setGradientFill(middleCircleGradient);
    g.fillEllipse(margin + (size - innerSize) / 2, margin + (size - innerSize) / 2, innerSize, innerSize);

    // Draw the inner black circle for the time display
    double timeDisplaySize = size * 0.6;
    g.setColour(juce::Colours::black);
    g.fillEllipse(margin + (size - timeDisplaySize) / 2, margin + (size - timeDisplaySize) / 2, timeDisplaySize, timeDisplaySize);

    // Draw the animated segments on the outer circle
    drawAnimatedSegments(g, center, radius);
}

// drawAnimatedSegments: Draws the animated segments on the outer circle of the jog wheel.
// These segments rotate to indicate the playback position.
void JogWheel::drawAnimatedSegments(juce::Graphics& g, double center, int radius)
{
    int numSegments = 24;                                // Number of segments to draw around the jog wheel
    double segmentAngle = 2.0 * M_PI / numSegments;      // Angle between each segment
    double animationProgress = fmod(player->getCurrentPosition() * 0.5, 1.0); // Example animation progress

    for (int i = 0; i < numSegments; ++i)
    {
        double angle = i * segmentAngle + animationProgress * segmentAngle;
        double startX = center + (radius - 10) * std::cos(angle);
        double startY = center + (radius - 10) * std::sin(angle);
        double endX = center + radius * std::cos(angle);
        double endY = center + radius * std::sin(angle);

        // Set the color and thickness for each segment (can animate these as well)
        g.setColour(juce::Colours::white);
        g.drawLine(startX, startY, endX, endY, 4.0f); // Adjust thickness as needed
    }
}

// calculateSize: Determines the size of the jog wheel based on the component's dimensions.
// The size is adjusted to fit within the component while maintaining the defined margin.
double JogWheel::calculateSize()
{
    double width = getWidth();
    double height = getHeight();
    return ((width <= height) ? width : height) - (margin * 2);
}

// resized: Called when the component is resized.
// This method adjusts the bounds of the circular meter and positions it within the jog wheel.
void JogWheel::resized()
{
    double size = calculateSize();
    double center = size / 2 + margin;
    int radius = size / 2;

    // Set the bounds of the circular meter in the center of the jog wheel
    circularMeter.setBounds(center - size * 0.25, center - size * 0.25, size * 0.5, size * 0.5);

    // If the playback position is at the beginning, set the initial point position on the jog wheel
    if (player->getCurrentPosition() == 0)
    {
        point.x = center + radius * std::cos(0);
        point.y = center + radius * std::sin(0);
    }
}

// update: This method is called by the animation system to update the component's state.
// Currently, it does not perform any actions but is required by the AnimatedAppComponent.
void JogWheel::update() {}

// mouseDown: Called when the mouse button is pressed within the component.
// Stops the audio playback when the jog wheel is clicked.
void JogWheel::mouseDown(const juce::MouseEvent& event)
{
    player->stop();
}

// mouseUp: Called when the mouse button is released within the component.
// Starts the audio playback when the jog wheel is released.
void JogWheel::mouseUp(const juce::MouseEvent& event)
{
    player->start();
}

// mouseDrag: Called when the mouse is dragged within the component.
// Allows the user to scrub through the audio track by rotating the jog wheel.
void JogWheel::mouseDrag(const juce::MouseEvent& event)
{
    double size = calculateSize();
    double center = size / 2 + margin;
    int radius = size / 2;

    // Calculate the angle of the mouse position relative to the center of the jog wheel
    double angle = std::atan2(event.y - center, event.x - center);

    // Convert the angle to a playback position in seconds
    double newPosition = angle / (2.0 * M_PI) * player->getTotalLength();

    // If the new position is negative, adjust it to a positive value within the total length
    if (newPosition < 0)
        newPosition += player->getTotalLength();

    // Set the new playback position in the DJAudioPlayer
    player->setPosition(newPosition);
}
