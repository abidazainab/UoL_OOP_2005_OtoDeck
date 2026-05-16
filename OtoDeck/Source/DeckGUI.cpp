
/*
  ==============================================================================

    DeckGUI.cpp
    Created: 28 Jul 2024 9:36:40pm
    Author:  Admin

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"
#include "PlaylistComponent.h"
#include <random>
#include <juce_core/juce_core.h> // Include this for DBG (debugging output)

//==============================================================================
// Constructor: Initializes the DeckGUI with the provided DJAudioPlayer and other dependencies.
// Sets up the GUI elements such as buttons, sliders, meters, and waveform display.
DeckGUI::DeckGUI(DJAudioPlayer* _player,
    AudioFormatManager& formatManagerToUse,
    AudioThumbnailCache& cacheToUse,
    PlaylistComponent* _playlistComponent,
    juce::Colour backgroundColor)

    : player(_player),
    waveFormDisplay(formatManagerToUse, cacheToUse),
    playlistComponent(_playlistComponent),
    trackEntries(playlistComponent->getTrackEntries()),
    backgroundColour(backgroundColor),
    jogWheel(player, backgroundColor),
    levelMeter([_player] { return _player->getRMSLevel(); }),
    levelMeterBulb([_player] { return _player->getRMSLevel(); })
{
    // Initialize and set up the load button
    addAndMakeVisible(loadButton);
    loadButton.addListener(this);
    loadButton.setButtonText("Load");
    loadButton.setLookAndFeel(&customLookAndFeel);

    // Initialize and set up the rewind button
    addAndMakeVisible(frevButton);
    frevButton.addListener(this);
    frevButton.setButtonText("Rev");
    frevButton.setLookAndFeel(&customLookAndFeel);

    // Initialize and set up the previous track button
    addAndMakeVisible(prevButton);
    prevButton.addListener(this);
    prevButton.setButtonText("Prev");
    prevButton.setLookAndFeel(&customLookAndFeel);

    // Initialize and set up the play/stop button
    addAndMakeVisible(playButton);
    playButton.addListener(this);
    playButton.setButtonText("Play");
    playButton.setLookAndFeel(&customLookAndFeel);
    playButton.onClick = [this] { togglePlay(); }; // Set the action to toggle play/stop

    // Initialize and set up the fast forward button
    addAndMakeVisible(ffwdButton);
    ffwdButton.addListener(this);
    ffwdButton.setButtonText("Forward");
    ffwdButton.setLookAndFeel(&customLookAndFeel);

    // Initialize and set up the loop button
    addAndMakeVisible(loopButton);
    loopButton.addListener(this);
    loopButton.setButtonText("Loop");
    loopButton.setLookAndFeel(&customLookAndFeel);

    // Initialize and set up the first cue buttons
    addAndMakeVisible(setCueButton1);
    setCueButton1.addListener(this);
    setCueButton1.setButtonText("Set Cue 1");
    setCueButton1.setColour(juce::TextButton::buttonColourId, juce::Colours::blue);

    addAndMakeVisible(playCueButton1);
    playCueButton1.addListener(this);
    playCueButton1.setButtonText("Play Cue");
    playCueButton1.setColour(juce::TextButton::buttonColourId, juce::Colours::magenta);

    addAndMakeVisible(clearCueButton1);
    clearCueButton1.addListener(this);
    clearCueButton1.setButtonText("Clear Cue");
    clearCueButton1.setColour(juce::TextButton::buttonColourId, juce::Colours::purple);

    // Initialize and set up the second cue buttons
    addAndMakeVisible(setCueButton2);
    setCueButton2.addListener(this);
    setCueButton2.setButtonText("Set Cue 2");
    setCueButton2.setColour(juce::TextButton::buttonColourId, juce::Colours::blue);

    addAndMakeVisible(playCueButton2);
    playCueButton2.addListener(this);
    playCueButton2.setButtonText("Play Cue");
    playCueButton2.setColour(juce::TextButton::buttonColourId, juce::Colours::magenta);

    addAndMakeVisible(clearCueButton2);
    clearCueButton2.addListener(this);
    clearCueButton2.setButtonText("Clear Cue");
    clearCueButton2.setColour(juce::TextButton::buttonColourId, juce::Colours::purple);

    // Initialize and set up the bass slider
    addAndMakeVisible(bassSlider);
    bassSlider.addListener(this);
    bassSlider.setRange(0.0, 1.0);
    bassSlider.setValue(0.0);

    // Initialize and set up the bass label
    bassLabel.setText("Bass", juce::dontSendNotification);
    addAndMakeVisible(bassLabel);
    bassLabel.setJustificationType(juce::Justification::centred);
    bassLabel.setFont(juce::Font(16.0f, juce::Font::bold));
    bassLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    bassSlider.setSliderStyle(Slider::Rotary);
    bassSlider.setLookAndFeel(&customLookAndFeel);
    bassSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);

    // Initialize and set up the mid slider
    addAndMakeVisible(midSlider);
    midSlider.addListener(this);
    midSlider.setRange(0.0, 1.0);
    midSlider.setValue(0.0);

    // Initialize and set up the mid label
    midLabel.setText("Mid", juce::dontSendNotification);
    addAndMakeVisible(midLabel);
    midLabel.setJustificationType(juce::Justification::centred);
    midLabel.setFont(juce::Font(16.0f, juce::Font::bold));
    midLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    midSlider.setSliderStyle(Slider::Rotary);
    midSlider.setLookAndFeel(&customLookAndFeel);
    midSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);

    // Initialize and set up the treble slider
    addAndMakeVisible(trebSlider);
    trebSlider.addListener(this);
    trebSlider.setRange(0.0, 1.0);
    trebSlider.setValue(0.0);

    // Initialize and set up the treble label
    trebLabel.setText("Treble", juce::dontSendNotification);
    addAndMakeVisible(trebLabel);
    trebLabel.setJustificationType(juce::Justification::centred);
    trebLabel.setFont(juce::Font(16.0f, juce::Font::bold));
    trebLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    trebSlider.setSliderStyle(Slider::Rotary);
    trebSlider.setLookAndFeel(&customLookAndFeel);
    trebSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);

    // Initialize and set up the level meters and waveform display
    addAndMakeVisible(levelMeter);
    addAndMakeVisible(levelMeterBulb);
    addAndMakeVisible(waveFormDisplay);
    waveFormDisplay.setPlayer(player);
    addAndMakeVisible(jogWheel);

    // Initialize and set up the track name display
    trackName.setAlpha(0.8);
    addAndMakeVisible(trackName);
    trackName.setJustificationType(juce::Justification::left);
    trackName.setFont(juce::Font(20.0f, juce::Font::bold));
    trackName.setColour(juce::Label::textColourId, juce::Colours::black);

    // Initialize and set up the time display
    timeDisplay.setAlpha(0.8);
    addAndMakeVisible(timeDisplay);
    timeDisplay.setJustificationType(juce::Justification::left);
    timeDisplay.setFont(juce::Font(17.0f, juce::Font::bold));
    timeDisplay.setColour(juce::Label::textColourId, juce::Colours::black);

    // Start the timer to update the GUI periodically
    startTimer(500);  // Update every 500ms
}

// Destructor: Cleans up the resources used by the DeckGUI.
DeckGUI::~DeckGUI()
{
    stopTimer();  // Stop the timer when the DeckGUI is destroyed
}

// paint: Draws the background and GUI elements.
void DeckGUI::paint(juce::Graphics& g)
{
    g.setColour(backgroundColour);  // Set the background color
    g.fillAll();  // Fill the background with the color

    // Draw an outline around the track name and time display
    g.drawRect(trackName.getBounds(), 1);
    g.drawRect(timeDisplay.getBounds(), 1);
    // Set the font and draw placeholder text (if needed)
    g.setColour(juce::Colours::white);
    g.setFont(14.0f);
    g.drawText("", getLocalBounds(), juce::Justification::centred, true);
}

// resized: Lays out the child components within the DeckGUI when resized.
void DeckGUI::resized()
{
    int sliderWidth = getWidth() * 5 / 40;
    int sliderHeight = getHeight() * 5 / 40;

    double rowH = getHeight() / 18;
    double rowW = (getWidth() / 2) - 30;
    int buttonWidth = getWidth() / 6;

    int buttonSpacing = 25.0;  // Adjust this value as needed for more space

    // Set bounds for each button with adjusted spacing
    loadButton.setBounds(0, rowH * 2 + 10, getWidth() / 4, rowH);
    frevButton.setBounds(0 + buttonSpacing, rowH * 13, buttonWidth, rowH);
    prevButton.setBounds(buttonSpacing + buttonWidth, rowH * 13, buttonWidth, rowH);
    playButton.setBounds(buttonSpacing + buttonWidth * 2, rowH * 13, buttonWidth, rowH);
    ffwdButton.setBounds(buttonSpacing + buttonWidth * 3, rowH * 13, buttonWidth, rowH);
    loopButton.setBounds(buttonSpacing + buttonWidth * 4, rowH * 13, buttonWidth, rowH);

    // Set bounds for cue buttons
    setCueButton1.setBounds(20, rowH * 5, rowH * 2, rowH);
    playCueButton1.setBounds(20, rowH * 6, rowH * 2, rowH);
    clearCueButton1.setBounds(20, rowH * 7, rowH * 2, rowH);

    setCueButton2.setBounds(20, rowH * 9, rowH * 2, rowH);
    playCueButton2.setBounds(20, rowH * 10, rowH * 2, rowH);
    clearCueButton2.setBounds(20, rowH * 11, rowH * 2, rowH);

    // Set bounds for waveform display, jog wheel, and level meters
    waveFormDisplay.setBounds(0, 0, getWidth(), rowH * 2);
    jogWheel.setBounds(sliderWidth * 2, rowH * 3, sliderWidth * 6, sliderHeight * 4);
    levelMeterBulb.setBounds(rowW + sliderWidth * 4 - 30, rowH * 13, 20, sliderHeight * 2);
    levelMeter.setBounds(rowW + sliderWidth * 4, rowH * 13, 20, sliderHeight * 2);

    // Set bounds for EQ sliders and labels
    bassSlider.setBounds(rowW - sliderWidth * 2, rowH * 15, sliderWidth, sliderHeight);
    midSlider.setBounds(rowW, rowH * 15, sliderWidth, sliderHeight);
    trebSlider.setBounds(rowW + sliderWidth * 2, rowH * 15, sliderWidth, sliderHeight);

    bassLabel.setBounds(rowW - sliderWidth * 2, rowH * 16 + 15, sliderWidth, sliderHeight);
    midLabel.setBounds(rowW, rowH * 16 + 15, sliderWidth, sliderHeight);
    trebLabel.setBounds(rowW + sliderWidth * 2, rowH * 16 + 15, sliderWidth, sliderHeight);

    // Set bounds for track name and time display
    trackName.setBounds(rowW + sliderWidth * 2, rowH * 2, getWidth() / 4, rowH);
    timeDisplay.setBounds(rowW + sliderWidth * 2, rowH * 3, getWidth() / 4, rowH);
}

// togglePlay: Toggles between play and stop states.
void DeckGUI::togglePlay()
{
    if (player->isPlaying()) {
        player->stop();  // Stop playback
        playButton.setButtonText("Play");  // Update button text to "Play"
    }
    else {
        player->start();  // Start playback
        playButton.setButtonText("Stop");  // Update button text to "Stop"
    }
}

// buttonClicked: Handles button click events.
void DeckGUI::buttonClicked(Button* button)
{
    if (button == &loadButton)
    {
        auto fileChooserFlags =
            FileBrowserComponent::canSelectFiles;

        fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser)
            {
                File chosenFile = chooser.getResult();
                if (chosenFile.existsAsFile())
                {
                    URL audioURL = URL{ chosenFile };
                    deckCleared = false;

                    // Check for duplicates in the playlist
                    auto trackEntries = playlistComponent->getTrackEntries();
                    bool isDuplicate = std::any_of(trackEntries->begin(), trackEntries->end(),
                        [&audioURL](const TrackEntry& entry) { return entry.audioURL == audioURL; });

                    if (isDuplicate)
                    {
                        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                            "Duplicate Track",
                            "The track '" + audioURL.getFileName() + "' is already in the playlist and will not be added again.",
                            "OK");
                    }
                    else
                    {
                        std::cout << "Loading file: " << chosenFile.getFullPathName() << std::endl;
                        player->loadURL(audioURL);
                        waveFormDisplay.loadURL(audioURL);
                        trackName.setText(audioURL.getFileName(), dontSendNotification);
                        playlistComponent->addToTrackEntries(audioURL, player->getTrackDuration());
                    }
                }
            });
    }

    // Handle rewind button click
    if (button == &frevButton)
    {
        player->stop();
        player->setPosition(0);
        player->start();
        playButton.setToggleState(true, dontSendNotification);
    }

    // Handle fast-forward button click (loads the next track in the playlist)
    if (button == &ffwdButton)
    {
        loopButton.setToggleState(false, dontSendNotification);
        if (trackEntries->size() == 0) return;

        player->stop();
        URL audioURL = playlistComponent->getNextTrack(shuffle);
        player->loadURL(audioURL);
        waveFormDisplay.loadURL(audioURL);
        trackName.setText(audioURL.getFileName(), dontSendNotification);
        player->start();
        playButton.setToggleState(true, dontSendNotification);
    }

    // Handle previous track button click
    if (button == &prevButton)
    {
        loopButton.setToggleState(false, dontSendNotification);
        if (trackEntries->size() == 0) return;

        player->stop();
        URL audioURL = playlistComponent->getPreviousTrack(shuffle);
        player->loadURL(audioURL);
        waveFormDisplay.loadURL(audioURL);
        trackName.setText(audioURL.getFileName(), dontSendNotification);
        player->start();
        playButton.setToggleState(true, dontSendNotification);
    }

    // Handle loop button click
    if (button == &loopButton)
    {
        isLooping = !isLooping;  // Toggle loop mode
        loopButton.setColour(juce::TextButton::textColourOffId, isLooping ? juce::Colours::red : juce::Colours::black);
    }

    // Handle cue setting, playing, and clearing for cue 1
    if (button == &setCueButton1)
    {
        cueStartPosition1 = player->getPositionRelative();
        setCueButton1.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
    }
    if (button == &playCueButton1)
    {
        if (cueStartPosition1 >= 0.0 && cueStartPosition1 <= 1.0)
        {
            player->setPositionRelative(cueStartPosition1);
            player->start();
        }
    }
    if (button == &clearCueButton1)
    {
        cueStartPosition1 = -1;
        setCueButton1.setColour(juce::TextButton::buttonColourId, juce::Colours::blue);
    }

    // Handle cue setting, playing, and clearing for cue 2
    if (button == &setCueButton2)
    {
        cueStartPosition2 = player->getPositionRelative();
        setCueButton2.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
    }
    if (button == &playCueButton2)
    {
        if (cueStartPosition2 >= 0.0 && cueStartPosition2 <= 1.0)
        {
            player->setPositionRelative(cueStartPosition2);
            player->start();
        }
    }
    if (button == &clearCueButton2)
    {
        cueStartPosition2 = -1;
        setCueButton2.setColour(juce::TextButton::buttonColourId, juce::Colours::blue);
    }
}

// sliderValueChanged: Handles slider value change events for the bass, mid, and treble EQ sliders.
void DeckGUI::sliderValueChanged(Slider* slider)
{
    if (slider == &bassSlider)
    {
        player->setBass(bassSlider.getValue());
    }
    else if (slider == &midSlider)
    {
        player->setMid(midSlider.getValue());
    }
    else if (slider == &trebSlider)
    {
        player->setTreble(trebSlider.getValue());
    }
}

// isInterestedInFileDrag: Determines if the DeckGUI is interested in the files being dragged.
bool DeckGUI::isInterestedInFileDrag(const StringArray& files)
{
    std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
    return true;
}

// filesDropped: Handles files being dropped onto the DeckGUI.
void DeckGUI::filesDropped(const juce::StringArray& files, int x, int y)
{
    std::cout << "DeckGUI::filesDropped" << std::endl;
    if (files.size() == 1)
    {
        juce::File droppedFile{ files[0] };
        juce::URL fileURL{ droppedFile };
        player->loadURL(fileURL);
        waveFormDisplay.loadURL(fileURL);
        trackName.setText(droppedFile.getFileName(), juce::dontSendNotification);
        playlistComponent->addToTrackEntries(fileURL, player->getTrackDuration());
    }
}

// secondsToString: Converts seconds to a formatted string "mm:ss".
juce::String DeckGUI::secondsToString(double seconds)
{
    int minutes = static_cast<int>(seconds) / 60;
    int secs = static_cast<int>(seconds) % 60;
    return juce::String::formatted("%02d:%02d", minutes, secs);
}

// timerCallback: Periodically updates the GUI elements, such as waveform position and time display.
void DeckGUI::timerCallback()
{
    if (deckCleared)
    {
        return;  // Skip updates if the deck is cleared
    }
    if (!isnan(player->getPositionRelative()))
    {
        waveFormDisplay.setPositionRelative(player->getPositionRelative());

        double currentTime = player->getCurrentPosition();
        double totalTime = player->getTotalLength();

        juce::String currentTimeString = secondsToString(currentTime);
        juce::String totalTimeString = secondsToString(totalTime);

        timeDisplay.setText(currentTimeString + " / " + totalTimeString, juce::dontSendNotification);
    
        // Skip updates if the deck is cleared
        if (deckCleared)
            {
                return;  
            }
    }

    if (player->getPositionRelative() >= 1)
    {
        if (isLooping)
        {
            player->setPositionRelative(0);
            player->start();
        }
        else if (!isLooping)
        {
            player->setPositionRelative(0);
            player->stop();
        }
    }
}

// clearDeck: Resets the DeckGUI, clearing the track information and resetting controls to their default states.
void DeckGUI::clearDeck()
{
    deckCleared = true;
    trackName.setText("", juce::NotificationType::dontSendNotification);
    timeDisplay.setText("00:00 / 00:00", juce::NotificationType::sendNotification);
    waveFormDisplay.fileLoaded = false;
    waveFormDisplay.repaint(); // Ensure the waveform display is cleared
    player->stop();
    player->setPosition(0);  // Reset the player's position
    isLooping = false;

    playButton.setButtonText("Play");
    loopButton.setColour(juce::TextButton::textColourOffId, juce::Colours::black);
    cueStartPosition1 = -1.0;
    cueStartPosition2 = -1.0;
    setCueButton1.setColour(juce::TextButton::buttonColourId, juce::Colours::blue);
    setCueButton2.setColour(juce::TextButton::buttonColourId, juce::Colours::blue);

    bassSlider.setValue(1.0);
    midSlider.setValue(1.0);
    trebSlider.setValue(1.0);
}
