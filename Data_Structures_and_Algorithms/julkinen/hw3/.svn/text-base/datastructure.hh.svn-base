// Definition of Datastructure class for UDS/Tiraka homework 2 -
// Family tree.


#ifndef  DATASTRUCTURE_HH
#define  DATASTRUCTURE_HH

enum Gender { MALE, FEMALE };

enum Relation {INVALID_ID, RELATED, NOT_RELATED };

class Datastructure {
   
 public:
   
    Datastructure();
    ~Datastructure();
    
    // adds a new person
    bool add(Gender gender, string& name, int ID, int born, 
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
   
};

#endif

