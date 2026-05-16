
/*
  ==============================================================================

    TrackEntry.cpp
    Created: 3 Aug 2024 9:02:48am
    Author: Admin

  ==============================================================================
*/

#include "TrackEntry.h"  // Include the header file for the TrackEntry class
#include <string>        // Standard library header for string manipulation
#include <cmath>         // Standard library header for mathematical functions (not used in current code)

//==============================================================================
// Constructor: Initializes the TrackEntry object with the given audio URL and duration.
// The title is extracted from the file name of the audio URL, and the track is not a favourite by default.
TrackEntry::TrackEntry(URL _audioURL, juce::String _duration)
    : audioURL(_audioURL),                                          // Initialize the audioURL member variable
    title(URL::removeEscapeChars(_audioURL.getFileName())),       // Derive the title from the URL's file name
    duration(_duration),                                          // Initialize the duration with the provided value
    isFavourite(false)                                            // Set the isFavourite flag to false by default
{
}

// toString: Returns a human-readable string representation of the TrackEntry.
// The string includes the title, duration, and whether the track is marked as a favourite.
juce::String TrackEntry::toString()
{
    return "Title: " + title + ", Duration: " + duration + ", Favourite: " + (isFavourite ? "Yes" : "No");
}

// toCSV: Returns a CSV-formatted string representation of the TrackEntry.
// The string includes the audio URL, duration, and whether the track is marked as a favourite.
juce::String TrackEntry::toCSV()
{
    return audioURL.toString(true) + "," + duration + "," + (isFavourite ? "Yes" : "No");
}

// operator==: Compares two TrackEntry objects for equality based on their audioURL and title.
// Two tracks are considered equal if they have the same URL and title, regardless of other properties.
bool TrackEntry::operator==(const TrackEntry& other) const
{
    return audioURL == other.audioURL && title == other.title;
}

