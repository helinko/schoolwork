// Definition of Datastructure class for UDS/Tiraka homework 1 -


#ifndef DATASTRUCTURE_HH
#define DATASTRUCTURE_HH

#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>

const char SEPARATOR = ' ';
const std::string MSG_EMPTY_DS = "Data structure is empty.\n";

class Datastructure {

public:

   Datastructure();
   
   ~Datastructure();
   
   // Adds one value to the datastructure
   void add(std::string rank, unsigned int birthYear, unsigned int 
	    enlistingYear, std::string shirtColor, std::string name);
   
   // Finds and prints person with index
   void find(unsigned int index);
   
   // Finds and prints youngest person
   void youngest();
   
   // Finds and prints oldest person
   void oldest();
   
   // Empties the datastructure
   void empty();
   
   // returns the size of the datastructure
   size_t size();
  
private:
  
   // Copy constructor is forbidden
   Datastructure(const Datastructure&);
   // Assignment operator is forbidden
   Datastructure& operator=(const Datastructure&);
   
   // Searches the kth smallest element by partially sorting the data
   // structure (when finished, the kth element is in c++ index k - 1).
   void quickselect(unsigned int left, unsigned int right, unsigned int k);
   
   // Prints the info on a given index
   void print(unsigned int index);
   // Exchanges the places of two elements in the vector
   void exchange(unsigned int i, unsigned int j);
   // A part of the quickselect function, partitions the array according
   // to a pivot element. After execution, all elements left to the pivot
   // have a smaller value and all on the right have a larger value than the
   // pivot. Returns the index of pivot.
   unsigned int partition(unsigned int left, unsigned int right);
   
   // The data structure is a vector, with structs as elements.
   struct Member {
      std::string rank;
      unsigned int birthYear;
      unsigned int enlistingYear;
      std::string shirtColor;
      std::string name;
   };
   std::vector<Member> members;
     
};

#endif
