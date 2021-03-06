// Definition of Datastructure class for UDS/Tiraka homework 2 -
// Family tree.


#ifndef  DATASTRUCTURE_HH
#define  DATASTRUCTURE_HH

#include <string>
#include <unordered_map>
#include <map>
#include <vector>

enum Gender { MALE, FEMALE };

enum Relation {INVALID_ID, RELATED, NOT_RELATED };

class Datastructure {
   
 public:
   
   Datastructure();
   ~Datastructure();
   
   // adds a new person
   bool add(Gender gender, std::string& name, int ID, int born, 
	    int died);
   
   //Adds ID1 and ID2 as partners
   bool addPartner(int ID1, int ID2);
    
   //Adds a child to the given people
   bool addChild(int IDChild, int IDwoman, int IDman);
    
   //Checks if the two people are related
   Relation relation(int ID1, int ID2);
   
   //Empties the data structure
   void remove();
   
   //Prints the family tree
   void print(int ID);
   
 private:
   // People are saved in structs, which in turn are saved in an
   // unordered map for quick indexing with the ID. The ID is
   // also saved inside the struct, because usually it's needed 
   // for comparison when the person has been encountered via 
   // pointers, and so the key (to my knowledge) cannot be 
   // accessed. At least not as efficiently. 
     
   struct Person {
      int ID_;
      std::string name_;
      int born_;
      int died_;
      Gender gender_;
      Person* father;
      Person* mother;
      // The children are saved in a multimap, and the key is 
      // the year of birth, so they will be sorted by it.
      std::multimap<int, Person*> children;
      std::vector<Person*> spouses;
      bool visited;
   };
   
   std::unordered_map<int, Person*> people;
   
   // Two functions to help the relation search.
   Relation find_rel(Person* current_node);
   void colour_ancestry(Person* current_node);
   
   // Two functions to help print the datastructure.
   void print_one(Person* person_ptr, int rec_level, bool spouse = false);
   void print_rec(Person* person_ptr, int rec_level);
   
};

#endif

