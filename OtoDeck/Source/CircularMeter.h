
/*
  ==============================================================================

    CircularMeter.h
    Created: 29 Aug 2024 1:51:39am
    Author:  Admin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h> // JUCE framework header for GUI and utility classes

//==============================================================================
// CircularMeter is a custom JUCE Component that visually represents a value in a circular format.
// It periodically updates and redraws itself using a Timer.
class CircularMeter : public juce::Component,    // Inherits from juce::Component to create a GUI component
    public juce::Timer         // Inherits from juce::Timer to update the component at a regular interval
{
public:
    // Constructor: Initializes the CircularMeter with a function to supply its value and a colour for rendering
    CircularMeter(std::function<float()>&& valueFunction, const juce::Colour& colour);

    // Destructor: Ensures proper cleanup when the CircularMeter is destroyed
    ~CircularMeter() override;

    // paint: Handles the drawing of the CircularMeter. This is where the meter's visual representation is created.
    void paint(juce::Graphics& g) override;

    // resized: Adjusts the component's layout when its size changes. Currently, this method does not perform any operations.
    void resized() override;

    // timerCallback: Called periodically by the Timer to update and repaint the component
    void timerCallback() override;

private:
    // generatePointWithRandomness: Generates a point with some random displacement, used to create the irregular shape of the meter
    Point<float> generatePointWithRandomness(Point<float> p, const float level);

    // valueSupplier: A function that provides the current value to be displayed by the meter. This value controls the meter's display level.
    std::function<float()> valueSupplier;

    // meterColour: The base colour used for rendering the CircularMeter
    juce::Colour meterColour;

    // random: A Random object, seeded with the current time, used to introduce variability in the meter's shape
    Random random{ Time::currentTimeMillis() };

    // Macro to prevent copying and memory leaks
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CircularMeter)
};
