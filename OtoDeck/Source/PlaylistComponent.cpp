
/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 30 Jul 2024 12:48:38pm
    Author:  Admin

  ==============================================================================
*/

#include <JuceHeader.h> // JUCE core headers for UI and audio functionality
#include "PlaylistComponent.h" // Including PlaylistComponent header file for class definitions
#include "PlaylistFilterManager.h" // Header for managing playlist filters (favourites, all tracks)

#include <random> // C++ standard library header for random number generation
#include <juce_core/juce_core.h> // JUCE Core utilities for data manipulation, threading, and basic system functions


//==============================================================================
// Constructor for PlaylistComponent - initializes child components, settings, and internal variables
PlaylistComponent::PlaylistComponent(DeckGUI* _deckGUI1,
    DeckGUI* _deckGUI2)

    // Initialising member variables in constructor
    : trackFound(false), // Boolean to indicate if track is found during search
    deckGUI1(_deckGUI1), // Deck 1 object (pass pointer from GUI)
    deckGUI2(_deckGUI2), // Deck 2 object (pass pointer from GUI)
    filterManager(trackEntries) // Instantiate PlaylistFilterManager to manage track filtering
{
    // Register audio formats (e.g. .mp3, .wav) with format manager to handle various file types
    formatManager.registerBasicFormats();

    // Adding buttons for track insert and filter actions to the GUI and making them visible
    addAndMakeVisible(insertButton);
    addAndMakeVisible(filterAll);
    addAndMakeVisible(filterFav);

    // Adding and defining columns for the playlist table (e.g. title, duration, preview, etc.)
    tableComponent.getHeader().addColumn("Track title", 1, 200);
    tableComponent.getHeader().addColumn("Duration", 2, 100);
    tableComponent.getHeader().addColumn("Add to favourite", 3, 100);
    tableComponent.getHeader().addColumn("Preview", 4, 200);
    tableComponent.getHeader().addColumn("Delete", 5, 100);
    tableComponent.getHeader().addColumn("Left", 6, 100);
    tableComponent.getHeader().addColumn("Right", 7, 100);

    // Set the table model to this class to manage data for the table
    tableComponent.setModel(this);
    addAndMakeVisible(tableComponent);

    // Adding event listeners to the insert and filter buttons (action listeners handle button clicks)
    insertButton.addListener(this);
    filterAll.addListener(this);
    filterFav.addListener(this);

    /* SEARCH BAR */
    // Adding and configuring search bar (for user input) to filter tracks in playlist
    addAndMakeVisible(search);
    search.addListener(this); // Attach listener to handle search actions
    search.setTextToShowWhenEmpty("Search", juce::Colours::white); // Placeholder text in search bar
    search.setJustification(juce::Justification::centredLeft); // Align the placeholder text to the left

    // Loading images for favourite (heart icon) in black and red for toggling favourite status
    heartBlackImg = juce::ImageCache::getFromMemory(BinaryData::heart_black_png, BinaryData::heart_black_pngSize);
    heartRedImg = juce::ImageCache::getFromMemory(BinaryData::heart_red_png, BinaryData::heart_red_pngSize);

}

// Destructor to handle clean-up of resources and pointers
PlaylistComponent::~PlaylistComponent()
{
    // Reset the table model to null to prevent invalid access if PlaylistComponent is destroyed before table
    tableComponent.setModel(nullptr);

    // Reset pointers to DeckGUI1 and DeckGUI2 to avoid dangling pointers
    deckGUI1 = nullptr;
    deckGUI2 = nullptr;

    // Additional clean-up code can be added here if required
}

// paint method for the PlaylistComponent, where custom graphics and UI elements are drawn
void PlaylistComponent::paint(juce::Graphics& g)
{
    // Fill the background with a sky blue to pink gradient for visual appeal
    juce::ColourGradient gradient(juce::Colours::skyblue, 0, 0, juce::Colours::pink, (float)getWidth(), 0, false);

    // Apply the gradient to fill the entire component background
    g.setGradientFill(gradient);
    g.fillAll();

    // Set text colour to white and define a font size of 14.0f for the text
    g.setColour(juce::Colours::white);
    g.setFont(14.0f);

    // Draw the component label "PlaylistComponent" centered within the bounds
    g.drawText("PlaylistComponent", getLocalBounds(),
        juce::Justification::centred, true);
}

// resized method is called when the component size is adjusted, used to position child components dynamically
void PlaylistComponent::resized()
{
    // Calculate row height (rowH) and column width (colW) based on the component size
    double rowH = getHeight() / 5;
    double colW = getWidth() / 10;

    // Set the bounds for the playlist table (fills most of the component's area)
    tableComponent.setBounds(0, rowH * 0.8, getWidth(), getHeight());

    // Positioning search bar and buttons dynamically based on calculated row height and column width
    search.setBounds(getWidth() * 0.6, rowH * 0.1, getWidth() * 0.2, rowH * 0.6);
    insertButton.setBounds(0, rowH * 0.2, colW, rowH * 0.6);
    filterAll.setBounds(colW, rowH * 0.2, colW, rowH * 0.6);
    filterFav.setBounds(colW * 2, rowH * 0.2, colW, rowH * 0.6);
}

// buttonClicked method is called when any button in the component is clicked (insert, filter, etc.)
void PlaylistComponent::buttonClicked(juce::Button* button)
{
    // If insertButton is clicked, launch file chooser to select audio files to add to the playlist
    if (button == &insertButton)
    {
        auto fileChooserFlags =
            FileBrowserComponent::canSelectFiles | FileBrowserComponent::canSelectMultipleItems; // Enable multi-select

        // Launch the file chooser asynchronously and process selected files
        mChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser)
            {
                auto chosenFiles = chooser.getResults(); // Get selected files as an array
                juce::String alreadyLoadedTracks; // String to store names of already loaded files

                if (chosenFiles.size() > 0)
                {
                    // Iterate through the selected files
                    for (const File& chosenFile : chosenFiles)
                    {
                        if (chosenFile.existsAsFile()) // Ensure the file exists
                        {
                            URL audioURL = URL{ chosenFile }; // Create a URL object for the audio file

                            // Check if the track is already in the playlist
                            if (std::any_of(trackEntries.begin(), trackEntries.end(),
                                [&audioURL](const TrackEntry& entry) { return entry.audioURL == audioURL; }))
                            {
                                alreadyLoadedTracks += audioURL.getFileName() + "\n"; // Add to duplicate list
                            }
                            else
                            {
                                // Add the new track to the playlist (without loading it into deckGUI)
                                addToTrackEntries(audioURL, deckGUI1->player->getTrackDurationWithoutLoadingIntoDeck(audioURL));
                            }
                        }
                    }

                    // If duplicates were found, show a message box to notify the user
                    if (!alreadyLoadedTracks.isEmpty())
                    {
                        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                            "Duplicate Tracks",
                            "The following tracks were already loaded and not added again:\n" + alreadyLoadedTracks,
                            "OK");
                    }
                }
            });
    }
    else if (button == &filterAll) // If filterAll button is clicked, reset the filter to show all tracks
    {
        filterManager.filterAll(); // Show all tracks
        tableComponent.updateContent(); // Refresh the table to reflect the filter change
        repaint(); // Redraw the component
    }
    else if (button == &filterFav) // If filterFav button is clicked, filter to show only favourite tracks
    {
        filterManager.filterFavourites(); // Show only favourite tracks
        tableComponent.updateContent(); // Refresh the table to reflect the filter change
        repaint(); // Redraw the component
    }
    else if (button->getComponentID().length() >= 2) // Handle buttons dynamically by their component IDs (e.g. "Delete", "Left", "Right")
    {
        int rowIndex = std::stoi(button->getComponentID().toStdString().substr(0, 1)); // Extract row index
        int colIndex = std::stoi(button->getComponentID().toStdString().substr(1, 2)); // Extract column index

        auto& entries = filterManager.getFilteredTracks(); // Get filtered track entries

        // Ensure the rowIndex is valid (within bounds of entries vector)
        if (rowIndex >= 0 && rowIndex < entries.size())
        {
            TrackEntry& entry = entries[rowIndex]; // Get the track entry for the clicked row

            if (colIndex == 3) // If "Add to Favourite" column is clicked, toggle the favourite status
            {
                entry.isFavourite = !entry.isFavourite; // Toggle favourite status of the track

                // Find the entry in the main trackEntries vector and update its status
                auto it = std::find(trackEntries.begin(), trackEntries.end(), entry);
                if (it != trackEntries.end())
                {
                    it->isFavourite = entry.isFavourite; // Update the favourite status
                }

                // Update the favourite button's icon based on the new status (red heart or black heart)
                auto* imgButton = dynamic_cast<juce::ImageButton*>(button);
                if (imgButton)
                {
                    imgButton->setToggleState(entry.isFavourite, juce::sendNotification);
                    imgButton->setImages(true, true, true,
                        entry.isFavourite ? heartRedImg : heartBlackImg, 1.0f, juce::Colours::transparentWhite,
                        entry.isFavourite ? heartRedImg : heartBlackImg, 1.0f, juce::Colours::transparentWhite,
                        entry.isFavourite ? heartRedImg : heartBlackImg, 1.0f, juce::Colours::transparentWhite);
                }

                // Reapply the filter (show all or only favourites, depending on current filter type)
                if (filterManager.getCurrentFilterType() == PlaylistFilterManager::FilterType::Favourites)
                {
                    filterManager.filterFavourites();
                }
                else
                {
                    filterManager.filterAll(); // Ensure all tracks are visible in case of filterAll
                }
                tableComponent.updateContent(); // Refresh the table content to reflect changes
                repaint(); // Redraw the component
            }

            else if (colIndex == 5) // If "Delete" column is clicked, remove the track from the playlist
            {
                // Check if the track is loaded into deckGUI1 or deckGUI2, and clear it if necessary
                if (isLoadedtoDeck1 && deckGUI1->player->getLoadedURL() == entry.audioURL)
                {
                    deckGUI1->clearDeck(); // Clear the track from deckGUI1
                    isLoadedtoDeck1 = false;
                }
                if (isLoadedtoDeck2 && deckGUI2->player->getLoadedURL() == entry.audioURL)
                {
                    deckGUI2->clearDeck(); // Clear the track from deckGUI2
                    isLoadedtoDeck2 = false;
                }

                // Remove the track from the main trackEntries vector
                auto it = std::remove(trackEntries.begin(), trackEntries.end(), entry);
                if (it != trackEntries.end())
                {
                    trackEntries.erase(it, trackEntries.end()); // Erase the track from the vector
                }

                // Reapply the filter (all tracks or favourites) to reflect the removal
                if (filterManager.getCurrentFilterType() == PlaylistFilterManager::FilterType::All)
                {
                    filterManager.filterAll();
                }
                else
                {
                    filterManager.filterFavourites();
                }

                tableComponent.updateContent(); // Refresh the table to reflect changes
                repaint(); // Redraw the component
            }
            else if (colIndex == 6)   // If "Left" column is clicked, load the track into deckGUI1
            {
                deckGUI1->deckCleared = false; // Set the deckCleared flag to false (deck is not empty)
                deckGUI1->playButton.setToggleState(false, dontSendNotification); // Ensure play button is in a stopped state
                deckGUI1->player->loadURL(entry.audioURL); // Load the selected track into deckGUI1's player
                deckGUI1->waveFormDisplay.loadURL(entry.audioURL); // Load the waveform for visual representation
                deckGUI1->waveFormDisplay.setPositionRelative(deckGUI1->player->getPositionRelative()); // Sync the waveform with the player's position
                deckGUI1->trackName.setText(entry.audioURL.getFileName(), dontSendNotification); // Display the track name in deckGUI1
                isLoadedtoDeck1 = true; // Set the flag to indicate that the deck has a loaded track
            }
            else if (colIndex == 7)   // If "Right" column is clicked, load the track into deckGUI2
            {
                deckGUI2->deckCleared = false; // Set the deckCleared flag to false (deck is not empty)
                deckGUI2->playButton.setToggleState(false, dontSendNotification); // Ensure play button is in a stopped state
                deckGUI2->player->loadURL(entry.audioURL); // Load the selected track into deckGUI2's player
                deckGUI2->waveFormDisplay.loadURL(entry.audioURL); // Load the waveform for visual representation
                deckGUI2->waveFormDisplay.setPositionRelative(deckGUI1->player->getPositionRelative()); // Sync the waveform with the player's position
                deckGUI2->trackName.setText(entry.audioURL.getFileName(), dontSendNotification); // Display the track name in deckGUI2
                isLoadedtoDeck2 = true; // Set the flag to indicate that the deck has a loaded track
            }
        }
    }
}

// Get the number of rows in the playlist table, which is determined by the number of filtered tracks
int PlaylistComponent::getNumRows()
{
    // Fetch the filtered tracks from the filter manager (all tracks or favourites based on the filter)
    auto& entries = filterManager.getFilteredTracks();

    // Return the size of the filtered track list, cast to an integer
    return static_cast<int>(entries.size());
}

// Paint the contents of individual cells in the playlist table
void PlaylistComponent::paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
    // Get the filtered track entries to display in the table
    auto& entries = filterManager.getFilteredTracks();

    // Ensure the row number is within valid bounds of the track list
    if (rowNumber >= 0 && rowNumber < entries.size())
    {
        // Get the track entry for the current row
        const TrackEntry& entry = entries[rowNumber];

        // If the current column is the Title column (ID 1), draw the track title
        if (columnId == 1) // Title column
            g.drawText(entry.title, 2, 0, width - 4, height, juce::Justification::centredLeft, true);
        // If the current column is the Duration column (ID 2), draw the track duration
        else if (columnId == 2) // Duration column
            g.drawText(entry.duration, 2, 0, width - 4, height, juce::Justification::centred, true);
    }
}

// Paint the background for each row in the playlist table, with different colors for selected rows
void PlaylistComponent::paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected)
{
    // If the row is selected, fill it with light blue, otherwise fill it with pink
    if (rowIsSelected)
    {
        g.fillAll(juce::Colours::lightblue);
    }
    else
    {
        g.fillAll(juce::Colours::pink);
    }
}

// Refresh the custom components (e.g., buttons, waveform displays) used for each cell in the table
Component* PlaylistComponent::refreshComponentForCell(int rowNumber,
    int columnId, bool isRowSelected,
    Component* existingComponentToUpdate)
{
    // Check if the column is the Preview column (ID 4), where the waveform display is rendered
    if (columnId == 4)
    {
        // Try to reuse the existing WaveFormDisplay component if it exists
        auto* waveFormDisplay = dynamic_cast<WaveFormDisplay*>(existingComponentToUpdate);

        // If there is no existing waveform display, create a new one
        if (waveFormDisplay == nullptr)
        {
            waveFormDisplay = new WaveFormDisplay(formatManager, thumbnailCache);
            existingComponentToUpdate = waveFormDisplay;
        }

        // Load the appropriate track's waveform into the display (either from search results or the main list)
        if (trackFound)
        {
            waveFormDisplay->loadURL(trackEntriesFound[rowNumber].audioURL);
        }
        else
        {
            waveFormDisplay->loadURL(trackEntries[rowNumber].audioURL);
        }

        // Return the updated waveform display component
        return waveFormDisplay;
    }
    // If the column is the Favourite column (ID 3), handle the favourite button
    else if (columnId == 3) // Add to favourite column
    {
        // Try to reuse the existing favourite button component if it exists
        auto* favouriteButton = dynamic_cast<juce::ImageButton*>(existingComponentToUpdate);

        // If there is no existing favourite button, create a new one
        if (favouriteButton == nullptr)
        {
            favouriteButton = new juce::ImageButton();
            favouriteButton->setClickingTogglesState(true); // Set button to toggle state on click

            // Assign a unique component ID based on the row and column index (e.g., "03" for row 0, column 3)
            favouriteButton->setComponentID(std::to_string(rowNumber) + std::to_string(columnId));
            favouriteButton->addListener(this); // Add a listener to handle button clicks

            existingComponentToUpdate = favouriteButton;
        }

        // Update the button image based on whether the track is marked as a favourite or not (red or black heart)
        auto& entries = filterManager.getFilteredTracks();
        favouriteButton->setImages(true, true, true,
            entries[rowNumber].isFavourite ? heartRedImg : heartBlackImg, 1.0f, juce::Colours::transparentWhite,
            entries[rowNumber].isFavourite ? heartRedImg : heartBlackImg, 1.0f, juce::Colours::transparentWhite,
            entries[rowNumber].isFavourite ? heartRedImg : heartBlackImg, 1.0f, juce::Colours::transparentWhite);

        return favouriteButton;
    }
    // If the column is for Delete, Left (Deck 1), or Right (Deck 2) buttons, create buttons accordingly
    else if (columnId >= 5 && columnId <= 7)
    {
        // Try to reuse an existing button component if it exists
        auto* btn = dynamic_cast<TextButton*>(existingComponentToUpdate);

        // If no button exists, create a new one with appropriate labels ("Delete", "Left", "Right")
        if (btn == nullptr)
        {
            btn = new TextButton{ columnId == 5 ? "Delete" : columnId == 6 ? "Left" : "Right" };
            btn->addListener(this); // Add a listener to handle button clicks

            // Assign a unique component ID based on the row and column index
            btn->setComponentID(std::to_string(rowNumber) + std::to_string(columnId));
            existingComponentToUpdate = btn;
        }
        return btn;
    }
    else
    {
        return nullptr; // If the column is not handled, return nullptr
    }
}

// Handle actions when the user presses the return key in the search bar (used to search for tracks)
void PlaylistComponent::textEditorReturnKeyPressed(juce::TextEditor& textEditor)
{
    // Get the user's search input and remove any leading or trailing whitespace
    juce::String searchInput = textEditor.getText().trim();

    // If the search input is empty, show all tracks and display a warning
    if (searchInput.isEmpty())
    {
        // Reset the filter to show all tracks
        filterManager.filterAll();

        // Show an alert box warning the user that the search box is empty
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
            "Search",
            "Search box is empty. Displaying all tracks.",
            "OK");
    }
    else
    {
        // If search input is provided, search for tracks that match the input
        searchForTrack(searchInput);
    }

    // Refresh the table to reflect any changes from the search
    tableComponent.updateContent();
    repaint(); // Redraw the component
}

// Search for tracks in the playlist that match the user's search input
void PlaylistComponent::searchForTrack(juce::String searchInput)
{
    std::vector<TrackEntry> foundEntries; // Vector to store tracks that match the search input

    // Iterate through all the tracks in the playlist and check if the title contains the search input
    for (const auto& entry : trackEntries)
    {
        if (entry.title.containsIgnoreCase(searchInput)) // Case-insensitive search
        {
            foundEntries.push_back(entry); // Add matching tracks to the foundEntries vector
        }
    }

    // If matching tracks are found, update the filter manager with the search results
    if (!foundEntries.empty())
    {
        filterManager.setFilteredTracks(foundEntries);
    }
    // If no matching tracks are found, display a warning message
    else
    {
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
            "Search",
            "No tracks found!",
            "OK");

        // Reset the filter to show all tracks again
        filterManager.filterAll();
    }

    // Update the table to display the search results (or all tracks if no match was found)
    tableComponent.updateContent();
    repaint();
}

// Utility function to check if one string contains another, ignoring case (helper function for searching)
bool PlaylistComponent::containsIgnoreCase(const juce::String& str, const juce::String& searchString)
{
    return str.containsIgnoreCase(searchString); // Perform case-insensitive substring search
}

/* Adds a new track to the trackEntries vector and updates the table to reflect the new entry */
void PlaylistComponent::addToTrackEntries(URL audioURL, juce::String duration)
{
    // Perform the track addition asynchronously to avoid blocking the UI thread
    juce::MessageManager::callAsync([this, audioURL, duration]()
        {
            // Add the new track (with URL and duration) to the trackEntries vector
            trackEntries.push_back(TrackEntry{ audioURL, duration });

            // Refresh the filter to show all tracks (including the newly added one)
            filterManager.filterAll();

            // Update the table to include the new track
            tableComponent.updateContent();

            // Repaint the UI to reflect the changes
            repaint();
        });
}

// Prints all track entries (for debugging or logging purposes)
void PlaylistComponent::printTrackEntries()
{
    // Loop through each track in the trackEntries vector and print its details
    for (int i = 0; i < trackEntries.size(); i++)
    {
        std::cout << "Track Entries " << i << ": " << trackEntries[i].toString() << std::endl;
    }
}

// Function to get the next track in the playlist when the fast-forward button is clicked in deckGUI
// Returns a random track if shuffle is enabled, otherwise returns the next track in sequence
URL PlaylistComponent::getNextTrack(bool shuffle)
{
    URL audioURL; // Variable to store the URL of the next track

    // Check if the trackEntries vector is not empty
    if (trackEntries.size() != 0)
    {
        // If shuffle is disabled, return the next track in sequence
        if (!shuffle)
        {
            // Increment trackCounter if it hasn't reached the last track in the list
            if (trackCounter < trackEntries.size() - 1)
            {
                trackCounter++;
            }
            // If the last track is reached, loop back to the first track
            else
            {
                trackCounter = 0;
            }
            // Get the URL of the next track
            audioURL = trackEntries[trackCounter].audioURL;
        }
        // If shuffle is enabled, return a random track
        else
        {
            /* Random number generation tools */
            std::random_device rand_dev; // Seed the random number generator
            std::mt19937 generator(rand_dev()); // Mersenne Twister engine for randomness
            std::uniform_int_distribution<int>  dist(0, int(trackEntries.size() - 1)); // Uniform distribution over track entries

            // Generate a random track index
            int r = dist(generator);

            // Ensure the randomly selected track is not the same as the currently playing track
            while (r == trackCounter)
            {
                r = dist(generator); // Generate a new random number if the same track is selected
            }

            // Get the URL of the randomly selected track
            audioURL = trackEntries[r].audioURL;

            // Update trackCounter to keep track of the current playing song
            trackCounter = r;
        }
    }

    // Return the URL of the next track (either sequential or random)
    return audioURL;
}

// Function to get the previous track in the playlist when the rewind button is clicked in deckGUI
// Returns a random track if shuffle is enabled, otherwise returns the previous track in sequence
URL PlaylistComponent::getPreviousTrack(bool shuffle)
{
    URL audioURL; // Variable to store the URL of the previous track

    // Check if the trackEntries vector is not empty
    if (trackEntries.size() != 0)
    {
        // If shuffle is disabled, return the previous track in sequence
        if (!shuffle)
        {
            // Decrement trackCounter if it's greater than 0 (move to the previous track)
            if (trackCounter > 0)
            {
                trackCounter--;
            }
            // If the first track is reached, loop back to the last track
            else
            {
                trackCounter = trackEntries.size() - 1;
            }
            // Get the URL of the previous track
            audioURL = trackEntries[trackCounter].audioURL;
        }
        // If shuffle is enabled, return a random track
        else
        {
            /* Random number generation tools */
            std::random_device rand_dev; // Seed the random number generator
            std::mt19937 generator(rand_dev()); // Mersenne Twister engine for randomness
            std::uniform_int_distribution<int> dist(0, int(trackEntries.size() - 1)); // Uniform distribution over track entries

            // Generate a random track index
            int r = dist(generator);

            // Ensure the randomly selected track is not the same as the currently playing track
            while (r == trackCounter)
            {
                r = dist(generator); // Generate a new random number if the same track is selected
            }

            // Get the URL of the randomly selected track
            audioURL = trackEntries[r].audioURL;

            // Update trackCounter to keep track of the current playing song
            trackCounter = r;
        }
    }

    // Return the URL of the previous track (either sequential or random)
    return audioURL;
}

// Function to get a pointer to the trackEntries vector (for external use)
std::vector<TrackEntry>* PlaylistComponent::getTrackEntries()
{
    return &trackEntries; // Return a pointer to the vector of track entries
}

// Function to determine if the component is interested in file drag-and-drop events
// Returns true to allow file drag-and-drop functionality
bool PlaylistComponent::isInterestedInFileDrag(const juce::StringArray& files)
{
    // Accept all file types for dragging, or add additional checks for specific file formats
    return true;
}

// Function to handle file drop events (e.g., when files are dropped onto the playlist component)
void PlaylistComponent::filesDropped(const juce::StringArray& files, int x, int y)
{
    juce::String alreadyLoadedTracks; // String to store tracks that are already loaded

    // Iterate through each file dropped onto the component
    for (const auto& filePath : files)
    {
        juce::File droppedFile(filePath); // Create a File object from the file path

        // Check if the dropped file exists
        if (droppedFile.existsAsFile())
        {
            juce::URL audioURL{ droppedFile }; // Create a URL object for the dropped file

            // Check if the track is already loaded in the playlist
            if (std::any_of(trackEntries.begin(), trackEntries.end(),
                [&audioURL](const TrackEntry& entry) { return entry.audioURL == audioURL; }))
            {
                // If the track is already loaded, add its file name to the list of already loaded tracks
                alreadyLoadedTracks += audioURL.getFileName() + "\n";
            }
            else
            {
                // If the track is not already loaded, add it to the trackEntries vector
                addToTrackEntries(audioURL, deckGUI1->player->getTrackDurationWithoutLoadingIntoDeck(audioURL));
            }
        }
    }

    // If any tracks were already loaded, display a warning message to the user
    if (!alreadyLoadedTracks.isEmpty())
    {
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
            "Duplicate Tracks",
            "The following tracks were already loaded and not added again:\n" + alreadyLoadedTracks,
            "OK");
    }
}

