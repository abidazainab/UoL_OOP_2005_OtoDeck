

/*
  ==============================================================================

    LevelMeterBulb.cpp
    Created: 29 Aug 2024 9:07:45am
    Author:  Admin

  ==============================================================================
*/

#include <JuceHeader.h>           // JUCE framework header
#include "LevelMeterBulb.h"       // Header file for the LevelMeterBulb and Bulb classes

//==============================================================================
// Bulb Implementation

// Constructor: Initializes the Bulb with the specified colour.
Bulb::Bulb(const juce::Colour& c) : colour(c) {}

// paint: This is where the Bulb's visual representation is drawn.
// If the bulb is on, it is drawn in its specified colour with a glowing effect. Otherwise, it is drawn in black.
void Bulb::paint(juce::Graphics& g)
{
    // Define the bounds of the bulb as a square within the component's bounds
    auto bounds = getLocalBounds().toFloat().reduced(2.0f);
    auto diameter = std::min(bounds.getWidth(), bounds.getHeight());
    juce::Rectangle<float> bulbBounds(bounds.getX(), bounds.getY(), diameter, diameter);

    if (isOn)
    {
        // Draw the bulb as a filled ellipse in the specified colour
        g.setColour(colour);
        g.fillEllipse(bulbBounds);

        // Add a glowing effect around the bulb using a radial gradient
        g.setGradientFill(juce::ColourGradient(colour.withAlpha(0.3f), bulbBounds.getCentre(),
            colour.withLightness(1.5f).withAlpha(0.f), {}, true));
        g.fillEllipse(bulbBounds.expanded(4.0f));  // Expand the glow slightly beyond the bulb's bounds
    }
    else
    {
        // If the bulb is off, draw it as a black ellipse
        g.setColour(juce::Colours::black);
        g.fillEllipse(bulbBounds);
    }

    // Draw an outline around the bulb, regardless of its state
    g.setColour(juce::Colours::black);
    g.drawEllipse(bulbBounds, 1.3f);
}

// setState: Updates the state of the bulb (on or off) and triggers a repaint if the state changes.
void Bulb::setState(bool state)
{
    if (isOn != state)
    {
        isOn = state;
        repaint();  // Repaint the bulb to reflect the new state
    }
}

//==============================================================================
// LevelMeterBulb Implementation

// Constructor: Initializes the LevelMeterBulb with the provided value-supplying function.
// It also starts a timer to regularly update the meter at 24Hz.
LevelMeterBulb::LevelMeterBulb(std::function<float()>&& valueFunction)
    : valueSupplier(std::move(valueFunction))  // Move the value supplier function to avoid unnecessary copies
{
    startTimerHz(24);  // Start a timer that calls timerCallback 24 times per second
}

// Destructor: Automatically stops the timer when the LevelMeterBulb is destroyed.
LevelMeterBulb::~LevelMeterBulb()
{
}

// paint: This is where the LevelMeterBulb's visual representation is drawn.
// The function checks the current value and determines which bulbs should be lit.
void LevelMeterBulb::paint(juce::Graphics& g)
{
    // Map the supplied value to a range between 0 and 1
    auto value = juce::jmap(valueSupplier(), 0.0f, 0.1f, 0.0f, 1.0f);

    // Determine which bulbs should be lit based on the current value
    for (int i = 0; i < totalNumberOfBulbs; ++i)
    {
        bool shouldBeOn = value >= static_cast<float>(i + 1) / totalNumberOfBulbs;
        bulbs[i]->setState(shouldBeOn);  // Update each bulb's state
    }
}

// resized: This function is called whenever the LevelMeterBulb is resized.
// It creates and positions the bulbs based on the component's new size.
void LevelMeterBulb::resized()
{
    // Define the bounds of the component
    auto bounds = getLocalBounds().toFloat();

    // Create a vertical gradient for the bulbs, transitioning from green at the bottom to red at the top
    juce::ColourGradient gradient(juce::Colours::green, bounds.getBottomLeft(),
        juce::Colours::red, bounds.getTopLeft(), false);
    gradient.addColour(0.5, juce::Colours::yellow);  // Add yellow at the middle of the gradient

    // Calculate the height for each bulb based on the total number of bulbs
    auto bulbHeight = getHeight() / totalNumberOfBulbs;
    auto bulbBounds = getLocalBounds();
    bulbs.clear();  // Clear any existing bulbs

    // Create and position each bulb
    for (int i = 0; i < totalNumberOfBulbs; ++i)
    {
        // Create a new Bulb with a colour from the gradient, based on its position in the stack
        auto bulb = std::make_unique<Bulb>(gradient.getColourAtPosition(static_cast<double>(i) / totalNumberOfBulbs));
        addAndMakeVisible(bulb.get());  // Add the bulb to the component
        bulb->setBounds(bulbBounds.removeFromBottom(bulbHeight));  // Position the bulb
        bulbs.push_back(std::move(bulb));  // Store the bulb in the vector
    }
}

// timerCallback: This method is called periodically by the Timer.
// It forces a repaint, ensuring the LevelMeterBulb's display is updated in real time.
void LevelMeterBulb::timerCallback()
{
    repaint();  // Repaint the component to reflect any changes in value or appearance
}
