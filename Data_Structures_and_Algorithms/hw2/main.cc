// Main program, implements simple shell and is used to use data
// structure and different functions.

#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <fstream>

#include "datastructure.hh"

// Commands are listed below
const char READ = 'R';
const char ADD = 'A';
const char REMOVE = 'D';
const char FIND = 'F';
const char EMPTY = 'E';
const char AMOUNT = 'N';
const char PRINT = 'P';
const char QUIT = 'Q';

// Some other useful constants, like prompt and messages
const std::string PROMPT = "> ";
const std::string MSG_WORD = "Word ";
const std::string MSG_FOUND = " found.\n";
const std::string MSG_NOT_FOUND = " not found.\n";
const std::string MSG_ADDED = " added.\n";
const std::string MSG_NOT_ADDED = " not added.\n";
const std::string MSG_REMOVED = " removed.\n";
const std::string MSG_NO_FILE = "File could not be read.\n";
const std::string MSG_WORDS_IN_DS = "Amount of words in the datastructure: ";

using namespace std;

// Function readFile reads words from file and adds them to the data
// structure
bool readFile(Datastructure& ds);

int main() {
  
  char command = QUIT;
  Datastructure ds;
  string word;
  
  do {
    cout << PROMPT;
    cin >> command;
    cout << command;
    
    if(command == READ) {
      // reads file and adds words to the data structure
      readFile(ds);
    } else if(command == ADD) {
      // Adds one word and prints related messgae
      cin >> word;
      cout << SEPARATOR << word << endl;
      if(ds.add(word)) {
	cout << MSG_WORD << word << MSG_ADDED;
      } else {
	cout << MSG_WORD << word << MSG_NOT_ADDED;
      }
    } else if(command == REMOVE) {
      // Command removes a given word and prints related message
      cin >> word;
      cout << SEPARATOR << word << endl;
      if(ds.remove(word)) {
	cout << MSG_WORD << word << MSG_REMOVED;
      } else {
	cout << MSG_WORD << word << MSG_NOT_FOUND;
      }
    } else if(command == FIND) {
      // Tries to find given word and prints related message
      cin >> word;
      cout << SEPARATOR << word << endl;
      if(ds.search(word)) {
	cout << MSG_WORD << word << MSG_FOUND;
      } else {
	cout << MSG_WORD << word << MSG_NOT_FOUND;
      }
    } else if(command == EMPTY) {
      // Empties the data structure
      cout << endl;
      ds.empty();
    } else if(command == AMOUNT) {
      cout << endl << MSG_WORDS_IN_DS << ds.amountOfWords() << endl;
    } else if(command == PRINT) {
      // Prints contents of the data structure
      cout << endl;
      ds.print();
    }
    
  } while(command != QUIT);

  cout << endl;  

  return EXIT_SUCCESS;
}

// Function reads file and adds words to the datastructure.
bool readFile(Datastructure& ds) {
  
  string filename;
  cin >> ws;
  getline(cin, filename);
  cout << SEPARATOR << filename << endl;
  
  ifstream fs(filename.c_str());
  
  if(!fs) {
    std::cout << MSG_NO_FILE;
    fs.close();
    return false;
  }
  string word;
  while(fs >> word) {
    
    if(!ds.add(word)) {
      std::cout << MSG_WORD << word << MSG_NOT_ADDED;
    }
    
  }
  
  return true;
}
