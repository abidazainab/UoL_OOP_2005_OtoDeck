
/*
  ==============================================================================

    PlaylistFilterManager.h
    Created: 29 Aug 2024 9:20:00pm
    Author:  Admin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>           // JUCE framework header for utilities
#include "TrackEntry.h"           // Include TrackEntry class for handling track data

//==============================================================================
// PlaylistFilterManager is a class that manages the filtering of a playlist of TrackEntry objects.
// It provides methods to filter tracks based on different criteria like "All", "Favourites", or custom filters.
class PlaylistFilterManager
{
public:
    // Enum to represent the different types of filters that can be applied.
    enum class FilterType { All, Favourites, Custom };

    // Constructor: Initializes the PlaylistFilterManager with a reference to a vector of TrackEntry objects.
    // The constructor starts by applying the "All" filter to include all tracks.
    // @param entries A reference to the vector of TrackEntry objects representing the full playlist.
    PlaylistFilterManager(std::vector<TrackEntry>& entries);

    // filterAll: Filters the playlist to include all tracks.
    // Copies all entries from the full playlist to the filtered playlist.
    void filterAll();

    // filterFavourites: Filters the playlist to include only favourite tracks.
    // Iterates through the full playlist and copies only the favourite tracks to the filtered playlist.
    void filterFavourites();

    // getFilteredTracks: Returns a reference to the current list of filtered tracks.
    // @return A reference to the vector of TrackEntry objects representing the filtered playlist.
    std::vector<TrackEntry>& getFilteredTracks();

    // setFilteredTracks: Sets a custom list of filtered tracks.
    // Marks the current filter type as "Custom".
    // @param foundEntries A vector of TrackEntry objects representing the custom filtered tracks.
    void setFilteredTracks(const std::vector<TrackEntry>& foundEntries);

    // getCurrentFilterType: Returns the current filter type that is applied.
    // @return The current filter type, which could be All, Favourites, or Custom.
    FilterType getCurrentFilterType() const { return currentFilterType; }

private:
    std::vector<TrackEntry>& allEntries;        // Reference to the full list of all TrackEntry objects
    std::vector<TrackEntry> filteredEntries;    // Vector to store the currently filtered list of TrackEntry objects
    FilterType currentFilterType;               // The type of filter currently applied to the playlist
};
