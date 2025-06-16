// dictionary.cpp
// Implementation file for the Translation and Entry classes.
// Provides the detailed logic for managing translations and dictionary entries.

// Include necessary header files
#include "dictionary.h"  // Main dictionary header
#include <algorithm>     // For std::transform
#include <iostream>      // For std::cout
#include <cctype>        // For tolower()

// Platform-specific includes for getting current working directory
#ifdef _WIN32
#include <direct.h>      // Windows-specific directory functions
#define getcwd _getcwd   // Windows uses _getcwd instead of getcwd
#else
#include <unistd.h>      // Unix/POSIX directory functions
#endif

// Convert a string to lowercase
std::string toLower(const std::string& str) {
    std::string result = str;  // Create a copy of the input string
    // Transform each character to lowercase
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;  // Return the lowercase string
}

// Get the current working directory path
std::string getCurrentWorkingDirectory() {
    char cwd[1024];  // Buffer to store directory path
    // Try to get current working directory
    if (getcwd(cwd, sizeof(cwd)) != nullptr) {
        return std::string(cwd);  // Return path if successful
    } else {
        return "Unable to get current working directory.";  // Error message
    }
}

// Translation class constructor
Translation::Translation(const std::string& meanings, const std::string& language) : language(language) {
    if (meanings.empty()) return;  // Skip if no meanings provided
    std::stringstream ss(meanings);  // Create string stream from meanings
    std::string meaning;  // Temporary storage for each meaning
    // Split meanings by semicolon and add to vector
    while (std::getline(ss, meaning, ';')) {
        if (!meaning.empty()) {  // Skip empty meanings
            this->meanings.push_back(meaning);
        }
    }
    // Ensure at least one meaning exists
    if (this->meanings.empty()) {
        this->meanings.push_back("");
    }
}

// Add a new meaning to the translation
void Translation::addMeaning(const std::string& newMeaning) {
    if (newMeaning.empty()) return;  // Skip empty meanings
    // Check if meaning already exists (case-insensitive)
    for (const auto& existing : meanings) {
        if (toLower(existing) == toLower(newMeaning)) return;
    }
    meanings.push_back(newMeaning);  // Add new meaning
}

// Display the translation information
void Translation::display() const {
    std::cout << language << " : ";  // Print language
    // Print all meanings separated by semicolons
    for (size_t i = 0; i < meanings.size(); ++i) {
        std::cout << meanings[i] << (i < meanings.size() - 1 ? "; " : "");
    }
    std::cout << std::endl;  // End line
}

// Get the language of the translation
const std::string& Translation::getLanguage() const {
    return language;
}

// Get the meanings (non-const version)
std::vector<std::string>& Translation::getMeanings() {
    return meanings;
}

// Get the meanings (const version)
const std::vector<std::string>& Translation::getMeanings() const {
    return meanings;
}

// Entry class constructor
Entry::Entry(const std::string& word, const std::string& meanings, const std::string& language, const std::string& originalLine)
    : word(toLower(word)), originalWord(word), originalLine(originalLine), deleted(false) {
    // Handle empty word case
    if (word.empty()) {
        this->word = "unknown";
        this->originalWord = "unknown";
        this->originalLine = "unknown:" + meanings;
    }
    // Add initial translation
    translations.push_back(Translation(meanings, language));
}

// Add a new translation to the entry
void Entry::addTranslation(const std::string& newMeanings, const std::string& language) {
    if (language.empty() || newMeanings.empty()) return;  // Validate input
    // Check if translation for this language already exists
    for (auto& trans : translations) {
        if (toLower(trans.getLanguage()) == toLower(language)) {
            trans.addMeaning(newMeanings);  // Add meaning to existing translation
            return;
        }
    }
    // Create new translation if language doesn't exist
    translations.emplace_back(newMeanings, language);
}

// Print entry information
void Entry::print(int comparisons, const std::string& displayWord) const {
    // Print search information
    std::cout << displayWord << " found in the Dictionary after " << comparisons << " comparisons." << std::endl;
    // Display all translations
    for (const auto& trans : translations) {
        trans.display();
    }
}

// Get lowercase version of word
const std::string& Entry::getWord() const {
    return word;
}

// Get original case version of word
const std::string& Entry::getOriginalWord() const {
    return originalWord;
}

// Get original line from input file
const std::string& Entry::getOriginalLine() const {
    return originalLine;
}

// Check if entry is marked as deleted
bool Entry::isDeleted() const {
    return deleted;
}

// Mark entry as deleted
void Entry::markDeleted() {
    deleted = true;
}

// Get translations (non-const version)
std::vector<Translation>& Entry::getTranslations() {
    return translations;
}

// Get translations (const version)
const std::vector<Translation>& Entry::getTranslations() const {
    return translations;
}




