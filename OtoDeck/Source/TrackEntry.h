
/*
  ==============================================================================

    TrackEntry.h
    Created: 3 Aug 2024 9:02:48am
    Author:  Admin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h> // JUCE framework header for URL, String, and other utilities
#include <string>       // Standard library header for string manipulation

//==============================================================================
// TrackEntry is a class representing an audio track with associated metadata,
// such as its URL, title, duration, and whether it is marked as a favourite.
class TrackEntry
{
public:
    // Constructor: Initializes the TrackEntry with an audio URL and its duration.
    // The title is derived from the file name of the URL, and the track is not a favourite by default.
    TrackEntry(URL _audioURL, juce::String _duration);

    // Member variables:

    juce::URL audioURL;       // The URL of the audio file associated with this track
    juce::String title;       // The title of the track, derived from the file name of the audioURL
    juce::String duration;    // The duration of the track, typically in "mm:ss" format
    bool isFavourite;         // A flag indicating whether this track is marked as a favourite

    // toString: Returns a string representation of the TrackEntry, including the title, duration, and favourite status.
    juce::String toString();

    // toCSV: Returns a CSV-formatted string representation of the TrackEntry, including the audio URL, duration, and favourite status.
    juce::String toCSV();

    // Operator overloading:

    // operator==: Compares two TrackEntry objects for equality based on their audioURL and title.
    bool operator==(const TrackEntry& other) const;
};
