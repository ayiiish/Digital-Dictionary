# Welcome to My Digital Dictionary! 🔎✨📖

This C++ program is a simple Digital Dictionary that allows users to search for words' meanings and administrators to manage the dictionary by adding, deleting, and updating words and providing word suggestions.

## Features

- **Search Word:** Users can search for the meaning of a word in the dictionary.
- **Admin Login:** Administrators can log in using a predefined code ("devDictionary").
- **Add Word:** Administrators can add new words and their meanings to the dictionary.
- **Delete Word:** Administrators can delete words from the dictionary.
- **Update Word:** Administrators can update the meaning of existing words in the dictionary.
- **Word Suggestion:** Administrators can get suggestions for words based on a given prefix.

## How to Use

### Loading Screen

The program starts with a loading screen indicating the initialization of the dictionary.

### Main Menu

Users are presented with a menu allowing them to either:
1. Search for a Word
2. Admin Login
0. Exit the Dictionary

### Search for a Word (Option 1)

Users can enter a word to search for its meaning. If the word is found, its meaning is displayed; otherwise, a "Word Not Found" message is shown.

### Admin Login (Option 2)

Administrators need to enter the predefined login code ("devDictionary"). Upon successful login, administrators can perform various management operations.

### Admin Menu

After logging in as an admin, the following options are available:
1. Add Word
2. Delete Word
3. Update Word
4. Word Suggestion
0. Back

### Exiting the Dictionary (Option 0)

Users can choose to exit the dictionary program.

## Important Notes

- The dictionary is loaded from a file ("Dictionary.txt") during program initialization.
- Any changes administrators make are reflected in the "Dictionary.txt" file.
- The program uses a Trie data structure for efficient word search and suggestions.

## Compilation and Execution

The code can be compiled using a C++ compiler. Make sure to include the necessary header files and links against the Windows API library.
The program uses console colours for a visually appealing interface.

**Note:** The program might not work as expected on non-Windows systems due to the Windows API dependency.

Whether you're a language enthusiast or a meticulous administrator, this Digital Dictionary simplifies your tasks and enhances your experience.
Enjoy exploring the world of words! 🌟
