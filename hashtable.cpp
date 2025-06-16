// hashtable.cpp
// Implementation file for the HashTable class.
// Provides the detailed logic for managing dictionary entries using a hash table.

// Include necessary header files
#include "hashtable.h"       // Header for HashTable class
#include "dictionary.h"      // Header for Dictionary functionality
#include <iostream>          // For std::cout and std::cerr
#include <fstream>           // For file operations (std::ofstream, std::ifstream)
#include <algorithm>         // For std::sort (though not directly used in this file)

// HashTable constructor with initial capacity
HashTable::HashTable(unsigned int capacity) : size(0), capacity(capacity), collisions(0) {
    // Validate capacity
    if (capacity <= 0) {
        capacity = 1171891; // Default capacity for large datasets
    }
    this->capacity = capacity; // Set the capacity
    buckets = new Entry*[capacity](); // Allocate array of Entry pointers
    if (!buckets) { // Check if allocation failed
        std::cerr << "Memory allocation failed for hash table!" << std::endl;
        exit(1); // Exit if allocation fails
    }
    // Initialize all buckets to nullptr
    for (unsigned int i = 0; i < static_cast<unsigned int>(capacity); ++i) {
        buckets[i] = nullptr;
    }
}

// HashTable destructor
HashTable::~HashTable() {
    // Delete all entries in the buckets
    for (unsigned int i = 0; i < capacity; ++i) {
        delete buckets[i];
    }
    delete[] buckets; // Delete the bucket array itself
}

// Get current size of hash table
unsigned int HashTable::getSize() const {
    return size;
}

// Get number of collisions that occurred
unsigned int HashTable::getCollisions() const {
    return collisions;
}

// Generate hash code for a given word
unsigned long HashTable::hashCode(const std::string& word) const {
    unsigned long hash = 0; // Initialize hash value
    std::string lowerWord = toLower(word); // Convert word to lowercase
    if (lowerWord.empty()) { // Handle empty string case
        return 0;
    }
    // Calculate hash using polynomial rolling hash function
    for (const auto& ch : lowerWord) {
        hash = (hash * 31 + ch) % capacity; // Modulo to keep within capacity
    }
    return hash;
}

// Insert a new word into the hash table
void HashTable::insert(const std::string& word, const std::string& meanings, const std::string& language,
                      const std::string& originalLine, bool silent) {
    // Validate input parameters
    if (word.empty() || meanings.empty() || language.empty()) {
        if (!silent) {
            std::cout << "Invalid input: word, meanings, and language cannot be empty." << std::endl;
        }
        return;
    }
    std::string lowerWord = toLower(word); // Convert word to lowercase
    unsigned int idx = hashCode(lowerWord); // Calculate initial hash index
    unsigned int originalIdx = idx; // Store original index for full table check
    unsigned int comparisons = 0; // Track number of comparisons made
    
    // Linear probing to find empty slot or existing word
    while (buckets[idx] != nullptr && toLower(buckets[idx]->getWord()) != lowerWord && !buckets[idx]->isDeleted()) {
        idx = (idx + 1) % capacity; // Move to next index (with wrap-around)
        ++collisions; // Increment collision count
        ++comparisons;
        // Check if table is full
        if (idx == originalIdx) {
            if (!silent) {
                std::cout << "Hash table is full after " << comparisons << " comparisons!" << std::endl;
            }
            return;
        }
    }
    

    
    // Handle empty or deleted slot
    if (buckets[idx] == nullptr || buckets[idx]->isDeleted()) {
        buckets[idx] = new Entry(word, meanings, language, originalLine);
        if (!buckets[idx]) {
            if (!silent) {
                std::cerr << "Memory allocation failed for new entry!" << std::endl;
            }
            return;
        }
        ++size;
    } else {
        buckets[idx]->addTranslation(meanings, language);
    }






}

// Find a word in the hash table
void HashTable::find(const std::string& word) const {
    if (word.empty()) { // Validate input
        std::cout << "Invalid input: word cannot be empty." << std::endl;
        return;
    }
    std::string lowerWord = toLower(word); // Convert to lowercase
    unsigned int idx = hashCode(lowerWord); // Calculate hash index
    unsigned int originalIdx = idx; // Store original index
    int comparisons = 0; // Track comparisons
    
    // Search for the word
    while (buckets[idx] != nullptr) {
        ++comparisons;
        // Check if current entry matches
        if (toLower(buckets[idx]->getWord()) == lowerWord && !buckets[idx]->isDeleted()) {
            buckets[idx]->print(comparisons, word); // Print entry info
            return;
        }
        idx = (idx + 1) % capacity; // Move to next index
        if (idx == originalIdx) break; // Check if we've looped around
    }
    std::cout << word << " not found in the Dictionary." << std::endl;
}

// Delete a word from the hash table
void HashTable::delWord(const std::string& word) {
    if (word.empty()) { // Validate input
        std::cout << "Invalid input: word cannot be empty." << std::endl;
        return;
    }
    std::string lowerWord = toLower(word); // Convert to lowercase
    unsigned int idx = hashCode(lowerWord); // Calculate hash index
    unsigned int originalIdx = idx; // Store original index
    
    // Search for the word
    while (buckets[idx] != nullptr) {
        if (toLower(buckets[idx]->getWord()) == lowerWord && !buckets[idx]->isDeleted()) {
            buckets[idx]->markDeleted(); // Mark as deleted
            --size; // Decrement size counter
            std::cout << word << " has been successfully deleted from the Dictionary." << std::endl;
            return;
        }
        idx = (idx + 1) % capacity; // Move to next index
        if (idx == originalIdx) break; // Check if we've looped around
    }
    std::cout << word << " not found in the Dictionary." << std::endl;
}

// Add a new word to the hash table (wrapper for insert)
void HashTable::addWord(const std::string& word, const std::string& meanings, const std::string& language) {
    insert(word, meanings, language, word + ":" + meanings, false);
}

// Delete a specific translation for a word
void HashTable::delTranslation(const std::string& word, const std::string& language) {
    if (word.empty() || language.empty()) { // Validate input
        std::cout << "Invalid input: word and language cannot be empty." << std::endl;
        return;
    }
    std::string lowerWord = toLower(word); // Convert to lowercase
    std::string lowerLanguage = toLower(language); // Convert to lowercase
    unsigned int idx = hashCode(lowerWord); // Calculate hash index
    unsigned int originalIdx = idx; // Store original index
    
    // Search for the word
    while (buckets[idx] != nullptr) {
        if (toLower(buckets[idx]->getWord()) == lowerWord && !buckets[idx]->isDeleted()) {
            auto& translations = buckets[idx]->getTranslations(); // Get translations
            // Search for matching language
            for (auto it = translations.begin(); it != translations.end(); ++it) {
                if (toLower(it->getLanguage()) == lowerLanguage) {
                    translations.erase(it); // Delete translation
                    std::cout << "Translation has been successfully deleted from the Dictionary." << std::endl;
                    return;
                }
            }
            std::cout << "Translation not found!" << std::endl;
            return;
        }
        idx = (idx + 1) % capacity; // Move to next index
        if (idx == originalIdx) break; // Check if we've looped around
    }
    std::cout << "Word not found!" << std::endl;
}

// Delete a specific meaning for a word in a specific language
void HashTable::delMeaning(const std::string& word, const std::string& meaning, const std::string& language) {
    if (word.empty() || meaning.empty() || language.empty()) { // Validate input
        std::cout << "Invalid input: word, meaning, and language cannot be empty." << std::endl;
        return;
    }
    std::string lowerWord = toLower(word); // Convert to lowercase
    std::string lowerLanguage = toLower(language); // Convert to lowercase
    unsigned int idx = hashCode(lowerWord); // Calculate hash index
    unsigned int originalIdx = idx; // Store original index
    
    // Search for the word
    while (buckets[idx] != nullptr) {
        if (toLower(buckets[idx]->getWord()) == lowerWord && !buckets[idx]->isDeleted()) {
            auto& translations = buckets[idx]->getTranslations(); // Get translations
            // Search for matching language
            for (auto it = translations.begin(); it != translations.end(); ++it) {
                if (toLower(it->getLanguage()) == lowerLanguage) {
                    auto& meanings = it->getMeanings(); // Get meanings
                    // Search for matching meaning
                    for (auto mit = meanings.begin(); mit != meanings.end(); ++mit) {
                        if (toLower(*mit) == toLower(meaning)) {
                            meanings.erase(mit); // Delete meaning
                            // If no meanings left, delete the translation
                            if (meanings.empty()) {
                                translations.erase(it);
                            }
                            std::cout << "Meaning has been successfully deleted from the Dictionary." << std::endl;
                            return;
                        }
                    }
                }
            }
            std::cout << "Meaning or language not found!" << std::endl;
            return;
        }
        idx = (idx + 1) % capacity; // Move to next index
        if (idx == originalIdx) break; // Check if we've looped around
    }
    std::cout << "Word not found!" << std::endl;
}

// Export data for a specific language to a file
void HashTable::exportData(const std::string& language, const std::string& filePath) const {
    if (language.empty() || filePath.empty()) { // Validate input
        std::cout << "Invalid input: language and file path cannot be empty." << std::endl;
        return;
    }
    std::string lowerLanguage = toLower(language); // Convert to lowercase
    std::ofstream outFile(filePath); // Open output file
    if (!outFile.is_open()) { // Check if file opened successfully
        std::cout << "Error opening file for export: " << filePath << std::endl;
        std::cout << "Current working directory: " << getCurrentWorkingDirectory() << std::endl;
        return;
    }
    outFile << language << std::endl; // Write language header
    unsigned int count = 0; // Track number of exported records
    
    // Iterate through all buckets
    for (unsigned int i = 0; i < capacity; ++i) {
        if (buckets[i] != nullptr && !buckets[i]->isDeleted()) {
            const std::vector<Translation>& translations = buckets[i]->getTranslations();
            // Check all translations for matching language
            for (std::vector<Translation>::const_iterator it = translations.begin(); it != translations.end(); ++it) {
                if (toLower(it->getLanguage()) == lowerLanguage) {
                    // Write word and meanings to file
                    outFile << buckets[i]->getOriginalWord() << ":";
                    const std::vector<std::string>& meanings = it->getMeanings();
                    for (size_t k = 0; k < meanings.size(); ++k) {
                        outFile << meanings[k] << (k < meanings.size() - 1 ? ";" : "");
                    }
                    outFile << std::endl;
                    count++;
                }
            }
        }
    }
    outFile.close(); // Close the file
    std::cout << count << " records have been successfully exported to " << filePath << std::endl;
}

// Import data from a file
void HashTable::import(const std::string& path) {
    if (path.empty()) { // Validate input
        std::cout << "Invalid input: file path cannot be empty." << std::endl;
        return;
    }
    std::ifstream inFile(path); // Open input file
    if (!inFile.is_open()) { // Check if file opened successfully
        std::cout << "Error opening file: " << path << std::endl;
        std::cout << "Current working directory: " << getCurrentWorkingDirectory() << std::endl;
        std::cout << "Please ensure the file exists in the current directory or provide the full path." << std::endl;
        return;
    }
    std::string language, line;
    if (!std::getline(inFile, language)) { // Read language from first line
        std::cout << "File is empty or corrupted." << std::endl;
        inFile.close();
        return;
    }
    if (language.empty()) { // Validate language
        std::cout << "Language not specified in file." << std::endl;
        inFile.close();
        return;
    }
    unsigned int count = 0; // Track number of imported words
    
    // Read each line from the file
    while (std::getline(inFile, line)) {
        if (line.empty()) continue; // Skip empty lines
        size_t colonPos = line.find(':'); // Find separator
        if (colonPos == std::string::npos) continue; // Skip invalid lines
        
        // Extract word and translation
        std::string word = line.substr(0, colonPos);
        std::string translation = (colonPos + 1 < line.length()) ? line.substr(colonPos + 1) : "";
        
        // Trim whitespace
        word.erase(0, word.find_first_not_of(" \t"));
        word.erase(word.find_last_not_of(" \t") + 1);
        translation.erase(0, translation.find_first_not_of(" \t"));
        translation.erase(translation.find_last_not_of(" \t") + 1);
        
        // Insert valid words
        if (!word.empty()) {
            insert(word, translation, language, line, true); // Silent insert
            ++count;
        }
    }
    inFile.close(); // Close the file
    std::cout << count << " " << language << " words have been imported successfully." << std::endl;
}