#include <Windows.h>
#include <iostream>
#include <conio.h>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
using namespace std;

void SetColor(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void Title()
{
    system("cls");
    int consoleWidth = 150;
    string title = "D I C T I O N A R Y";
    int titlePadding = (consoleWidth - title.length()) / 2, titlelen = titlePadding + title.length();
    SetColor(13);
    cout << setw(titlelen) << title << "\n";
    SetColor(7);
}
void Menu()
{
    int consoleWidth = 150;
    string op0 = "Exit Dictionary (Press 0)", op1 = "Search          (Press 1)", op2 = "Admin Login     (Press 2)"/*, op3 = "View Dictionary  (Press 3)"*/;
    string selText = "Select Option ...";
    int opPadding = (consoleWidth - op0.length()) / 2, oplen = opPadding + op0.length();
    int selTextPadding = (consoleWidth - selText.length()) / 2, selText_len = selTextPadding + selText.length();
    SetColor(11);
    cout << "\n" << setw(oplen) << op1 << "\n" << setw(oplen) << op2 << "\n" /*<< setw(oplen) << op3 << "\n"*/ << setw(oplen) << op0 << "\n";
    cout << setw(selText_len) << selText << "\n";
    SetColor(7);
}

bool isLettersOnly(const string& check)
{
    for (char letters : check)
    {
        if (!(isalpha(letters) || " "))
            return false;
    }
    return true;
}

void LoadingScreen()
{
    int consoleWidth = 150;
    string loadingtext = "LOADING DICTIONARY";
    int leftPadding = (consoleWidth - loadingtext.length()) / 2;
    char a = 178;
    int i = 0;

    do
    {
        SetColor(11);
        cout << setw(leftPadding + loadingtext.length()) << loadingtext << endl;
        SetColor(14);
        cout << setw(leftPadding + 9) << i << "%\n" << setw(48);
        SetColor(10);
        for (int x = 0; x <= i / 2; x++)
            cout << a;
        SetColor(7);
        this_thread::sleep_for(chrono::milliseconds(150));
        i++;
        if (i != 101)
            system("cls");
    } while (i != 101);
}

class TrieNode
{
public:
    char data;
    string meaning;
    TrieNode* children[26]; // assuming lowercase letter only
};

class DictionaryManager
{
    TrieNode* root;
public:

    DictionaryManager() : root(new TrieNode{}) {}

    void InsertWord(const string& word, const string& meaning)
    {
        TrieNode* current = root;
        for (char c : word)
        {
            if ('a' <= c && c <= 'z')
            {
                if (current->children[c - 'a'] == nullptr)
                    current->children[c - 'a'] = new TrieNode{ c, "", {nullptr} };
                current = current->children[c - 'a'];
            }
            else
            {
                cerr << "Invalid character in word: " << c << endl;
                return;
            }
        }
        current->meaning = meaning;
    }

    void LoadDictionary(const string& fileName)
    {
        ifstream file(fileName);
        if (!file.is_open())
        {
            cout << "< Error Opening " << fileName << " File >\n";
            return;
        }
        string line;
        while (getline(file, line))
        {
            // Split the line into word and meaning
            size_t pos = line.find("\t");
            if (pos != string::npos)
            {
                string word = line.substr(0, pos);
                string meaning = line.substr(pos + 1);
                InsertWord(word, meaning);
            }
        }
        cout << "< Dictionary Successfully Loaded >\n";
        file.close();
    }

    void SaveDictionaryToFile(const string& fileName)
    {
        ofstream file(fileName);
        if (!file.is_open())
        {
            cout << "< Error >\n";
            return;
        }
        SavingDictionary(root, file);
        file.close();
        cout << "< Dictionary Saved >\n";
    }

    void SavingDictionary(TrieNode* node, ofstream& file)
    {
        if (node == nullptr)
            return;
        if (!node->meaning.empty())
            file << node->data << " " << node->meaning << endl;
        for (int i = 0; i < 26; i++)
            if (node->children[i] != nullptr)
                SavingDictionary(node->children[i], file);
    }

    void AddWordToFile(const string& word, const string& meaning)
    {
        string fileName = "Dictionary.txt";
        ifstream inputFile(fileName);
        vector<pair<string, string>> wordsAndMeanings;
        if (inputFile.is_open())
        {
            string line;
            bool wordAdded = false;
            while (getline(inputFile, line))
            {
                size_t pos = line.find("\t");
                if (pos != string::npos)
                {
                    string currentWord = line.substr(0, pos);
                    if (word < currentWord && !wordAdded)
                    {
                        wordsAndMeanings.push_back({ word, meaning });
                        wordAdded = true;
                    }
                    wordsAndMeanings.push_back({ currentWord, line.substr(pos + 1) });
                }
            }
            if (!wordAdded)
                wordsAndMeanings.push_back({ word, meaning });
            inputFile.close();
        }
        ofstream outputFile(fileName);
        for (const auto& pair : wordsAndMeanings)
            outputFile << pair.first << "\t" << pair.second << endl;
        cout << "< Word Added >\n";
        outputFile.close();
    }

    TrieNode* Searching(TrieNode* node, const string& word, size_t index)
    {
        if (node == nullptr)
            return nullptr;
        if (index == word.length())
            return node;
        char currentChar = word[index];
        if (node->children[currentChar - 'a'] != nullptr)
            return Searching(node->children[currentChar - 'a'], word, index + 1);
        return nullptr;
    }
    bool SearchWord(const string& word)
    {
        TrieNode* node = Searching(root, word, 0);
        return (node != nullptr && !node->meaning.empty());
    }
    string search_word(const string& word)
    {
        TrieNode* node = Searching(root, word, 0);
        return (node != nullptr && !node->meaning.empty()) ? node->meaning : "";
    }

    void WordDeletion(TrieNode* node, const string& word, size_t index, ofstream& outputFile)
    {
        if (node == nullptr)
            return;
        if (index == word.length())
        {
            node->meaning.clear();
            return;
        }
        char currentChar = word[index];
        if (node->children[currentChar - 'a'] != nullptr)
            WordDeletion(node->children[currentChar - 'a'], word, index + 1, outputFile);
    }

    void DeleteWord(const string& word)
    {
        ifstream inputFile("Dictionary.txt");
        if (!inputFile.is_open())
        {
            cout << "Error: Unable to open file for reading.\n";
            return;
        }
        ofstream tempFile("temp.txt");
        if (!tempFile.is_open())
        {
            cout << "Error: Unable to open temporary file for writing.\n";
            inputFile.close();
            return;
        }
        string currentWord, currentMeaning;
        while (inputFile >> currentWord >> currentMeaning)
        {
            if (currentWord != word)
                tempFile << currentWord << "\t" << currentMeaning << endl;
        }
        inputFile.close();
        tempFile.close();
        remove("Dictionary.txt");
        if (rename("temp.txt", "Dictionary.txt") != 0)
        {
            cout << "Error: Unable to rename temporary file.\n";
            return;
        }
        cout << "< Word Deleted >\n";
    }

    void UpdateWord(const string& word, const string& newMeaning)
    {
        ifstream inputFile("Dictionary.txt");
        if (!inputFile.is_open())
        {
            cout << "Error: Unable to open file for reading.\n";
            return;
        }
        ofstream tempFile("temp.txt");
        if (!tempFile.is_open())
        {
            cout << "Error: Unable to open temporary file for writing.\n";
            inputFile.close();
            return;
        }
        string currentWord, currentMeaning;
        while (inputFile >> currentWord >> currentMeaning)
        {
            if (currentWord == word)
                tempFile << currentWord << " " << newMeaning << endl;
            else
                tempFile << currentWord << " " << currentMeaning << endl;
        }
        inputFile.close();
        tempFile.close();
        remove("Dictionary.txt");
        if (rename("temp.txt", "Dictionary.txt") != 0)
        {
            cout << "Error: Unable to rename temporary file.\n";
            return;
        }
        cout << "< Word Updated Successfully >\n";
    }

    void SuggestWords(TrieNode* node, const string& currPrefix)
    {
        if (node == nullptr)
            return;
        if (!node->meaning.empty())
            cout << currPrefix << " ";
        for (int i = 0; i < 26; i++)
        {
            if (node->children[i] != nullptr)
            {
                char nextLetter = 'a' + i;
                SuggestWords(node->children[i], currPrefix + nextLetter);
            }
        }
    }

    void WordSuggestion(const string& prefix)
    {
        TrieNode* node = Searching(root, prefix, 0);
        bool trieSuggestions = false;
        if (node != nullptr)
        {
            cout << "Suggestions (Trie): ";
            SuggestWords(node, prefix);
            trieSuggestions = true;
        }
        ifstream inputFile("Dictionary.txt");
        if (inputFile.is_open())
        {
            string currentWord, currentMeaning;
            while (inputFile >> currentWord >> currentMeaning)
            {
                if (currentWord.find(prefix) == 0 && currentWord != prefix)
                {
                    if (!trieSuggestions)
                        cout << "Suggestions: ";
                    cout << currentWord << " ";
                }
            }
            inputFile.close();
            if (trieSuggestions)
                cout << endl;
        }
        else
            cout << "Error: Unable to open file for reading.\n";
        if (!trieSuggestions)
            cout << "< No suggestions >\n";
    }
};

int main()
{
    DictionaryManager dictionary;
    LoadingScreen();
    cout << endl;
    dictionary.LoadDictionary("Dictionary.txt");
    string searchWord, managing_code, word, meaning, prefix;
    int consoleWidth = 150;
    char select;
    system("cls");
    while (true)
    {
        Title();
        Menu();
        select = _getch();
        switch (select)
        {
        case '1':
            Title();
            while (true)
            {
                cout << "Search Word (or enter '0' to exit search): ";
                cin >> searchWord;
                if (searchWord == "0")
                    break;
                bool wordExists = dictionary.SearchWord(searchWord);
                if (wordExists)
                {
                    SetColor(10);
                    string meaning = dictionary.search_word(searchWord);
                    cout << "Meaning: " << meaning << endl;
                    SetColor(7);
                }
                else {
                    SetColor(12);
                    cout << "< Word Not Found >\n" << endl;
                    SetColor(7);
                }
            }
            break;
        case '2':
            Title();
            cout << "\n" << setw(consoleWidth / 2) << "Enter Manage Login Code: ";
            cin >> managing_code;
            if (managing_code != "devDictionary")
            {
                SetColor(12);
                cout << setw(consoleWidth / 2) << "Invalid Input!\n";
                cout << setw(consoleWidth / 2) << "Press 0 To Go Back\n";
                SetColor(7);
                select = _getch();
                if (select == '0')
                    main();
                else
                    break;
            }
            while (true)
            {
                Title();
                cout << "";
                cout << "\n1. Add Word\n2. Delete Word\n3. Update Word\n4. Word Suggestion\n0. Back\n";
                select = _getch();
                switch (select)
                {
                case '1':
                    Title();
                    cout << "Add Word: ";
                    cin >> word;
                    cout << "Enter Meaning: ";
                    cin >> meaning;
                    dictionary.InsertWord(word, meaning);
                    dictionary.AddWordToFile(word, meaning);
                    break;
                case '2':
                    Title();
                    cout << "Delete Word: ";
                    cin >> word;
                    if (!word.empty())
                        dictionary.DeleteWord(word);
                    else
                    {
                        SetColor(12);
                        cout << "< Invalid Input: Word cannot be empty. >\n";
                        SetColor(7);
                    }
                    break;
                case '3':
                    Title();
                    cout << "Enter Word: ";
                    cin >> word;
                    cout << "Updated Meaning: ";
                    cin >> meaning;
                    dictionary.UpdateWord(word, meaning);
                    break;
                case '4':
                    Title();
                    SetColor(10);
                    cout << "Enter prefix for word suggestion: ";
                    cin >> prefix;
                    SetColor(11);
                    dictionary.WordSuggestion(prefix);
                    SetColor(7);
                    cout << "\n\nPress Any Key to Go Back To Admin Menu...";
                    select = _getch();
                    break;
                case '0':
                    main();
                default:
                    break;
                }
            }
            break;
        case '0':
            return 0;
        default:
            break;
        }
    }
    return 0;
}