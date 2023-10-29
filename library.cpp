// Program: Library Management System
// Author: Meha Mallu
// This project is a library management system that implements various functionalities using a given database.  

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cctype>
using namespace std;

// Function prototypes for all the glorious function decomposition
void printMenu();

// This function adds an entry which is a title and an ISBN number to the library
void addEntry(vector<string>& titles, vector<string>& isbn) {
  string title;
  string ISBN;
  cout << "What is the book title? ";
  getline(cin, title);
  cout << endl;
  for (char c: title) {   // checks for commas in string
    if (c == ',') {
      cout << "The book title cannot contain commas." << endl;
      return;
    }
  }
  cout << "What is the 13-digit ISBN (with hyphens)? ";
  getline(cin, ISBN);
  cout << endl;
  // adds user input to each vector respectively 
  titles.push_back(title); 
  isbn.push_back(ISBN);
  cout << "The Following Entry Was Added" << endl;
  cout << "-----------------------------" << endl;
  cout << title << " --- " << ISBN << " --- In Library" << endl;
}

// This function displays all the titles, their respective ISBN numbers and the checkout statuses
void displayLibrary(const vector<string>& titles, const vector<string>& isbn) {
  cout << "Your Current Library" << endl;
  cout << "--------------------" << endl;
  if (titles.size() == 0) {
    cout << "The library has no books.";
    return;
  }
  for (int i=0; i < titles.size(); i++) {
    string title = titles[i];
    // checks for commas in title string and replaces the first comma with " --- " and the 2nd comma with an empty string for correct formatting purposes
    string newTitle; 
    int commaCount = 0;
    for (char c : title) {
      if (c == ',') {
        commaCount ++;
        if (commaCount == 1) {
          newTitle += " ---";
        }
        else if (commaCount == 2) {
          newTitle += "";
        }
      }
      else {
        newTitle += c;
      }
    }
    cout << newTitle << " ---" << isbn[i];
    // formatting for checkout status so if there isn't a comma in the string then the book's status is "In Library"
    if (title.find(',') == string::npos) {  
      cout << " --- In Library";
    }
    cout << endl;
  }
}

// This function clears all entries in the library and resets it to its initial status
void clearLibrary(vector<string>& titles, vector<string>& isbn) {
  titles.clear();
  isbn.clear();
  cout << "Your library is now empty." << endl;
}

// This function reads in data from a given database file and stores the information read in from the files into vectors
void loadLibrary(vector<string>& titles, vector<string>& isbn) {
  string filename;
  string line;
  cout << "What database to read from? ";
  getline(cin, filename);
  ifstream file(filename);
  if (!file) {
    cout << "Could not find the database file." << endl;
    return;
  }
  while (getline(file, line)) {
    // loops through characters in line in reverse order so that the line from the file can be separated into title, isbn and status
    int lastDigitPosition = -1;
      for (int i = line.length() - 1; i >= 0; i--) {
        if (line[i] == ' ') {
          lastDigitPosition = i;
          break;
        }
      }
      if (lastDigitPosition != -1) {
        titles.push_back(line.substr(0, lastDigitPosition));  // separates the title from line and stores in titles vector
      } 
      else {
        titles.push_back(line);
      }
    // checks status which is the last character in the line and outputs the appropriate checkout status for the character
    char status = line.back();
    if (line.empty() || !isdigit(line.back())) {  // this is accounting for the default status as in if the line is empty or if the last character is not a digit 
      status = '0';
    }
    if (status == '0') {                            
      isbn.push_back("In Library");
    }
    else if (status == '1') {
      isbn.push_back("Checked Out");
    }
    else if (status == '2') {
      isbn.push_back("On Loan");
    }
    else if (status == '3') {
      isbn.push_back("Unknown State");
    }
    else {
      isbn.push_back("Invalid State");
    }
  }
  cout << "Read in " << titles.size() << " lines from the file." << endl;
}

// This function outputs each entry to an output file 
void outputLibrary(const vector<string>& titles, const vector<string>& isbn) {
  string filename;
  getline(cin, filename);
  ofstream outputFile(filename);
  // loops through titles vector to output title and ISBN number by concatenating them 
  for (int i=0; i<titles.size(); i++) {
    string line = titles[i] + ", " + isbn[i];
    outputFile << line << ", 0" << endl;
  }
}

// This function prints the checkout information related to all the entries that were made to the library
void printStatus(const vector<string>& titles, const vector<string>& isbn) {
  // various information to keep track of
  int totalBooks = titles.size();
  int inLibrary = 0;
  int checkedOut = 0;
  int onLoan = 0;
  int unknown = 0;
  int other = 0;
  // iterates through each string in titles vector and if it matches the checkout status, the variable gets incremented for that if/else statement
  for (const string& statusString : titles) {
    if (statusString == "In Library") {
      inLibrary ++;
    }
    else if (statusString == "Checked Out") {
      checkedOut ++;
    }
    else if (statusString == "On Loan") {
      onLoan ++;
    }
    else if (statusString == "Unknown") {
      unknown ++;
    }
    else if (statusString == "Other"){
      other ++;
    }
  }
  cout << "Your Current Library's Stats" << endl;
  cout << "----------------------------" << endl;
  cout << "Total Books: " << totalBooks << endl;
  cout << "   In Library: " << inLibrary << endl;
  cout << "   Checked Out: " << checkedOut << endl;
  cout << "   On Loan: " << onLoan << endl;
  cout << "   Unknown: " << unknown << endl;
  cout << "   Other: " << other << endl;
}

// This function searches the library for a name or ISBN number and displays the results
void searchLibrary(const vector<string>& titles, const vector<string>& isbn) {
  string searchEntry;
  getline(cin, searchEntry);
  bool searchItemFound = false;
  cout << "The Following Are Your Search Results" << endl;
  cout << "-------------------------------------" << endl;
  // loops through titles vector to check that the item number that the user wants to search for is greater than 1 and if true, then outputs the title and isbn
  for (int i=0; i< titles.size(); i++) {
    if (titles[i].find(searchEntry) != string::npos || isbn[i] == searchEntry) {
      cout << titles[i] << isbn[i];
      searchItemFound = true;
    }
    cout << endl;
  }
  if (!searchItemFound) {
    cout << "No matching entries found." << endl;
  }
}

// This function removes an entry from the library if it's searched and found
void removeEntry(vector<string>& titles, vector<string>& isbn) {
  string searchItem;
  bool searchRemoved = false;
  cout << "Please enter the book title or the 13-digit ISBN (with dashes): " << endl;
  getline(cin, searchItem);
  // removes search item if it matches within the respective vectors
  for (int i=0; i<titles.size(); i++) {
     if (titles[i].find(searchItem) != string::npos) {
       cout << "The Following Entry Was Removed From The Library" << endl;
       cout << "------------------------------------------------" << endl;
       cout << titles[i] << " --- " << isbn[i] << endl;
       titles.erase(titles.begin() + i);
       isbn.erase(isbn.begin() + i);
       searchRemoved = true;
       break;
     }
  }
  if (!searchRemoved) {
    cout << "No matching entry found." << endl;
  }
}

int main()
{
    string command;
    vector<string> titles;
    vector<string> isbn;

    cout << "Welcome to the Library Management System" << endl;
    cout << "----------------------------------------" << endl;
    cout << endl;

    do 
    {
        // Output the menu and acquire a user selection
        printMenu();
        cout << endl << "Enter a command (case does not matter): ";

        // We use getline for all user input to avoid needing to handle
        // input buffer issues relating to using both >> and getline
        getline(cin, command);
        cout << endl;

        if (command == "A" || command == "a") {
          addEntry(titles, isbn); 
        }
        else if (command == "C" || command == "c") {
          clearLibrary(titles, isbn);
        }
        else if (command == "D" || command == "d") {
          displayLibrary(titles, isbn);          
        }
        else if (command == "L" || command == "l") {
          loadLibrary(titles, isbn);
        }
        else if (command == "O" || command == "o") {
          outputLibrary(titles, isbn);
        }
        else if (command == "P" || command == "p") {
          printStatus(titles, isbn);
        }
        else if (command == "R" || command == "r") {
          removeEntry(titles, isbn);
        }
        else if (command == "S" || command == "s") {
          searchLibrary(titles, isbn);
        }
        cout << endl;
    } while (!(command == "x" || command == "X"));

    return 0;
}

// Function definitions for all the function decomposition

/// print out the main menu of the management system
void printMenu() 
{
    cout << "Library Management Menu" << endl;
    cout << "-----------------------" << endl;
    cout << "A - Add Item To Library" << endl;
    cout << "C - Clear The Library Of All Entries" << endl;
    cout << "D - Display Library Entries" << endl;
    cout << "L - Load Library From File" << endl;    
    cout << "O - Output Library To File" << endl;
    cout << "P - Print Out Checkout Stats" << endl;
    cout << "R - Remove A Library Entry" << endl;      
    cout << "S - Search For A Library Entry" << endl;    
    cout << "X - Exit Program" << endl;
}