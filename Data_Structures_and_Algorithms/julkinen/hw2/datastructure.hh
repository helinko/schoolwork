// Definition of Datastructure class for UDS/Tiraka homework 2 trie.
//
#ifndef DATASTRUCTURE_HH
# define DATASTRUCTURE_HH

# include <string>

// SEPARATOR is recommended to use when printing contents of the data
// structure
const char SEPARATOR = ' ';

class Datastructure
{

 public:

   Datastructure();

   ~Datastructure();

   // Returns true if word was added and false otherwise.
   bool add(std::string& word);

   // Returns true if word was removed and false otherwise.
   bool remove(std::string& word);

   // Returns true if word was found and false otherwise.
   bool search(std::string& word);

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

   // Students implementation comes here
   
};

#endif
