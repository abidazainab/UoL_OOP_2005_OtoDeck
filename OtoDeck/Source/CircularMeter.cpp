
/*
  ==============================================================================

    CircularMeter.cpp
    Created: 29 Aug 2024 1:51:39am
    Author:  Admin

  ==============================================================================
*/

#include <JuceHeader.h>           // JUCE framework header
#include "CircularMeter.h"        // Header file for the CircularMeter class

//==============================================================================
// Constructor: Initializes the CircularMeter with the provided value-supplying function and colour.
// It also starts a timer to regularly update the meter at 24Hz.
CircularMeter::CircularMeter(std::function<float()>&& valueFunction, const juce::Colour& colour)
    : valueSupplier(std::move(valueFunction)), meterColour(colour)
{
    startTimerHz(24);  // Start a timer that calls timerCallback 24 times per second
}

// Destructor: Automatically stops the timer when the CircularMeter is destroyed.
CircularMeter::~CircularMeter()
{
}

// paint: This is where the CircularMeter's visual representation is drawn.
// The size of the ellipse is controlled by the current value supplied by valueSupplier.
// The ellipse is distorted with randomness to create a dynamic, organic look.
void CircularMeter::paint(juce::Graphics& g)
{
    // Get the bounds of the component as a floating-point rectangle
    const auto bounds = getLocalBounds().toFloat();

    // Map the supplied value to a level between 0 and 1
    const auto level = jlimit(0.f, 1.f, jmap(valueSupplier(), 0.0f, 0.1f, 0.f, 1.f));

    // Create a reference path based on an ellipse that will be deformed with randomness
    Path referencePath{};
    referencePath.addEllipse(bounds.reduced(bounds.proportionOfHeight(0.4f * (1.f - level))));

    Path ellipse{};
    const auto numberOfPoints = 50;  // Number of points to use when constructing the ellipse

    // Get the length of the reference ellipse path
    const auto ellipseLength = referencePath.getLength();

    // Start drawing the deformed ellipse by moving to a randomised starting point
    const auto startingPoint = generatePointWithRandomness(referencePath.getPointAlongPath(0.f), level);
    ellipse.startNewSubPath(startingPoint);

    // Generate the points of the ellipse, adding random displacement at each step
    for (auto i = 1; i <= numberOfPoints; i++)
    {
        if (i == numberOfPoints)
            ellipse.lineTo(startingPoint);  // Close the ellipse by connecting back to the starting point
        else
            ellipse.lineTo(generatePointWithRandomness(referencePath.getPointAlongPath(ellipseLength * i / numberOfPoints), level));
    }
    ellipse.closeSubPath();  // Close the path to form a complete shape
    ellipse = ellipse.createPathWithRoundedCorners(1.f);  // Smooth the corners of the ellipse

    // Apply a transformation to the ellipse to slightly shrink it based on the level
    const auto affine = AffineTransform::translation(-4.f * level, -4.f * level);
    ellipse.applyTransform(affine);

    // Create a radial gradient for filling the ellipse, transitioning from a semi-transparent to a fully transparent colour
    const auto gradient = ColourGradient(meterColour.withAlpha(0.6f),
        { getWidth() / 2.f, getHeight() / 2.f },
        meterColour.withAlpha(0.f),
        {}, true);
    g.setGradientFill(gradient);

    // Stroke the ellipse path with a thick line, the thickness depends on the level
    g.strokePath(ellipse, PathStrokeType{ 5.f * level, PathStrokeType::JointStyle::curved, PathStrokeType::rounded });

    // Draw another stroke around the ellipse with a slightly thinner line and a lighter colour
    g.setColour(meterColour.withMultipliedLightness(1.f + level * 0.3f));
    g.strokePath(ellipse, PathStrokeType{ 2.f + 1.f * level, PathStrokeType::JointStyle::curved, PathStrokeType::rounded });
}

// resized: This function is called whenever the CircularMeter is resized. 
// Currently, it doesn't perform any operations but is a placeholder for future layout logic.
void CircularMeter::resized()
{
    // This method is where you would adjust the bounds of any child components.
}

// timerCallback: This method is called periodically by the Timer.
// It forces a repaint, ensuring the CircularMeter's display is updated in real time.
void CircularMeter::timerCallback()
{
    repaint();  // Repaint the component to reflect any changes in value or appearance
}

// generatePointWithRandomness: Adds random displacement to a given point, 
// creating a dynamic, wavy effect on the ellipse to make it appear more organic.
Point<float> CircularMeter::generatePointWithRandomness(Point<float> p, const float level)
{
    // Displace the point's x and y coordinates by a random amount proportional to the level
    p.addXY(random.nextFloat() * level * 8.f, random.nextFloat() * level * 8.f);
    return p;
}
