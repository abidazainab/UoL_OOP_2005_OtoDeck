
/*
  ==============================================================================

    CustomLookAndFeel.cpp
    Created: 3 Aug 2024 12:14:58pm
    Author:  Admin

  ==============================================================================
*/

#include "CustomLookAndFeel.h" // Include the header file for the CustomLookAndFeel class

//==============================================================================
// Constructor: Sets up the CustomLookAndFeel by initializing default colors for various components.
// It defines the thumb color for sliders and the text and background colors for buttons.
CustomLookAndFeel::CustomLookAndFeel()
{
    // Set default colours for components
    setColour(juce::Slider::thumbColourId, juce::Colours::red);
    setColour(juce::TextButton::buttonColourId, juce::Colours::black);
    setColour(juce::TextButton::textColourOffId, juce::Colours::black);
    setColour(juce::TextButton::textColourOnId, juce::Colours::red);
}

// Destructor: Resets the LookAndFeel to the system default if this instance is currently set as the default LookAndFeel.
CustomLookAndFeel::~CustomLookAndFeel()
{
    // Reset to the default system LookAndFeel if this is currently set as default
    if (&juce::LookAndFeel::getDefaultLookAndFeel() == this)
    {
        juce::LookAndFeel::setDefaultLookAndFeel(nullptr);
    }
}

// drawRotarySlider: Customizes the appearance of rotary sliders.
// Draws a rotary knob with a metal effect, including a shadow, gradient fill, and a pointer.
void CustomLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
    const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&)
{
    // Calculate dimensions and center point for the rotary slider
    double radius = (float)juce::jmin(width / 2, height / 2) - 4.0f;
    double centreX = (float)x + (float)width * 0.5f;
    double centreY = (float)y + (float)height * 0.5f;
    double rx = centreX - radius;
    double ry = centreY - radius;
    double rw = radius * 2.0f;
    double angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    // Draw background shadow for depth effect
    g.setColour(juce::Colours::black.withAlpha(0.3f));
    g.fillEllipse(rx + 6, ry + 6, rw, rw); // Offset for shadow effect

    // Create a gradient for the main knob with a metal effect
    juce::ColourGradient gradient(juce::Colours::orange, centreX, centreY - radius,
        juce::Colours::silver, centreX, centreY + radius,
        false); // Linear gradient
    gradient.addColour(0.5, juce::Colours::lightgoldenrodyellow);
    g.setGradientFill(gradient);
    g.fillEllipse(rx, ry, rw, rw);

    // Add inner highlight for the knob
    g.setColour(juce::Colours::white.withAlpha(0.3f));
    g.drawEllipse(rx + 1, ry + 1, rw - 2, rw - 2, 2.0f);

    // Draw the pointer for the knob
    juce::Path p;
    double pointerLength = radius;
    double pointerThickness = 5.0f;
    p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
    p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));

    g.setColour(juce::Colours::black);
    g.fillPath(p);

    // Draw the center circle with a gradient effect
    juce::ColourGradient centerGradient(juce::Colours::black, centreX, centreY,
        juce::Colours::grey, centreX, centreY + 5,
        false); // Linear gradient
    g.setGradientFill(centerGradient);
    double smallw = rw * 0.3;
    g.fillEllipse(centreX - (smallw / 2), centreY - (smallw / 2), smallw, smallw);

    // Add a shadow around the center circle for extra depth
    g.setColour(juce::Colours::black.withAlpha(0.5f));
    g.drawEllipse(centreX - (smallw / 2), centreY - (smallw / 2), smallw, smallw, 1.0f);
}

// drawLinearSlider: Customizes the appearance of linear sliders.
// Handles both horizontal and vertical sliders, adding gradient fills, scaling lines, and custom thumbs.
void CustomLookAndFeel::drawLinearSlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
    float minSliderPos, float maxSliderPos, const Slider::SliderStyle style,
    Slider& slider)
{
    if (style == Slider::LinearHorizontal)
    {
        // Draw the background for horizontal slider (crossfader)
        g.setColour(juce::Colours::purple);
        g.fillRect(static_cast<float>(x), static_cast<float>(y) + 8, static_cast<float>(width), static_cast<float>(height) - 16);

        // Draw scaling lines across the slider (horizontal lines)
        g.setColour(juce::Colours::yellow);
        for (int i = 0; i <= 10; ++i)
        {
            float lineX = static_cast<float>(x + width * i / 10.0f);
            g.drawLine(lineX, static_cast<float>(y + 2), lineX, static_cast<float>(y + height - 2), 1.0f);
        }

        // Draw the thumb (handle) for the horizontal slider
        g.setColour(juce::Colours::green);
        g.fillRect(sliderPos - 10.0f, static_cast<float>(y), 15.0f, static_cast<float>(height));
    }
    else
    {
        // For vertical sliders, draw a gradient-filled background
        juce::ColourGradient gradient(juce::Colours::white, static_cast<float>(x), static_cast<float>(y),
            juce::Colours::orange, static_cast<float>(x), static_cast<float>(y + height),
            false); // Linear gradient
        g.setGradientFill(gradient);
        g.fillRect(static_cast<float>(x) + 10, static_cast<float>(y), static_cast<float>(width) - 20, static_cast<float>(height) + 20);

        // Draw scaling lines along the vertical slider
        g.setColour(juce::Colours::darkgrey);
        for (int i = 0; i <= 12; ++i)
        {
            float lineY = static_cast<float>(y + height * i / 10.0f);
            g.drawLine(static_cast<float>(x + (width / 2) - 15), lineY,
                static_cast<float>(x + width * 2 / 3), lineY, 1.0f);
        }

        // Draw the slider line in the center (dark grey, always visible)
        float centerX = static_cast<float>(x + width / 2);
        g.setColour(juce::Colours::darkgrey);
        g.fillRect(centerX - 2.0f, static_cast<float>(y), 4.0f, static_cast<float>(height) + 20);

        // Draw the thumb (white rectangle) for the vertical slider
        g.setColour(juce::Colours::grey);
        g.fillRect(centerX - static_cast<float>(width / 4), sliderPos, static_cast<float>(width / 2), 15.0f);
    }
}

// drawButtonBackground: Customizes the appearance of button backgrounds.
// Draws a hexagon shape for the button, with shadow and glowing effects based on mouse interaction.
void CustomLookAndFeel::drawButtonBackground(juce::Graphics& g, juce::Button& button,
    const juce::Colour& backgroundColour, bool isMouseOverButton, bool isButtonDown)
{
    juce::Rectangle<int> buttonArea(button.getLocalBounds()); // Get the bounds of the button area

    // Create a hexagon path for the button
    juce::Path hexagon;
    float buttonWidth = static_cast<float>(buttonArea.getWidth());
    float buttonHeight = static_cast<float>(buttonArea.getHeight());
    float sideLength = buttonWidth / 3.0f;

    // Define hexagon points (6 points for the hexagon)
    hexagon.startNewSubPath(buttonArea.getCentreX(), buttonArea.getY()); // Top point
    hexagon.lineTo(buttonArea.getRight() - sideLength / 2, buttonArea.getY() + buttonHeight / 4);  // Top right
    hexagon.lineTo(buttonArea.getRight() - sideLength / 2, buttonArea.getBottom() - buttonHeight / 4); // Bottom right
    hexagon.lineTo(buttonArea.getCentreX(), buttonArea.getBottom()); // Bottom point
    hexagon.lineTo(buttonArea.getX() + sideLength / 2, buttonArea.getBottom() - buttonHeight / 4); // Bottom left
    hexagon.lineTo(buttonArea.getX() + sideLength / 2, buttonArea.getY() + buttonHeight / 4); // Top left
    hexagon.closeSubPath(); // Complete the hexagon

    // Draw shadow for the hexagon
    juce::Colour shadowColour = juce::Colours::black.withAlpha(0.5f);
    juce::DropShadow shadowEffect(shadowColour, 8, juce::Point<int>(4, 4)); // Shadow with an offset
    shadowEffect.drawForPath(g, hexagon);

    // Draw glowing effect if the mouse is over the button
    if (isMouseOverButton) {
        juce::Colour glowColour = juce::Colours::magenta.withAlpha(0.3f);
        juce::DropShadow glowShadow(glowColour, 15, juce::Point<int>(0, 0)); // Centered glow effect
        glowShadow.drawForPath(g, hexagon);
    }

    // Draw the hexagon background with a gradient fill
    juce::ColourGradient gradient(juce::Colours::white, buttonArea.getTopLeft().toFloat(),
        juce::Colours::orange, buttonArea.getBottomRight().toFloat(), false); // Linear gradient
    g.setGradientFill(gradient);
    g.fillPath(hexagon);

    // Draw the hexagon border
    g.setColour(juce::Colours::orange.brighter());
    g.strokePath(hexagon, juce::PathStrokeType(2.0f));

    // Add a light transparent overlay when the mouse is over the button
    if (isMouseOverButton) {
        g.setColour(juce::Colours::white.withAlpha(0.1f));
        g.fillPath(hexagon);
    }
}

// drawButtonText: Customizes the appearance of button text.
// Draws the text within the button, taking into account the hover and click states.
void CustomLookAndFeel::drawButtonText(juce::Graphics& g, juce::TextButton& button,
    bool isMouseOverButton, bool isButtonDown)
{
    juce::Rectangle<int> textArea = button.getLocalBounds().reduced(10); // Add padding to keep text inside the hexagon
    juce::Colour textColour = button.findColour(button.getToggleState() ?
        juce::TextButton::textColourOnId :
        juce::TextButton::textColourOffId);

    g.setColour(textColour);

    // Set font for the button text
    juce::Font font(15.0f, juce::Font::bold);
    g.setFont(font);

    // Draw the text centered within the reduced bounds
    g.drawFittedText(button.getButtonText(), textArea, juce::Justification::centred, 1);
}

