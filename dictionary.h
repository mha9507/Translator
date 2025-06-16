// dictionary.h
// Header file defining the Translation and Entry classes for the translator program.
// Provides shared structures and utility functions.
// Header guard to prevent multiple inclusions
#ifndef DICTIONARY_H
#define DICTIONARY_H

// Include necessary standard library headers
#include <string>    // For std::string
#include <vector>    // For std::vector
#include <sstream>   // For std::stringstream

// Function declaration: Converts a string to lowercase for case-insensitive operations
std::string toLower(const std::string& str);

// Function declaration: Gets the current working directory for file operations
std::string getCurrentWorkingDirectory();

// Translation class: Represents translations for a word in a specific language
class Translation {
private:
    std::string language;               // Stores the language of the translation
    std::vector<std::string> meanings;  // Stores all meanings/translations for this language
public:
    // Constructor: Creates a new Translation with given meanings and language
    Translation(const std::string& meanings, const std::string& language);
    
    // Adds a new meaning to this translation if it doesn't already exist
    void addMeaning(const std::string& newMeaning);
    
    // Displays the translation in a readable format
    void display() const;
    
    // Returns the language of this translation (const version)
    const std::string& getLanguage() const;
    
    // Returns the meanings vector (non-const version)
    std::vector<std::string>& getMeanings();
    
    // Returns the meanings vector (const version)
    const std::vector<std::string>& getMeanings() const;
};

// Entry class: Represents a word with its translations in multiple languages
class Entry {
private:
    std::string word;                   // Lowercase version of the word for case-insensitive comparison
    std::string originalWord;           // Original case version of the word
    std::string originalLine;           // Original line from input file
    std::vector<Translation> translations;  // All translations for this word
    bool deleted;                       // Flag to mark deleted entries
public:
    // Constructor: Creates a new dictionary entry
    Entry(const std::string& word, const std::string& meanings, const std::string& language, const std::string& originalLine = "");
    
    // Adds a new translation or meaning to an existing translation
    void addTranslation(const std::string& newMeanings, const std::string& language);
    
    // Prints the entry information including comparison count
    void print(int comparisons, const std::string& displayWord) const;
    
    // Returns the lowercase version of the word
    const std::string& getWord() const;
    
    // Returns the original case version of the word
    const std::string& getOriginalWord() const;
    
    // Returns the original line from input file
    const std::string& getOriginalLine() const;
    
    // Checks if this entry is marked as deleted
    bool isDeleted() const;
    
    // Marks this entry as deleted
    void markDeleted();
    
    // Returns the translations vector (non-const version)
    std::vector<Translation>& getTranslations();
    
    // Returns the translations vector (const version)
    const std::vector<Translation>& getTranslations() const;
};

// End of header guard
#endif // DICTIONARY_H


