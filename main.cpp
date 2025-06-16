#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include "hashtable.h"

void help() {
    std::cout << "find <word>                         : Search a word and its meanings in the dictionary." << std::endl;
    std::cout << "import <path>                       : Import a dictionary file." << std::endl;
    std::cout << "add <word:meaning(s):language>      : Add a word and/or its meanings (separated by ;) to the dictionary." << std::endl;
    std::cout << "delTranslation <word:language>      : Delete a specific translation of a word from the dictionary." << std::endl;
    std::cout << "delMeaning <word:meaning:language>  : Delete only a specific meaning of a word from the dictionary." << std::endl;
    std::cout << "delWord <word>                      : Delete a word and its all translations from the dictionary." << std::endl;
    std::cout << "export <language:filename>          : Export a given language dictionary to a file." << std::endl;
    std::cout << "exit                                : Exit the program" << std::endl;
}

int main(int argc, char** args) {
    HashTable myHashTable(1171891); // Initialize hash table with default capacity.
    myHashTable.import("en-de.txt"); // Import the dictionary file
    std::cout << "===================================================" << std::endl;
    std::cout << "Size of HashTable                = " << myHashTable.getSize() << std::endl;
    std::cout << "Total Number of Collisions       = " << myHashTable.getCollisions() << std::endl;
    std::cout << "Avg. Number of Collisions/Entry  = " << std::fixed << std::setprecision(2)
        << (myHashTable.getSize() ? static_cast<float>(myHashTable.getCollisions()) / myHashTable.getSize() : 0) << std::endl;
    std::cout << "===================================================" << std::endl;

    help();

    std::string user_input, command, argument1, argument2, argument3;
    while (true) {
        user_input = command = argument1 = argument2 = argument3 = ""; // Clear old values
        std::cout << ">";
        if (!std::getline(std::cin, user_input)) {
            std::cout << "Error reading input. Exiting..." << std::endl;
            break;
        }
        if (user_input.empty()) continue;

        std::stringstream sstr(user_input);
        if (!std::getline(sstr, command, ' ')) {
            std::cout << "Invalid command format!" << std::endl;
            continue;
        }

        if (command.empty()) continue;
        std::transform(command.begin(), command.end(), command.begin(), ::tolower);

        if (command == "find") {
            std::getline(sstr, argument1);
            myHashTable.find(argument1);
        }
        else if (command == "import") {
            std::getline(sstr, argument1);
            myHashTable.import(argument1);
        }
        else if (command == "add") {
            std::getline(sstr, argument1, ':');
            std::getline(sstr, argument2, ':');
            std::getline(sstr, argument3);
            myHashTable.addWord(argument1, argument2, argument3);
        }
        else if (command == "delword") {
            std::getline(sstr, argument1);
            myHashTable.delWord(argument1);
        }
        else if (command == "deltranslation") {
            std::getline(sstr, argument1, ':');
            std::getline(sstr, argument2);
            myHashTable.delTranslation(argument1, argument2);
        }
        else if (command == "delmeaning") {
            std::getline(sstr, argument1, ':');
            std::getline(sstr, argument2, ':');
            std::getline(sstr, argument3);
            myHashTable.delMeaning(argument1, argument2, argument3);
        }
        else if (command == "export") {
            std::getline(sstr, argument1, ':');
            std::getline(sstr, argument2);
            myHashTable.exportData(argument1, argument2);
        }
        else if (command == "exit") {
            break;
        }
        else {
            std::cout << "Invalid command!" << std::endl;
        }
        std::cout << std::flush;
    }
    return 0;
}
