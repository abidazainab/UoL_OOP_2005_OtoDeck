
/*
  ==============================================================================

    CSVtrack.h
    Created: 3 Aug 2024 9:02:21am
    Author:  Admin

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"        // JUCE framework header for file handling, strings, and other utilities
#include "TrackEntry.h"        // Include TrackEntry class for handling track data
#include <string>              // Standard library header for string manipulation
#include <vector>              // Standard library header for using vectors

//==============================================================================
// CSVtrack is a utility class for handling the reading and writing of TrackEntry objects
// to and from CSV (Comma-Separated Values) files. It provides methods to read from and
// write to CSV files, as well as to tokenize CSV lines into TrackEntry components.
class CSVtrack
{
public:
    // Constructor: Initializes a CSVtrack object.
    CSVtrack();

    // readCSV: Reads a CSV file to retrieve TrackEntry objects.
    // Takes a pointer to a JUCE File object representing the CSV file.
    // Returns a vector of TrackEntry objects parsed from the CSV file.
    static std::vector<TrackEntry> readCSV(File* resourceFile);

    // writeCSV: Writes a vector of TrackEntry objects to a CSV file.
    // Takes a pointer to a JUCE File object representing the CSV file and a pointer to a vector of TrackEntry objects.
    // If the file does not exist, it creates a new file.
    static void writeCSV(File* resourceFile, std::vector<TrackEntry>* trackEntries);

    // tokenise: Splits a CSV line into tokens based on the separator character.
    // Takes a JUCE String representing a CSV line and a character separator.
    // Returns a vector of JUCE Strings representing the tokens.
    static std::vector<juce::String> tokenise(juce::String csvLine, char separator);

private:
    // stringsToTrackEntry: Converts a vector of JUCE Strings into a TrackEntry object.
    // Takes a vector of JUCE Strings representing the CSV tokens.
    // Returns a TrackEntry object constructed from the tokens.
    static TrackEntry stringsToTrackEntry(std::vector<juce::String> strings);
};
