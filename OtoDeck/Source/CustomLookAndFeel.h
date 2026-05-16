
/*
  ==============================================================================

    CustomLookAndFeel.h
    Created: 3 Aug 2024 12:14:58pm
    Author:  Admin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h> // JUCE framework header for GUI components and utilities

//==============================================================================
// CustomLookAndFeel is a class that extends the JUCE LookAndFeel_V4 to customize the appearance of sliders, buttons, and other GUI components.
// It overrides various drawing methods to provide a unique look for the application's interface.
class CustomLookAndFeel : public juce::LookAndFeel_V4 // Inherits from LookAndFeel_V4 to customize the appearance of components
{
public:
    // Constructor: Sets up the custom LookAndFeel with default colours for various components.
    CustomLookAndFeel();

    // Destructor: Cleans up any resources used by the CustomLookAndFeel.
    ~CustomLookAndFeel();

    // drawRotarySlider: Customizes the appearance of rotary sliders.
    // This method is responsible for drawing the rotary knob, background, and pointer.
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&) override;

    // drawLinearSlider: Customizes the appearance of linear sliders.
    // This method is responsible for drawing both horizontal and vertical sliders.
    void drawLinearSlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
        float minSliderPos, float maxSliderPos, const Slider::SliderStyle,
        Slider& slider) override;

    // drawButtonBackground: Customizes the appearance of button backgrounds.
    // This method is responsible for drawing the button's shape, background, and effects like shadows and glows.
    void drawButtonBackground(juce::Graphics& g, juce::Button& button,
        const juce::Colour& backgroundColour,
        bool isMouseOverButton, bool isButtonDown) override;

    // drawButtonText: Customizes the appearance of button text.
    // This method is responsible for drawing the text on buttons, considering hover and click states.
    void drawButtonText(juce::Graphics& g, juce::TextButton& button,
        bool isMouseOverButton, bool isButtonDown) override;
};
