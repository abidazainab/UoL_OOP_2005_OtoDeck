
/*
  ==============================================================================

    CSVtrack.cpp
    Created: 3 Aug 2024 9:02:21am
    Author:  Admin

  ==============================================================================
*/

#include "CSVtrack.h"          // Include the header file for the CSVtrack class
#include <iostream>            // Standard library header for input/output operations
#include <fstream>             // Standard library header for file stream operations

//==============================================================================
// Constructor: Initializes a CSVtrack object.
// Currently, this constructor does not perform any operations but is defined for completeness.
CSVtrack::CSVtrack()
{
}

/* reading from file to retrieve TrackEntry objects based on CSV strings in file */
// readCSV: Reads the contents of a CSV file and parses it into a vector of TrackEntry objects.
// Takes a pointer to a JUCE File object representing the CSV file.
// Returns a vector of TrackEntry objects parsed from the file.
std::vector<TrackEntry> CSVtrack::readCSV(File* resourceFile)
{
    // Initialize a vector to store the TrackEntry objects to be returned
    std::vector<TrackEntry> entries;

    // Check if the file exists, and output an error message if it does not
    if (!resourceFile->existsAsFile())
        std::cout << "File does not exist" << std::endl;

    // Create a FileInputStream to read the file's contents
    // The input stream is created by dereferencing the resourceFile pointer
    FileInputStream input(*resourceFile);

    // Check if the input stream was opened successfully, and output an error message if not
    if (!input.openedOk())
        std::cout << "Error getting input" << std::endl;
    else
    {
        // Read the file line by line until the end is reached
        while (!input.isExhausted())
        {
            try
            {
                // Read the next line from the file
                juce::String line = input.readNextLine(); // '%20' represents spaces in URLs

                // Convert the CSV line to a TrackEntry object
                // The line is tokenized, and special characters are removed
                TrackEntry entry = stringsToTrackEntry(tokenise(URL::removeEscapeChars(line), ','));

                // Add the TrackEntry object to the vector
                entries.push_back(entry);
            }
            catch (const std::exception& e)
            {
                // Handle any exceptions that occur during parsing, typically due to malformed CSV data
                std::cout << "CSVtrack::readCSV bad data" << std::endl;
            }
        }
    }

    // Output the number of tracks read from the CSV file
    std::cout << "CSVtrack::readCSV read " << entries.size() << " tracks." << std::endl;

    // Return the vector of TrackEntry objects
    return entries;
}

/* for save function: write <TrackEntry> data to existing file, if not creates new file to store <TrackEntry> data as CSV strings */
// writeCSV: Writes a vector of TrackEntry objects to a CSV file.
// Takes a pointer to a JUCE File object representing the CSV file and a pointer to a vector of TrackEntry objects.
// If the file does not exist, it creates a new file.
void CSVtrack::writeCSV(File* resourceFile, std::vector<TrackEntry>* trackEntries)
{
    // Check if the file exists, and output an error message if it does not
    if (!resourceFile->existsAsFile())
        std::cout << "File does not exist" << std::endl;

    // Create a TemporaryFile to write data, which ensures safe writing by avoiding partial writes
    TemporaryFile tempFile(*resourceFile);

    // Create a FileOutputStream to write the data to the file
    FileOutputStream output(tempFile.getFile());

    // Check if the output stream was opened successfully, and output an error message if not
    if (!output.openedOk())
        std::cout << "Error getting output" << std::endl;
    else
    {
        // Reset the output stream position to the beginning of the file to prevent appending data
        output.setPosition(0);
        output.truncate(); // Truncate the file to remove any existing data

        // Iterate over the TrackEntry vector and write each entry as a CSV line
        for (int i = 0; i < trackEntries->size(); i++)
        {
            // Convert each TrackEntry object to a CSV-formatted string
            juce::String entry = (*trackEntries)[i].toCSV();

            // Write the CSV string to the file
            output.writeText(entry + "\n", false, false, nullptr);

            // Flush the output stream to ensure data is written to disk
            output.flush();
        }

        // Replace the original file with the temporary file
        bool success = tempFile.overwriteTargetFileWithTemporary();

        // Output a message indicating whether the file write was successful
        if (success)
        {
            std::cout << "File write was successful." << std::endl;
        }
    }
}

/* reads line in file one by one and retrieve available tokens for TrackEntry objects */
// tokenise: Splits a CSV line into tokens based on the separator character.
// Takes a JUCE String representing a CSV line and a character separator.
// Returns a vector of JUCE Strings representing the tokens.
std::vector<juce::String> CSVtrack::tokenise(juce::String csvLine, char separator)
{
    // Initialize a vector to store the tokens to be returned
    std::vector<juce::String> tokens;
    signed int start, end;
    juce::String token;

    // Convert the JUCE String to a standard string for easier manipulation with standard library functions
    std::string csvLineStdString = csvLine.toStdString(); // std::string

    // Set the start point for tokenization, ignoring leading separator characters
    start = int(csvLineStdString.find_first_not_of(separator, 0)); // call std::string functions

    do
    {
        // Find the end of the current token by locating the next separator character
        end = int(csvLineStdString.find_first_of(separator, start)); // call std::string functions

        // If the start point is at the end of the line, break out of the loop
        if (start == csvLine.length() || start == end)
            break;

        // Extract the token from the line
        if (end >= 0)
            token = csvLineStdString.substr(start, end - start); // std::string to juce::String
        else
            token = csvLineStdString.substr(start, csvLine.length() - start); // std::string to juce::String

        // Add the extracted token to the vector
        tokens.push_back(token); // juce::String

        // Update the start point for the next iteration
        start = end + 1;

    } while (end > 0);

    // Return the vector of tokens
    return tokens;
}

/* reading line from csv file -> TrackEntry object */
// stringsToTrackEntry: Converts a vector of JUCE Strings into a TrackEntry object.
// Takes a vector of JUCE Strings representing the CSV tokens.
// Returns a TrackEntry object constructed from the tokens.
TrackEntry CSVtrack::stringsToTrackEntry(std::vector<juce::String> tokens)
{
    URL audioURL;
    juce::String duration;

    // If the number of tokens is not exactly 2 (URL and duration), throw an exception
    if (tokens.size() != 2)
        throw std::exception{};

    // Parse the tokens to create a TrackEntry object
    try
    {
        audioURL = URL{ tokens[0] }; // Convert the first token to a URL
        duration = tokens[1];        // Assign the second token as the duration
    }
    catch (const std::exception& e)
    {
        // Handle any exceptions that occur during parsing
        std::cout << "CSVtrack::stringsToTE Bad URL " << tokens[0] << std::endl;
        std::cout << "CSVtrack::stringsToTE Bad double " << tokens[1] << std::endl;
        throw; // Re-throw the exception to be handled by the calling function
    }

    // Return a new TrackEntry object constructed from the parsed URL and duration
    TrackEntry entry{ audioURL, duration };
    return entry;
}

