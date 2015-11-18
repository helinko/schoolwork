// Definition of Datastructure class for UDS/Tiraka homework 2 -
// Family tree.

#include <iostream>
#include <cstdlib>
#include <string>
#include <cctype>

#include "datastructure.hh"

using namespace std;

int main() {
    
    int id1 = 0;
    int id2 = 0;
    int id3 = 0;
    string name = "";
    int born = 0;
    int died = 0;
    
    Datastructure familytree;
    
    char command = ' ';
    while(true) {
	cin >> command;
	switch(command) {
	    
	  case 'Q':
	      
	      // Quits the program. Prints end on a line of its own.
	      cout << "the end" << endl;      
	      familytree.remove();
	      return EXIT_SUCCESS;
	      //break;
	      
	  case 'M':
	      
	      //If the archive number (id) is not unique, print "V".
	      //Otherwise a new man is added into the tree and "M"is printed.
	      //A missing year of death is indicated with 0.
	      
	      while( isspace( cin.peek() ) ) {
		  cin.get();
	      }
	      getline(cin, name, '\n');
	      
	      cin >> id1 >> born >> died;
	      // add a new man into the tree
	      
	      if( familytree.add(MALE, name, id1, born, died) ) {
		  cout << "M ";
	      }
	      else {
		  cout << "V" << endl;
	      }
	      break;    
	      
	  case 'N':
         
	   //If the archive number (id) is not unique, print "V".
	   //Otherwise a new woman is added into the tree and "N" is printed.
	   //A missing year of death is indicated with 0.
	           
	      while( isspace( cin.peek() ) ) {
		  cin.get();
	      }
	      getline(cin, name, '\n');
	      
	      cin >> id1 >> born >> died;
	      
	      // add a new woman
	      if( familytree.add(FEMALE, name, id1, born, died)) {
		  cout << "N ";
	      }
	      else {
		  cout << "V" << endl;
	      }
	      break;
	      
	  case 'A':
         
	      //A spousal relationship is formed between the two people.
	      //"A"is printed. If one of the people are not found in the
	      //family tree, "V"is printed. If there already is a spousal 
	      //relationship between the two, "A"is printed, but
	      //nothing gets added.
         
         cin >> id1 >> id2;
         // add partners : A <id1> <id2>
         if (familytree.addPartner(id1,id2)) {
            cout << "A ";
         } else {
            cout << "V" << endl;
         }
        
         break;
         
       case 'P':
         
	   //Forms the required family relations between the people and
	   //prints "P". A spousal relationship is created between
	   //the mother and the father. If some of the people are not found
	   //in the family tree, someone is of an impossible gender, or
	   //the child already has parents, "V"is printed.
         
         cin >> id1 >> id2 >> id3;
         
         if(familytree.addChild(id1,id2,id3)) {
            cout << "P ";
         } else {
            cout << "V" << endl;
         }
         
         break; 
         
       case 'K':
         
	   //Investigates if the persons are related to each other.
	   //If they are, "+"is printed, otherwise " – "is printed.
	   //A person is related to him/herself. If one is not found 
	   //in the tree, "V"gets printed.

         
         cin >> id1 >> id2;
         // related : K <person> <person>
         switch (familytree.relation(id1,id2)) {
          case INVALID_ID:
	      cout << "V" << endl;
            break;
          case RELATED:
	      cout << "+" << endl;
	      break;
          case NOT_RELATED:
	      cout << "-" << endl;
	      break;
         }
         break;
         
       case 'T':
         
	   //Prints the id’s social family i.e. partners and offspring data
	   //hierarchically as an indented list. A child together forms 
	   //a spousal relationship between people.
	   //The relationships are printed in the order they have been added.
	   //Children are printed in age order, oldest first.
	   //If the person is not found in the tree, "V"gets printed.
         
	   cin >> id1;
	   // print familytree : T <person> 
	   familytree.print(id1);
         break;
         
       default:
	   familytree.remove();
	   return EXIT_SUCCESS;
      }
   }
}
