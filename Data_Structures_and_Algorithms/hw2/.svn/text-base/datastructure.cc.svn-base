// TIE-20106 Data Structures and Algorithms
// Autumn 2014
// Homework 2: Trie
// 
// Risto Helinko
// 211661
// risto.helinko@student.tut.fi
//  
// This is the implementation of the class data structure. The main idea is to
// save a list of words and maintain an alphabetical order efficiently.

#include <iostream>
#include <string>

using namespace std;

#include "datastructure.hh"

const unsigned int ASCII_DIFF = 97;

// 'First' is initialized as nullptr, so it can be used to recognize a 
// structure with no nodes yet. Also wordcount needs to be initialized as
// zero, so when adding words, we can just increment it. 
// 
// The first, empty node is not created here because of possible failure
// of memory allocation. Also, it's just as easy to do in add-function.

Datastructure::Datastructure(): first(nullptr), wordcount(0) 
{
}

// A function to return the corresponding index for a given character, used
// at least in search and remove.

unsigned int Datastructure::array_index(char char_)
{     
   // An index between 0 and 25 is needed for a given character - a = 0, b = 1 
   // and so forth. As the characters are only allowed to be small letters a-z,
   // the conversion is quite simple:
   // 
   // First convert the character into a number that is determined by the 
   // ASCII-table. The numbers from 97 to 122 correspond to the small alphabet,
   // so the right number is found by substracting 97 from the ASCII value.
   
   return (unsigned int)char_ - ASCII_DIFF;
}

// The opposite of the one above. Returns the character for a given index.

char Datastructure::character(unsigned int i) 
{
   // From the index, the corresponding ascii value can be calculated by
   // adding 97 to the index. That is then converted to a char variable and
   // returned.
   
   unsigned int ascii_number = i + ASCII_DIFF;
   return (char)ascii_number;
}
// Adds a word to the data structure. Returns true if added, false if not 
// (false also when the word is already contained).
bool Datastructure::add(string& word) 
{
   // If first is nullptr, there are no nodes in the data structure.
   
   if (first == nullptr) 
     {
	// Add the first node and check if memory allocation succeeds.
	try 
	  {
	     first = new Node;
	  }
      catch (...) 
	  {
	     cerr << "Memory allocation failed" << endl;
	     return false;
	  }
	// Now we have to initialize the values of the node.
      for (int i = 0; i < ALPHABET_SIZE; ++i) 
	  {
	     first->ptrs[i] = nullptr;
	  }
	first->flag = false;
     }
   
   // So now there is at least one node in the structure.
   Node* node_ptr = first;

   // This loop goes through the word character by character.
   
   for (string::size_type word_index = 0; word_index < word.length(); 
	++word_index)
     {
	char char_ = word.at(word_index);
	unsigned int i = array_index(char_);
	
	// Check whether there's a node corresponding to the character in
	// question. If not, create it.
	if (node_ptr->ptrs[i] == nullptr) 
	  {
	     try 
	       {
		  node_ptr->ptrs[i] = new Node;
	       }
	     catch (...) 
	       {
		  cerr << "Memory allocation failed" << endl;
		  return false;
	       }
	     // Initialize the variables in the new node.
	     for (unsigned int j = 0; j < ALPHABET_SIZE; ++j) 
	          node_ptr->ptrs[i]->ptrs[j] = nullptr;
	     node_ptr->ptrs[i]->flag = false;
	  }
	node_ptr = node_ptr->ptrs[i];
     }
   // Now there are nodes corresponding to every letter of the word. If the
   // flag of the last node is true, that means the word was already in the
   // structure, so it was not added -> return false.
   if (node_ptr->flag == true)
     	return false;
   else 
     {
        node_ptr->flag = true;
	++wordcount;
	return true;
     }
}
// This is a combined function to search and remove. In either case the 
// procedure is the same a side from a couple of rows in the end. The removal
// doesn't delete any nodes, it just changes the last flag to false to indicate
// non-existence of the word. This is because in this case it is likely that
// words, that have previously been in the trie, are added. Therefore it would
// be inefficient to delete them.  Also the amount of words is unlikely to
// be so large that there would be storage issues. 
// 
// If the function call comes from 'remove', then bool remove is true. 
// Otherwise (with no parameter in the call) it's false because of the
// default parameter.
bool Datastructure::search(std::string& word, bool remove_) 
{
   if (first == nullptr) 
     	return false;

   Node* node_ptr = first;
   
   // Go through the word char by char
   for (string::size_type word_index = 0; word_index < word.length(); 
	++word_index)
     {
	// Take the corresponding array index for the character in question
	unsigned int i = array_index(word.at(word_index));
	
	// If the pointer to the right node doesn't exist, the word isn't
	// in the trie.
	if (node_ptr->ptrs[i] == nullptr)
	     return false;
	node_ptr = node_ptr->ptrs[i];
     }
   
   // Now we are in the node that corresponds to the last character of the
   // word. All the nodes have existed, but we still have to check whether
   // the flag is true. If it isn't, then the word isn't in.
   
   if (node_ptr->flag == false)
        return false;
   
   // The deletion in the case that we wanted to remove the searched word.
   if (remove_ == true)
     {
	node_ptr->flag = false;
	--wordcount;
     }
   return true;
}
// As the remove is actually implemented above, this just calls search with
// the parameter 'remove' being true.
bool Datastructure::remove(string& word) 
{
   bool remove_ = true;
   return search(word, remove_);
}
// The amount of words is stored in a variable in the private interface, so
// this just returns that
unsigned int Datastructure::amountOfWords()
{
   return wordcount;
}
// A recursive function to empty the data structure. I had to divide empty 
// into two, because otherwise I couldn't have used node_ptr as parameter.
void Datastructure::rec_empty(Node* node_ptr) 
{
   // Go through the pointer array in the node.
   for (int i = 0; i < ALPHABET_SIZE; ++i) 
     {
	// When an actual pointer is encountered, it is followed to the 
	// next node. That is, the function calls itself with that next
	// node as a parameter.
	if (node_ptr->ptrs[i] != nullptr)
	     rec_empty(node_ptr->ptrs[i]);
     }
   
   // At this point we have gone through all the children and deleted them.
   // Therefore we delete the current node.
   
   delete node_ptr;
}
void Datastructure::empty() 
{
   // First we check whether there is any nodes in the trie. 
   if (first == nullptr)
        return;
   rec_empty(first);

   // Now all nodes have been deleted, so the member variables have to be
   // changed to their initial values.
   first = nullptr;
   wordcount = 0;
}
// A recursive function to print all the contents of the data structure. I had
// some issues with the parameters when I tried to do this in the public 
// interface, so I decided to divide it into two. Otherwise I couldn't find
// a way to keep track of the first and last word, as they would have 
// required a reference parameter, which can't be a default parameter too.
void Datastructure::rec_print(Node* node_ptr, string prefix,
			      bool& first_word) 
{
   
   // If on the current node the flag is true, it means the word represented
   // by the path to it is in the structure, so we print it. 
   
   // In this case the words are separated by a space, so it doesn't matter
   // that much whether an extra separator is printed after the last word.
   // On the other hand, if you wanted to change it to a comma, for example,
   // you could just change the constant and this would still work.
   
   // NOTE TO COURSE STAFF: I implemented this at first assuming that there
   // is not supposed to be a space after the last word. However, after 
   // testing I realized that I had been penalized for not having that extra
   // space. So I changed that if-statement to be a comment, and moved the 
   // printing of a separator after the prefix. I decided to leave it like
   // this, because I still think this is much more versatile.
   if (node_ptr->flag == true)
     {
	// Check if this is the first word. If it's not, print the
	// separator before the word.
//	if (first_word == false) 
	
	cout << prefix << SEPARATOR;
	
	// As the first word is now printed, first_word is switched to false.
	// It's a reference parameter, so it's also changed in any recursions 
	// that might be waiting.
	first_word = false;
     }
   
   for (unsigned int i = 0; i < ALPHABET_SIZE; ++i) 
     {  
	// For all the non-null pointers of the current node, the function 
	// calls itself, with parameters pointing to the next node and 
	// adding the character (that corresponds to the pointer) to prefix.
	if (node_ptr->ptrs[i] != nullptr) 
	  {
	     rec_print(node_ptr->ptrs[i], prefix + character(i), first_word); 
	  }
     }
}
void Datastructure::print() 
{
   // First we check if there are any nodes.  
   
   if (first == nullptr)
        return;
   
   string prefix = "";
   bool first_word = true;
   rec_print(first, prefix, first_word);
   
   // Now all the words are printed. Since there was no line change printed 
   // in main, we do it here.
   cout << endl;
}
// There already is a function to release all dynamic memory, so that is 
// called.
Datastructure::~Datastructure()
{
   empty();
}

   
