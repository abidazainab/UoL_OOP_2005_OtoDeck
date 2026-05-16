
/*
  ==============================================================================

    PlaylistFilterManager.cpp
    Created: 29 Aug 2024 9:20:00pm
    Author:  Admin

  ==============================================================================
*/

#include "PlaylistFilterManager.h"  // Include the header file for the PlaylistFilterManager class

//==============================================================================
// Constructor: Initializes the PlaylistFilterManager with a reference to the full list of TrackEntry objects.
// The constructor applies the "All" filter by default, including all tracks in the filtered list.
// @param entries A reference to the vector of TrackEntry objects representing the full playlist.
PlaylistFilterManager::PlaylistFilterManager(std::vector<TrackEntry>& entries)
    : allEntries(entries), currentFilterType(FilterType::All)  // Initialize with all tracks and default filter type
{
    filterAll();  // Start by applying the "All" filter
}

// filterAll: Filters the playlist to include all tracks.
// Copies all TrackEntry objects from the full list to the filtered list.
void PlaylistFilterManager::filterAll()
{
    filteredEntries = allEntries;  // Copy all entries to the filtered list
    currentFilterType = FilterType::All;  // Set the filter type to "All"
}

// filterFavourites: Filters the playlist to include only favourite tracks.
// Iterates through the full list and adds only the favourite tracks to the filtered list.
void PlaylistFilterManager::filterFavourites()
{
    filteredEntries.clear();  // Clear the filtered list before adding new entries
    for (const auto& entry : allEntries)
    {
        if (entry.isFavourite)  // Check if the track is marked as a favourite
        {
            filteredEntries.push_back(entry);  // Add the favourite track to the filtered list
        }
    }
    currentFilterType = FilterType::Favourites;  // Set the filter type to "Favourites"
}

// setFilteredTracks: Sets a custom list of filtered tracks provided by the user.
// Replaces the current filtered list with the provided list and marks the filter type as "Custom".
// @param foundEntries A vector of TrackEntry objects representing the custom filtered tracks.
void PlaylistFilterManager::setFilteredTracks(const std::vector<TrackEntry>& foundEntries)
{
    filteredEntries = foundEntries;  // Replace the filtered list with the provided custom list
    currentFilterType = FilterType::Custom;  // Mark the filter type as "Custom"
}

// getFilteredTracks: Returns a reference to the current list of filtered tracks.
// The list reflects the tracks that meet the criteria of the currently applied filter.
// @return A reference to the vector of TrackEntry objects representing the filtered playlist.
std::vector<TrackEntry>& PlaylistFilterManager::getFilteredTracks()
{
    return filteredEntries;  // Return the current filtered list of tracks
}
