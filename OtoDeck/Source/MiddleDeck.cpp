
/*
  ==============================================================================

    MiddleDeck.cpp
    Created: 1 Aug 2024 8:43:59pm
    Author:  Admin

  ==============================================================================
*/

#include <JuceHeader.h>        // JUCE framework header
#include "MiddleDeck.h"        // Include the header file for the MiddleDeck class

//==============================================================================
// Constructor: Initializes the MiddleDeck component with two DJAudioPlayer instances.
// Sets up the sliders, labels, and the custom LookAndFeel for the component.
MiddleDeck::MiddleDeck(DJAudioPlayer* _player1, DJAudioPlayer* _player2)
    : player1(_player1),
    player2(_player2)
{
    // Initialize the custom LookAndFeel and apply it to the sliders
    customLookAndFeel = std::make_unique<CustomLookAndFeel>();

    vol1.setLookAndFeel(customLookAndFeel.get());
    vol2.setLookAndFeel(customLookAndFeel.get());
    speed1.setLookAndFeel(customLookAndFeel.get());
    speed2.setLookAndFeel(customLookAndFeel.get());
    crossfader.setLookAndFeel(customLookAndFeel.get());

    // Set up slider ranges and initial values
    vol1.setRange(0.1, 1.0);
    vol1.setValue(1.0);
    vol2.setRange(0.1, 1.0);
    vol2.setValue(1.0);
    crossfader.setRange(0.1, 1.0);
    crossfader.setValue(0.5);
    speed1.setRange(0.1, 10.0);
    speed1.setValue(0.5);
    speed2.setRange(0.1, 10.0);
    speed2.setValue(0.5);

    // Set slider styles (no text box, rotary for volume, linear vertical for speed)
    crossfader.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    vol1.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    vol2.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    speed1.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    speed2.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);

    vol1.setSliderStyle(Slider::Rotary);
    vol2.setSliderStyle(Slider::Rotary);
    speed1.setSliderStyle(Slider::LinearVertical);
    speed2.setSliderStyle(Slider::LinearVertical);

    // Make sliders visible
    addAndMakeVisible(vol1);
    addAndMakeVisible(vol2);
    addAndMakeVisible(speed1);
    addAndMakeVisible(speed2);
    addAndMakeVisible(crossfader);

    // Initialize and style labels
    vol1Label.setText("Vol1", dontSendNotification);
    vol2Label.setText("Vol2", dontSendNotification);
    speed1Label.setText("Speed-1", dontSendNotification);
    speed2Label.setText("Speed-2", dontSendNotification);
    crossfaderLabel.setText("Crossfader", dontSendNotification);

    //positioning center
    vol1Label.setJustificationType(juce::Justification::centred);
    vol2Label.setJustificationType(juce::Justification::centred);
    speed1Label.setJustificationType(juce::Justification::centred);
    speed2Label.setJustificationType(juce::Justification::centred);
    crossfaderLabel.setJustificationType(juce::Justification::centred);

    //Set font style
    vol1Label.setFont(juce::Font(16.0f, juce::Font::bold));
    vol1Label.setColour(juce::Label::textColourId, juce::Colours::black);

    vol2Label.setFont(juce::Font(16.0f, juce::Font::bold));
    vol2Label.setColour(juce::Label::textColourId, juce::Colours::black);

    speed1Label.setFont(juce::Font(16.0f, juce::Font::bold));
    speed1Label.setColour(juce::Label::textColourId, juce::Colours::black);

    speed2Label.setFont(juce::Font(16.0f, juce::Font::bold));
    speed2Label.setColour(juce::Label::textColourId, juce::Colours::black);

    speed1ValueLabel.setJustificationType(juce::Justification::centred);
    speed1ValueLabel.setFont(juce::Font(16.0f, juce::Font::bold));
    speed1ValueLabel.setColour(juce::Label::textColourId, juce::Colours::black);

    speed2ValueLabel.setJustificationType(juce::Justification::centred);
    speed2ValueLabel.setFont(juce::Font(16.0f, juce::Font::bold));
    speed2ValueLabel.setColour(juce::Label::textColourId, juce::Colours::black);

    crossfaderLabel.setFont(juce::Font(16.0f, juce::Font::bold));
    crossfaderLabel.setColour(juce::Label::textColourId, juce::Colours::black);

    // Make labels visible
    addAndMakeVisible(vol1Label);
    addAndMakeVisible(vol2Label);
    addAndMakeVisible(speed1Label);
    addAndMakeVisible(speed2Label);
    addAndMakeVisible(speed1ValueLabel);
    addAndMakeVisible(speed2ValueLabel);
    addAndMakeVisible(crossfaderLabel);

    // Add listeners to sliders to handle value changes
    crossfader.addListener(this);
    vol1.addListener(this);
    vol2.addListener(this);
    speed1.addListener(this);
    speed2.addListener(this);

    // Load an image from memory to display in the component
    image = juce::ImageCache::getFromMemory(BinaryData::dj_png, BinaryData::dj_pngSize);
}

// Destructor: Cleans up resources used by the MiddleDeck component, including resetting the LookAndFeel for the sliders.
MiddleDeck::~MiddleDeck()
{
    vol1.setLookAndFeel(nullptr);
    vol2.setLookAndFeel(nullptr);
    speed1.setLookAndFeel(nullptr);
    speed2.setLookAndFeel(nullptr);
    crossfader.setLookAndFeel(nullptr);
}

// paint: Handles the drawing of the MiddleDeck component.
// This method draws the background gradient, the image, and the text labels.
void MiddleDeck::paint(juce::Graphics& g)
{
    // Create a horizontal gradient from left (sky blue) to right (pink)
    juce::ColourGradient gradient(juce::Colours::skyblue, 0, 0, juce::Colours::pink, (float)getWidth(), 0, false);

    // Fill the entire background with the gradient
    g.setGradientFill(gradient);
    g.fillAll();

    // Set the color for the outline and draw it
    g.setColour(juce::Colours::white);
    g.drawRect(getLocalBounds(), 1);  // Draw an outline around the component

    // Draw the image above the text, scaling it to fit the desired area
    if (image.isValid())
    {
        g.drawImage(image, 25, 10, getWidth() * 0.8, getHeight() / 4, 0, 0, image.getWidth(), image.getHeight());
    }

    // Set the color and font for the text
    g.setColour(juce::Colours::black);
    juce::Font customFont = juce::Font(juce::Typeface::createSystemTypefaceFor(BinaryData::Rancho_ttf, BinaryData::Rancho_ttfSize));
    customFont.setHeight(55.0f);  // Set font size
    g.setFont(customFont);

    // Draw the "Otodeck DJ" text below the image
    g.drawText("Otodeck DJ", getLocalBounds().withTrimmedTop(getHeight() / 4),
        juce::Justification::centredTop, true);
}

// resized: Adjusts the layout of the sliders and labels when the component size changes.
// Positions each slider and label within the component based on the current dimensions.
void MiddleDeck::resized()
{
    int halfWidth = getWidth() / 3;
    int smallHeight = getHeight() / 7;

    // Declare label height and Y offset
    int labelHeight = 20;
    int labelYOffset = 5;

    // Set the bounds for the sliders
    speed1.setBounds(15, smallHeight * 3 - labelHeight, halfWidth - 20, smallHeight * 2);
    speed2.setBounds((halfWidth * 2) - 5, smallHeight * 3 - labelHeight, halfWidth - 20, smallHeight * 2);
    vol1.setBounds(5, smallHeight * 5 + labelYOffset, halfWidth, smallHeight - 10);
    vol2.setBounds((halfWidth * 2) - 10, smallHeight * 5 + labelYOffset, halfWidth, smallHeight - 10);
    crossfader.setBounds(10, smallHeight * 6 + labelHeight, getWidth() - 20, smallHeight / 2);

    // Set the bounds for the labels
    speed1Label.setBounds(10, smallHeight * 3 - (labelHeight * 3 + labelYOffset), halfWidth, smallHeight);
    speed2Label.setBounds(halfWidth * 2 - 10, smallHeight * 3 - (labelHeight * 3 + labelYOffset), halfWidth, smallHeight);
    speed1ValueLabel.setBounds(5, smallHeight * 4 + labelHeight * 2 - labelYOffset, halfWidth, smallHeight);
    speed2ValueLabel.setBounds(halfWidth * 2 - 15, smallHeight * 4 + labelHeight * 2 - labelYOffset, halfWidth, smallHeight);
    vol1Label.setBounds(5, smallHeight * 6, halfWidth, labelHeight);
    vol2Label.setBounds((halfWidth * 2) - 10, smallHeight * 6, halfWidth, labelHeight);
    crossfaderLabel.setBounds(0, getHeight() - labelHeight, getWidth(), labelHeight);
}

// sliderValueChanged: Called when a slider's value changes.
// Updates the corresponding DJAudioPlayer or the displayed speed value based on which slider was changed.
void MiddleDeck::sliderValueChanged(Slider* slider)
{
    if (slider == &crossfader)
    {
        // Adjust the mix ratio for both players based on the crossfader's value
        player1->setMixRatio(1.0f - slider->getValue());
        player2->setMixRatio(slider->getValue());
    }
    else if (slider == &vol1)
    {
        // Set the gain for player 1 based on the vol1 slider's value
        player1->setGain(slider->getValue());
    }
    else if (slider == &vol2)
    {
        // Set the gain for player 2 based on the vol2 slider's value
        player2->setGain(slider->getValue());
    }
    else if (slider == &speed1)
    {
        // Set the speed for player 1 and update the speed1ValueLabel
        player1->setSpeed(slider->getValue());
        speed1ValueLabel.setText(juce::String(slider->getValue(), 1) + "x", juce::dontSendNotification);
    }
    else if (slider == &speed2)
    {
        // Set the speed for player 2 and update the speed2ValueLabel
        player2->setSpeed(slider->getValue());
        speed2ValueLabel.setText(juce::String(slider->getValue(), 1) + "x", juce::dontSendNotification);
    }
}

