// TIE-20106 Data Structures and Algorithms
// Autumn 2014
// Homework 1: Red Shirt Hypothesis
// 
// Risto Helinko
// 211661
// 
// datastructure.cc
// Implementation of the class 'Datastructure'
// 
// The data structure is vector, which has structs as elements. The search
// of kth element is carried out by a quickselect algorithm, which does
// partially sorts the structure to find out the right element. 
// 
// It's related to quicksort, but after finding right place for the pivot, it 
// doesn't branch. At that point we can rule out one of two subarrays parted
// by the pivot.  
// 
// Public interface:
// 
// void add(std::string rank, unsigned int birthYear,
//          unsigned int enlistingYear, std::string shirtColor,
//          std::string name);
// void find(unsigned int index);
// void youngest();
// void oldest();
// void empty();
// size_t size();
// 
// Private interface (copy constructor and assignment operator are 
// forbidden):
// 
// void quickselect(unsigned int left, unsigned int right, unsigned int k);
// void print(unsigned int index);
// void exchange(unsigned int i, unsigned int j);
// unsigned int partition(unsigned int left, unsigned int right);
// 
// struct Member {
//    std::string rank;
//    unsigned int birthYear;
//    unsigned int enlistingYear;
//    std::string shirtColor;
//    std::string name;
// };
// std::vector<Member> members;
//                                                 
//                                                 



#include <string>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <cstdio>
#include <ctime>

using namespace std;

#include "datastructure.hh"

const int FIRST_ELEMENT = 0;

Datastructure::Datastructure() {
}

// A function to switch the places of two elements in indices i and j.
 
void Datastructure::exchange(unsigned int i, unsigned int j) {
   // Save the first one in a variable.
   
   Member member = members.at(i);
   
   // Take the info from the second element, and place it in the place of 
   // the first one.

   members.at(i) = members.at(j);
   
   // Add the saved info from the first position to the second.
   members.at(j) = member;
}
// This is used by the quickselect function, and is practically the same
// as in quicksort. It partitions the array according to a pivot, sorting
// the smaller elements to the left and larger to the right of the pivot.
unsigned int Datastructure::partition(unsigned int left, unsigned int right) {
   
   // I went with using a randomized pivot to increase the average efficiency.
   // The random number generator is initialized with a seed, which
   // needs to be different for different executions, as the same seed
   // produces the same sequence of numbers. For that I used the 
   // time-function, as in here: 
   // http://www.cplusplus.com/reference/cstdlib/srand/
   
   // Return value of time is in 'long' and needs to be converted to 
   // unsigned int for the srand function.
   
   srand(static_cast<unsigned int> (time(NULL)));
   
   // The random number is a regular integer even though it's always positive.
   // I needed it to be compatible with all index variables, so I converted
   // it to an unsigned int.
   
   // To scale the number I used a technique from
   // http://www.cplusplus.com/reference/cstdlib/rand/
   // in which a random number is scaled to a certain range by modulo
   // operation.

   unsigned int pivot = (static_cast<unsigned int> (rand())
			 % (right - left)) + left;
   
   // Switch the chosen pivot element with the last element, so I don't
   // have to figure out its place at any given loop.
   
   exchange(pivot, right);
   
   // Now we can proceed similarly to the implementation in the lecture notes.
   // First, take the values from pivot, which is now in the index 'right'.
   
   unsigned int pivot_enl = members.at(right).enlistingYear;
   unsigned int pivot_birth = members.at(right).birthYear;

   unsigned int i = left - 1;
   
   // Go through all the elements in the array, comparing them to pivot.
   
   for ( unsigned int j = left; j <= (right - 1); ++j ) {

      unsigned int comp_enl = members.at(j).enlistingYear;
      unsigned int comp_birth = members.at(j).birthYear;
      
      // The smallest element is the most senior crew member (that is, 
      // their year of enlistment/birth is the smallest).
      
      // I added the secondary sorting criterion to the same 
      // if-statement to make it more concise.
      
      if ( comp_enl < pivot_enl or ( comp_enl == pivot_enl and 
				     comp_birth < pivot_birth ) ) {
	 
	 ++i;
	 exchange(i,j);
      }
      
   }
   exchange(i + 1, right);
   return i + 1;
}
// Quickselect is a related algorithm to quicksort, with the exception that
// it recurses only to one subarray. That leads to an average time complexity
// of O(n).
void Datastructure::quickselect(unsigned int left, unsigned int right,
				unsigned int k) {
   
   // k is the index from 0 to n - 1.
   
   // A (sub)array of 1 element is 'sorted', with the kth element in its right
   // place.
   
   if (left >= right)
      return;
   
   // After partition, everything left of index q is smaller (more senior)
   // than the element in q, and everything to the right is bigger. 
   
   unsigned int q = partition(left, right);
   
   // If the chosen pivot was in kth place after partition, we are done.
   
   if ( q == k) 
      return;
   
   // If after partition the pivot's index is larger than k, the kth element
   // is found from the left subarray, and vice versa.
   
   else if ( q > k )      
      quickselect(left, q - 1, k);
   
   else if ( q < k ) 
      quickselect(q + 1, right, k);
}
// Just a straight-forward function to collect the variables in a struct, and
// adding that in the data structure.
void Datastructure::add(std::string rank, unsigned int birthYear,
			unsigned int enlistingYear, std::string shirtColor,
			std::string name) {
   
   // Create a Member-type variable and add the info from parameters to it.
   
   Member member;
   member.rank = rank;
   member.birthYear = birthYear;
   member.enlistingYear = enlistingYear;
   member.shirtColor = shirtColor;
   member.name = name;
   
   // Then just add that member to the vector.

   members.push_back(member);
}
// Function to print the info in a given index.
void Datastructure::print(unsigned int index) {
   
   Member member = members.at(index);
   
   cout << member.name << " " << member.rank << " " << member.birthYear
        << " " << member.enlistingYear << " " << member.shirtColor << endl;
}
// A function for the public interface, not used in member functions.
size_t Datastructure::size() {
     return members.size();
}
// Finds a crew member according to a index, that is, how senior they are
// compared to others, index 1 being the most senior.
void Datastructure::find(unsigned int index) {
   
   // I had two options with this implementation. I could either have the 
   // quickselect function access the data structure directly, or I could
   // copy it in a variable here, and use it as a reference parameter.
   // I know that I should avoid copying large data structures, but on the
   // other hand, if I do quickselect on the actual data structure, it will
   // partially sort it, which can compromise its efficiency. I decided to
   // go with the latter.
   
   // Call the quickselect function, which rearranges the vector such that
   // kth smallest element is in index k - 1 (k is from 1 to n).
   // In this case smallest is the most senior crew member. 

   // There was a problem using the size function of this class, and I
   // couldn't figure out why. So I just used the STL-function directly.
   
   unsigned int size = static_cast<unsigned int> (members.size());
   
   // There was no requirements for error messages, so for an illegal index
   // the function just does nothing.
   
   if ( index > size ) 
      return;
   
   unsigned int last_element = size - 1;
   
   quickselect(FIRST_ELEMENT, last_element, index - 1);

   // So now the vector is partially sorted, and its kth element in place.
   
   print(index - 1);
}
// Youngest and oldest members are searched just like any other, the indices
// being largest and smallest respectively. I was thinking of doing this
// in a kind of brute force manner, as it seemed to guarantee linear 
// time complexity. It turned out in the testing, though, that in practise
// they were quite efficient as they were, so I ended up leaving them like
// this.
void Datastructure::youngest() {
   
   if (members.empty())
      return;
   
   unsigned int size = static_cast<unsigned int> (members.size());
   unsigned int last_element = size - 1;
   
   quickselect(FIRST_ELEMENT, last_element, last_element);
   print(last_element);
}
void Datastructure::oldest() {
   
   if (members.empty())
     return;
   
   unsigned int size = static_cast<unsigned int> (members.size());
   unsigned int last_element = size - 1;
   
   quickselect(FIRST_ELEMENT, last_element, FIRST_ELEMENT);
   print(FIRST_ELEMENT);
}
// I didn't get full points for emptying the data structure for some reason.
// I couldn't find a way to do it in sublinear time, which I assume was the
// problem. 

void Datastructure::empty() {
   members.clear();
}
// There is no need for defining a destructor as this isn't a dynamic data
// structure.
Datastructure::~Datastructure() {
}
   










