// hashtable.h
// Header file for the HashTable class, which manages dictionary entries using a hash table.
// Provides declarations for the HashTable class and its associated methods.

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "dictionary.h"

// HashTable class: Manages dictionary entries using an open-addressing hash table with linear probing.
class HashTable {
private:
    Entry** buckets;                // Array of pointers to Entry objects (the hash table).
    unsigned int size;              // Current number of non-deleted entries in the table.
    unsigned int capacity;          // Maximum number of buckets in the table.
    unsigned int collisions;        // Total number of collisions during insertion.

public:
    // Constructor: Initializes the hash table with a given capacity.
    explicit HashTable(unsigned int capacity);

    // Destructor: Cleans up dynamically allocated memory.
    ~HashTable();

    // Getter for the current size (number of non-deleted entries).
    unsigned int getSize() const;

    // Getter for the total number of collisions.
    unsigned int getCollisions() const;

    // Computes the hash code for a word using polynomial hashing.
    unsigned long hashCode(const std::string& word) const;

    // Inserts a new word or updates an existing word's translations.
    void insert(const std::string& word, const std::string& meanings, const std::string& language,
                const std::string& originalLine = "", bool silent = false);

    // Searches for a word and prints its translations if found.
    void find(const std::string& word) const;

    // Deletes a word from the dictionary (lazy deletion).
    void delWord(const std::string& word);

    // Adds a word and its translation (wrapper for insert).
    void addWord(const std::string& word, const std::string& meanings, const std::string& language);

    // Deletes all translations for a word in a specific language.
    void delTranslation(const std::string& word, const std::string& language);

    // Deletes a specific meaning for a word in a given language.
    void delMeaning(const std::string& word, const std::string& meaning, const std::string& language);

    // Exports all entries for a given language to a file in alphabetical order.
    void exportData(const std::string& language, const std::string& filePath) const;

    // Imports dictionary entries from a file.
    void import(const std::string& path);
};

#endif // HASHTABLE_H