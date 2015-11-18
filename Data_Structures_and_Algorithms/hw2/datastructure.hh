// Definition of Datastructure class for UDS/Tiraka homework 2 trie.
//
#ifndef DATASTRUCTURE_HH
#define DATASTRUCTURE_HH

#include <string>

// SEPARATOR is recommended to use when printing contents of the data
// structure
const char SEPARATOR = ' ';
const int ALPHABET_SIZE = 26;

class Datastructure
{

 public:

   Datastructure();

   ~Datastructure();

   // Returns true if word was added and false otherwise.
   bool add(std::string& word);

   // Returns true if word was removed and false otherwise.
   bool remove(std::string& word);

   // Returns true if word was found and false otherwise. I combined the search
   // and remove functions, so 
   bool search(std::string& word, bool remove_ = false);

   // Solves and returns amount of words in datastructure.
   unsigned int amountOfWords();

   // Empties the datastructure. 
   void empty();

   // Prints contents of the data structure.
   void print();

 private:
   // Copy constructor is forbidden
   Datastructure(const Datastructure&);
   // Assignment operator is forbidden
   Datastructure& operator=(const Datastructure&);

   // The data structure is a trie, with structs as nodes. 
   
   struct Node {
      Node* ptrs[ALPHABET_SIZE];
      bool flag;
   };
   Node* first;
   unsigned int wordcount;
   
   // Returns the index for a given character.
   unsigned int array_index(char char_);
   // Returns corresponding character for a given index.
   char character(unsigned int i);
   // Helps with the recursive implementation of empty.
   void rec_empty(Node* node_ptr);   
   // Helps with the recursive implementation of print.
   void rec_print(Node* node_ptr, std::string prefix, bool& first_word);
};

#endif
