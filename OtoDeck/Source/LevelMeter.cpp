
/*
  ==============================================================================

    LevelMeter.cpp
    Created: 28 Aug 2024 10:35:31pm
    Author:  Admin

  ==============================================================================
*/

#include <JuceHeader.h>           // JUCE framework header
#include "LevelMeter.h"           // Header file for the LevelMeter class

//==============================================================================
// Constructor: Initializes the LevelMeter with the provided value-supplying function.
// It also starts a timer to regularly update the meter at 24Hz.
LevelMeter::LevelMeter(std::function<float()>&& valueFunction)
    : valueSupplier(std::move(valueFunction))  // Move the value supplier function to avoid unnecessary copies
{
    startTimerHz(24);  // Start a timer that calls timerCallback 24 times per second
}

// Destructor: Automatically stops the timer when the LevelMeter is destroyed.
LevelMeter::~LevelMeter()
{
}

// paint: This is where the LevelMeter's visual representation is drawn.
// The height of the filled rectangle represents the current value supplied by valueSupplier.
void LevelMeter::paint(juce::Graphics& g)
{
    // Define the bounds of the component, slightly reduced to give a padding effect
    auto bounds = getLocalBounds().toFloat().reduced(2.0f);

    // Fill the background with black to create a contrast with the meter's colours
    g.setColour(juce::Colours::black);
    g.fillRect(bounds);

    // Set the gradient fill, which transitions from green to red, to indicate the level
    g.setGradientFill(gradient);

    // Map the supplied value to a height within the bounds of the component
    const auto scaledY = juce::jmap(valueSupplier(), 0.0f, 0.1f, 0.0f, static_cast<float>(getHeight()));

    // Fill a rectangle from the bottom of the component up to the scaled height
    g.fillRect(bounds.removeFromBottom(scaledY));
}

// resized: This function is called whenever the LevelMeter is resized. 
// It sets up the gradient for the meter, transitioning from green at the bottom to red at the top.
void LevelMeter::resized()
{
    // Define the bounds of the component
    const auto bounds = getLocalBounds().toFloat();

    // Create a vertical gradient from green (bottom) to red (top), with intermediate colours for yellow and orange
    gradient = juce::ColourGradient{ juce::Colours::green, bounds.getBottomLeft(),  // Start colour and position
                                     juce::Colours::red, bounds.getTopLeft(),       // End colour and position
                                     false };  // Linear gradient

    // Add intermediate colours to the gradient
    gradient.addColour(0.3, juce::Colours::yellow);         // Yellow at 30% height
    gradient.addColour(0.6, juce::Colours::orange);         // Orange at 60% height
    gradient.addColour(0.8, juce::Colours::red.brighter()); // Brighter red at 80% height
}

// timerCallback: This method is called periodically by the Timer.
// It forces a repaint, ensuring the LevelMeter's display is updated in real time.
void LevelMeter::timerCallback()
{
    repaint();  // Repaint the component to reflect any changes in value or appearance
}
