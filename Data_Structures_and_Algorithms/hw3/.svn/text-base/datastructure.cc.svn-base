// TIE-20106 Data Structures and Algorithms
// Autumn 2014
// Homework 3: Family tree
//
// Risto Helinko
// 211661
// risto.helinko@student.tut.fi
// 
// A family tree is saved as a graph. The user can add people and spousal,
// maternal and paternal relationships between them. The tree can then
// be searched for a blood relation between two people. A family tree 
// starting from a person can also be printed.

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <iterator>
#include <stack>

using namespace std;

#include "datastructure.hh"

Datastructure::Datastructure() {
}
// A straight-forward function to add the info in the data structure.
bool Datastructure::add(Gender gender, string& name, int ID, int born,
			int died) {
   
   // Check the uniqueness of the ID. If count is true, the ID already
   // exists
   
   if (  people.count(ID) ) {
      return false;
   }
	
   // First create the struct that represents the new person.
    
   Person* person_ptr = nullptr;
   
   try  {
      person_ptr = new Person;
   }
   catch (...) {
      cerr << "Memory allocation failed" << endl;
      return false;
   }
   
   // Add the given values and initialize the rest.
    
   person_ptr->ID_ = ID;
   person_ptr->name_ = name;
   person_ptr->born_ = born;
   person_ptr->died_ = died;
   person_ptr->gender_ = gender;
   
   person_ptr->father = nullptr;
   person_ptr->mother = nullptr;
   person_ptr->visited = false;
    
   // Lastly add the person in the map.
    
   people.insert(make_pair(ID, person_ptr));
   
   return true;
}
// A function to print the info of one person, with the right indentation.
// Spouse is false by default, and changed true if a name of a spouse is
// being printed.
void Datastructure::print_one(Person* person_ptr, int rec_level, 
			      bool spouse) {

   cout << endl;
  
   // First, print empty spaces according to the level of recursion.
   // The indentation is four spaces for every generation.
    
   for ( int i = 0; i < rec_level; ++i ) {
      cout << "    ";
   }
   // If a spouse is being printed, a '&' must be added here.
   
   if ( spouse ) {
      cout << "& ";
   }
      
   cout << person_ptr->name_ << " " << person_ptr->born_ << " - ";
   
   // The year of death is only printed if it exists, that is,
   // it's not zero.
   if ( person_ptr->died_ != 0 ) {
      cout << person_ptr->died_;
   }
}
// To implement the print recursively, I wanted a pointer as a parameter,
// so I did a function in the private interface for the recursive part.
void Datastructure::print_rec(Person* person_ptr, int rec_level) { 
   // First, print the person for which the call has been made.
    
   print_one(person_ptr, rec_level);
    
   // The children are supposed to be printed in order of their year of birth.
   // They also have to be printed after both of their parents. But the
   // spouses are not necessarily in that order - they are in the order given
   // by the user. 
   
   // The assignment's specifications were quite ambiguous here.
   // It only talked about the age order of the children, but the test files
   // suggested that the spouses are printed in order of their addition
   // and children of that relationship are printed in order of their age.
   // So if a person has an older child with spouse 2 than with spouse 1,
   // it will be printed later.
    
   for ( vector<int>::size_type spo = 0; spo < person_ptr->spouses.size();
	 ++spo ) {
      
      // First print the name of the spouse.
      
      print_one(person_ptr->spouses.at(spo), rec_level, true);
       
      for ( auto child_it = person_ptr->children.begin();
	    child_it != person_ptr->children.end(); ++child_it ) {
	 
	 // Search for children with the spouse in question. If found, do
	 // a recursive call by which the child, his/her children are 
	 // printed.
	 
	 // 'child_it' refers to the map element, which is key+value. So 
	 // child_it->second refers to the value, which is a pointer. 
   
	 if ( person_ptr->gender_ == MALE and
	      child_it->second->mother->ID_ ==
	      person_ptr->spouses.at(spo)->ID_ ) {
	   	
	    // As we do a recursive call to a child, the print must be
	    // indented, which means print_one has to know the level
	    // of recursion.
   
	    print_rec(child_it->second, rec_level + 1);
	 }
      
	 else if ( person_ptr->gender_ == FEMALE and 
		   child_it->second->father->ID_ == 
		   person_ptr->spouses.at(spo)->ID_ ) {
	    print_rec(child_it->second, rec_level + 1);
	 }
      }
   }
}
void Datastructure::print(int ID) {
   // First check if ID exists. In cplusplus.com references it was recommended
   // to use the count-function to check existence, so I used that. 
   if ( ! people.count(ID) ) {
      return;
   }
    
   int rec_level = 0;
   
   print_rec(people.at(ID), rec_level);
   cout << endl;
}

// This function is called to the second ID in relation search. All of 
// ID1's ancestry has been marked as visited, so if one such node is found,
// the two are related.
Relation Datastructure::find_rel(Person* current_node) {
   
   // First check if node is coloured. If it is, 1 and 2 are related.
   if ( current_node->visited ) {
      return RELATED;
   }
   
   // Do recursive calls if parents exist. If the return value is
   // RELATED, pass it on.
   
   if ( current_node->mother != nullptr ) {
      
      if ( find_rel(current_node->mother) == RELATED ) {
	 return RELATED;
      }
      if ( find_rel(current_node->father) == RELATED ) {
	 return RELATED;
      }
   }
   return NOT_RELATED;
}
// A function to mark all ancestors of person 1, so a common ancestor can
// be found. 
void Datastructure::colour_ancestry(Person* current_node) {
   
   current_node->visited = true;
   
   // Do recursive calls to mother and father if they exist and
   // have not been visited yet.
   
   if ( current_node->mother != nullptr ) {
	
      if ( not current_node->mother->visited ) {
	 colour_ancestry(current_node->mother);
      }
      if ( not current_node->father->visited ) {
	 colour_ancestry(current_node->father);
      }
   }
} 
// The relation search is implemented by trying to find a common ancestor.
// This is done by marking the ancestry of person 1 and then seeing if we
// find any of them when searching through the ancestry of person 2.
// 
// There was a few ways to implement this, but I didn't see much difference
// in their efficiency. For example unordered set could be used to store the
// ID:s of person 1's ancestry, and then you wouldn't have to initialize
// the 'visited' bit. 
Relation Datastructure::relation(int ID1, int ID2) {
     
   // First we check if the ID's exist.
    
   if ( ! people.count(ID1) or 
	! people.count(ID2) ) {
      return INVALID_ID;
   }
   // Initialize all nodes as not visited.
   
   for ( auto it = people.begin(); it != people.end(); ++it )
      it->second->visited = false;
   
   // First we take person 1 and colour all his/her ancestry as visited.
   colour_ancestry(people.at(ID1));
   
   return find_rel(people.at(ID2));
}
// A function to add a spousal relationship between two people.
bool Datastructure::addPartner(int ID1, int ID2) {
      
   // First check whether both IDs exist.
  
   if ( ! people.count(ID1) or
	! people.count(ID2) ) {
      return false;
   }
   
   // If people are just married but don't have children, their genders don't
   // matter, they are not checked here. 
    
   // If they are already married, 'true' is returned but nothing gets added.
    
   Person* first = people.at(ID1);
   Person* second = people.at(ID2);
    
   // The spouses are stored in a vector. Check if the second ID is in the
   // first one's spouse vector.
    
   for ( vector<int>::size_type spo = 0; spo < first->spouses.size(); ++spo ) {
      if ( first->spouses.at(spo)->ID_ == ID2 ) 
	 return true;   
   }
   
   // Now we know that both ID's exist and there is not yet a spousal relationship
   // between them. First, add the second person as the spouse of the first one.
    
   first->spouses.push_back(second);
   
   // And vice versa:
    
   second->spouses.push_back(first);
    
   return true;
}
// A function to add a parental relationship between a child and its parents.
// A spousal relationship is formed between the parents if it doesn't already
// exist. 
bool Datastructure::addChild(int IDChild, int IDwoman, int IDman) { 
   // There are four checks in this one. The first is the existence of IDs.
   
   if ( ! people.count(IDChild) or
	! people.count(IDwoman) or
	! people.count(IDman) ) {
      return false;
   }
    
   // Check if the child already has parents. As before, checking one will
   // suffice.
    
   Person* child_ptr = people.at(IDChild);
    
   if ( child_ptr->mother != nullptr ) 
      return false;
      
   // The third check is whether the woman is actually female and the man
   // actually male.
    
   Person* woman_ptr = people.at(IDwoman);
   Person* man_ptr = people.at(IDman);
   
   if ( woman_ptr->gender_ == MALE or man_ptr->gender_ == FEMALE ) 
      return false;
   
   // Lastly, check whether the parents are already married, in which case the
   // spousal relationship will not be added.
    
   bool add_spousal = true;
    
   for ( vector<int>::size_type spo = 0; spo < woman_ptr->spouses.size();
	 ++spo ) {
      if ( woman_ptr->spouses.at(spo)->ID_ == IDman ) 
	 add_spousal = false;
   } 
    
   // Now, everything's in order and if the relationship doesn't exist yet, so
   // it is added.
    
   if ( add_spousal == true ) {
      woman_ptr->spouses.push_back(man_ptr);
      man_ptr->spouses.push_back(woman_ptr);
   }
    
   // Spousal relationship between the parents now exists. We add the child
   // to the parents info and vice versa. We don't need to check whether
   // the child is already in the children-map, as the child would
   // then have pointers to its parents (which we already checked).
    
   woman_ptr->children.insert(make_pair(child_ptr->born_, child_ptr));
   man_ptr->children.insert(make_pair(child_ptr->born_, child_ptr));
   
   child_ptr->mother = woman_ptr;
   child_ptr->father = man_ptr;
    
   return true;
}
// A function to erase the datastructure.
void Datastructure::remove() 
{
   // Iterate through the map and delete the structs to which the elements
   // point to. Delete takes a pointer as its "parameter".
    
   for ( auto it = people.begin(); it != people.end(); ++it) {
      delete it->second;
   }
   
   // Now we have just the map with pointers leading "nowhere". So just
   // clear that. There is no values to be reinitialized, so that's it.
    
   people.clear();
}
// A destructor would do the things that remove does, so it just calls that.
Datastructure::~Datastructure() {
   remove();
}
