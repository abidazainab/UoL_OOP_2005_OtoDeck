
/*
  ==============================================================================

    LevelMeter.h
    Created: 28 Aug 2024 10:35:31pm
    Author:  Admin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h> // JUCE framework header for GUI components and utilities

//==============================================================================
// LevelMeter is a custom JUCE Component that visually represents an audio level as a vertical bar.
// It periodically updates and redraws itself using a Timer.
class LevelMeter : public juce::Component,    // Inherits from juce::Component to create a GUI component
    public juce::Timer         // Inherits from juce::Timer to update the component at regular intervals
{
public:
    // Constructor: Initializes the LevelMeter with a function to supply its value.
    LevelMeter(std::function<float()>&& valueFunction);

    // Destructor: Ensures proper cleanup when the LevelMeter is destroyed.
    ~LevelMeter() override;

    // paint: Handles the drawing of the LevelMeter. This is where the meter's visual representation is created.
    void paint(juce::Graphics& g) override;

    // resized: Adjusts the component's layout when its size changes. 
    // Used here to set up the gradient for the meter.
    void resized() override;

    // timerCallback: Called periodically by the Timer to update and repaint the component.
    void timerCallback() override;

private:
    // valueSupplier: A function that provides the current value to be displayed by the meter.
    // This value controls the meter's level.
    std::function<float()> valueSupplier;

    // gradient: A ColourGradient object used to fill the meter with a smooth transition of colours from green to red.
    juce::ColourGradient gradient{};

    // Macro to prevent copying and memory leaks
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelMeter)
};
