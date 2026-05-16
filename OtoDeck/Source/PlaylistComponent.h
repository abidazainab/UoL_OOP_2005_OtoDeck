
/*
  ==============================================================================

    PlaylistComponent.h
    Created: 30 Jul 2024 12:48:38pm
    Author:  Admin

  ==============================================================================
*/

#pragma once // Ensures the file is only included once during compilation to prevent redefinition errors

#include <JuceHeader.h> // Include JUCE framework headers for audio and GUI functionality
#include "TrackEntry.h" // Include the header file for TrackEntry class, which represents individual tracks
#include "CSVtrack.h" // Include the header for CSVtrack class, likely used for handling CSV-related track data
#include "DeckGUI.h" // Include the header for DeckGUI class, which controls individual decks (Deck 1, Deck 2)
#include "PlaylistFilterManager.h" // Include the header for managing playlist filtering (e.g., favourites)
#include <vector> // Include the vector header from C++ standard library for storing track data
#include <string> // Include the string header from C++ standard library for handling string data


/*
==============================================================================
*/
// The PlaylistComponent class represents the playlist UI and its functionalities
// It inherits from various JUCE components and interfaces for handling UI interactions
class PlaylistComponent : public juce::Component,
    public juce::TextEditor::Listener, // Interface to handle text editor events (e.g., search bar input)
    public juce::Button::Listener, // Interface to handle button click events (e.g., insert, filter buttons)
    public juce::TableListBoxModel, // Interface to handle table data (e.g., track list table)
    public LookAndFeel_V2, // Used for customising the look and feel of the component (e.g., buttons, sliders)
    public juce::FileDragAndDropTarget // Interface to handle file drag-and-drop events for track imports
{
public:
    // Constructor for the PlaylistComponent, takes pointers to DeckGUI1 and DeckGUI2 to manage deck interactions
    PlaylistComponent(DeckGUI* _deckGUI1, DeckGUI* _deckGUI2);

    // Destructor for cleaning up resources when PlaylistComponent is destroyed
    ~PlaylistComponent() override;

    // Override paint method to handle custom drawing of the component (e.g., background, text)
    void paint(juce::Graphics&) override;

    // Override resized method to handle dynamic resizing and repositioning of child components
    void resized() override;

    // Override buttonClicked method to handle click events for buttons (insert, filter)
    void buttonClicked(Button* button) override;

    // Override getNumRows method to return the number of rows in the table (tracks to display)
    int getNumRows() override;

    // Override paintRowBackground method to handle custom painting of the row background (e.g., colour when selected)
    void paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) override;

    // Override paintCell method to handle custom painting of the cell contents (e.g., track title, duration)
    void paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

    // Override refreshComponentForCell to update custom components in table cells (e.g., buttons, waveform display)
    Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate) override;

    /* TextEditor override function: called when the return key is pressed in the search bar (used for searching tracks) */
    void textEditorReturnKeyPressed(TextEditor& textEditor);

    /* Method to search for tracks based on user input */
    void searchForTrack(juce::String searchInput);

    /* Method to add a track to the <trackEntries> vector and update the table component with the new track */
    void addToTrackEntries(URL audioURL, juce::String duration);

    /* Method to print all TrackEntry objects in the trackEntries vector (for debugging or logging) */
    void printTrackEntries();

    /* Method to get a pointer to the trackEntries vector, used for external access */
    std::vector<TrackEntry>* getTrackEntries();

    /* Method to return the next track to play when fast-forward button is clicked in DeckGUI
       Depending on the shuffle condition, it returns the next track in sequence or a random track */
    URL getNextTrack(bool shuffle);

    /* Method to return the previous track to play when rewind button is clicked in DeckGUI
       Depending on the shuffle condition, it returns the previous track in sequence or a random track */
    URL getPreviousTrack(bool shuffle);

    // Public member variables for DeckGUI to access
    TableListBox tableComponent; // The table that displays the list of tracks
    bool trackFound = false; // Boolean flag indicating whether a track search result is displayed
    int trackCounter; // Integer counter to track the currently playing track index

    // Override method to check if file dragging is accepted, always return true to allow all file types
    bool isInterestedInFileDrag(const juce::StringArray& files);

    // Override method to handle file drop events (e.g., when files are dropped into the component)
    void filesDropped(const juce::StringArray& files, int x, int y);

    // Utility method to check if a string contains another string, ignoring case (helper for search functionality)
    bool containsIgnoreCase(const juce::String& str, const juce::String& searchString);

private:
    // Vector to store the loaded tracks (TrackEntry objects) and save them
    std::vector<TrackEntry> trackEntries;

    // Vector to temporarily store search results (TrackEntry objects)
    std::vector<TrackEntry> trackEntriesFound;

    // Filter manager object to handle playlist filtering (e.g., showing only favourite tracks)
    PlaylistFilterManager filterManager;

    // Pointers to the DeckGUI components (Deck 1 and Deck 2) created in the MainComponent
    DeckGUI* deckGUI1; // Pointer to DeckGUI1
    DeckGUI* deckGUI2; // Pointer to DeckGUI2

    // Search bar component for filtering tracks in the playlist
    TextEditor search{ "search", 0 };

    // Boolean flags to indicate whether a track is loaded into Deck 1 or Deck 2
    bool isLoadedtoDeck1 = false; // Flag for Deck 1
    bool isLoadedtoDeck2 = false; // Flag for Deck 2

    // Variables to keep track of which track is currently playing in each deck
    int trackPlaying1; // Index of the track playing in Deck 1
    int trackPlaying2; // Index of the track playing in Deck 2

    // Buttons for adding tracks, filtering tracks (All or Favourites)
    TextButton insertButton{ "Insert" }; // Button for inserting tracks into the playlist
    TextButton filterAll{ "All" }; // Button for showing all tracks
    TextButton filterFav{ "Favourite" }; // Button for showing only favourite tracks

    // File chooser component for selecting and importing tracks into the playlist
    juce::FileChooser mChooser{ "Select a file..." };

    // Audio format manager for handling different audio file formats (e.g., MP3, WAV)
    juce::AudioFormatManager formatManager;

    // Audio thumbnail cache for efficient waveform rendering of audio tracks
    juce::AudioThumbnailCache thumbnailCache{ 100 };

    // Favourite button for marking tracks as favourites
    juce::ImageButton* favouriteButton = nullptr;

    // Images for displaying a black heart (unfavourited) and red heart (favourited) for tracks
    juce::Image heartBlackImg; // Image for non-favourite track (black heart)
    juce::Image heartRedImg; // Image for favourite track (red heart)

    // Macro to prevent copying and ensure a single instance of PlaylistComponent is created
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaylistComponent)
};
