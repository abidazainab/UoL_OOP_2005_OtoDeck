
/*
  ==============================================================================

    LevelMeterBulb.h
    Created: 29 Aug 2024 9:07:45am
    Author:  Admin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h> // JUCE framework header for GUI components and utilities

//==============================================================================
// Bulb is a custom JUCE Component representing an individual light bulb in the level meter.
// It can be turned on or off based on the current level of the meter.
class Bulb : public juce::Component
{
public:
    // Constructor: Initializes the Bulb with a specific colour.
    Bulb(const juce::Colour& c);

    // paint: Handles the drawing of the Bulb. The bulb will be drawn as an ellipse.
    void paint(juce::Graphics& g) override;

    // setState: Sets the state of the Bulb (on or off) and repaints it if the state changes.
    void setState(bool state);

private:
    // isOn: Indicates whether the bulb is currently on or off.
    bool isOn = false;

    // colour: The colour of the bulb when it is on.
    juce::Colour colour;

    // Macro to prevent copying and memory leaks
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Bulb)
};

//==============================================================================
// LevelMeterBulb is a custom JUCE Component that displays an array of Bulbs to represent the audio level.
// The bulbs light up progressively as the audio level increases.
class LevelMeterBulb : public juce::Component,     // Inherits from juce::Component to create a GUI component
    public juce::Timer          // Inherits from juce::Timer to update the component at regular intervals
{
public:
    // Constructor: Initializes the LevelMeterBulb with a function to supply its value.
    LevelMeterBulb(std::function<float()>&& valueFunction);

    // Destructor: Ensures proper cleanup when the LevelMeterBulb is destroyed.
    ~LevelMeterBulb() override;

    // paint: Handles the drawing of the LevelMeterBulb. This controls which bulbs are lit.
    void paint(juce::Graphics& g) override;

    // resized: Adjusts the component's layout when its size changes.
    // Used here to create and position the Bulbs.
    void resized() override;

    // timerCallback: Called periodically by the Timer to update and repaint the component.
    void timerCallback() override;

private:
    // valueSupplier: A function that provides the current value to be displayed by the meter.
    // This value controls how many Bulbs are lit.
    std::function<float()> valueSupplier;

    // bulbs: A vector of unique pointers to Bulb objects, representing the individual lights in the meter.
    std::vector<std::unique_ptr<Bulb>> bulbs;

    // totalNumberOfBulbs: The total number of Bulbs in the LevelMeterBulb component.
    const int totalNumberOfBulbs = 10;

    // Macro to prevent copying and memory leaks
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelMeterBulb)
};
