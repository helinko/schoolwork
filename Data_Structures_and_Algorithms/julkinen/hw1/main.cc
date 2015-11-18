// Main program for first homework of Utilization of datastructures
// and Tietorakenteet ja algoritmit.

#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <chrono>

#include "datastructure.hh"

// Known commands as constant chars
const char ADD = 'A';
const char READ = 'R';
const char FIND = 'F';
const char MAXMIN = 'X';
const char COUNT = 'C';
const char GENERATE = 'G';
const char TEST = 'T';
const char EMPTY = 'E';
const char QUIT = 'Q';

// Constant MAX is used when asking biggest value and MIN for smallest
const std::string OLD = "old";
const std::string YOUNG = "young";

const std::string E_FILE_NOT_FOUND = "Error: file not found.\n";

const std::string PROMPT = "> ";


// Function readFile is used to read one input file at time. 
void readFile(std::string const& filename, Datastructure& ds);

// Function generate is used to generate nodes to ds.
void generate(Datastructure& ds, unsigned int amount);

// Main program contains simple shell and it uses datastructure and
// handles error situations
int main() {
  
   std::srand(static_cast<unsigned int>(std::time(0)));
   char command = QUIT;
   std::string line;
   Datastructure ds;
   
   std::string rank;
   std::string name;
   unsigned int birthYear;
   unsigned int enlistingYear;
   std::string shirtColor;

      
   std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
   
  
   // Simple shell reads command and it's parameters if any and then
   // executes the command
   do {
      std::cout << PROMPT;
      std::cin >> command;
      std::cout << command;
      
      if(command == ADD) {
         std::cin >> rank >> birthYear >> enlistingYear >> shirtColor;
         std::getline(std::cin, name);
         name = name.substr(1);
         std::cout << SEPARATOR << rank << SEPARATOR << birthYear << SEPARATOR 
           << enlistingYear << SEPARATOR << shirtColor << SEPARATOR <<  name 
           << std::endl;
         ds.add(rank, birthYear, enlistingYear, shirtColor, name);
      }
      if(command == READ) {
         std::string filename;
         std::cin >> filename;
         std::cout << SEPARATOR << filename << std::endl;
         readFile(filename, ds);
      }
      else if(command == FIND) {
                  
         unsigned int index = 0;
         std::cin >> index;
         std::cout << SEPARATOR << index << std::endl;

         ds.find(index);
         
         
         
      }
      else if (command == TEST)
      {
         
         std::ofstream file("find.dat");
         file << "#x y" << std::endl;

         for ( int j = 0; j < 30; ++j )
         {
            double sum = 0.0;
            generate(ds, 250);
            for ( int i = 0; i < 5; ++i)
            {
               start = std::chrono::system_clock::now();
               generate(ds,1);
               ds.find((unsigned int)std::rand() % ds.size());
               end = std::chrono::system_clock::now();
               std::chrono::duration<double> elapsed = end-start;
               sum += elapsed.count();
            }
         
            file << ds.size() << ' ' << sum/5.0 << std::endl;
         }
         file.close();
         ds.empty();
         
         std::ofstream addfile("add.dat");
         addfile << "#x y" << std::endl;
         std::ofstream oldfile("old.dat");
         oldfile << "#x y" << std::endl;
         std::ofstream youngfile("young.dat");
         youngfile << "#x y" << std::endl;
         
         for ( int j = 0; j < 30; ++j )
         {
            double addsum = 0.0;
            double oldsum = 0.0;
            double youngsum = 0.0;
            generate(ds, 250);
            for ( int i = 0; i < 5; ++i)
            {   
               start = std::chrono::system_clock::now();
               generate(ds, 10);
               end = std::chrono::system_clock::now();
               std::chrono::duration<double> elapsed = end-start;
               addsum += elapsed.count();
               
               start = std::chrono::system_clock::now();
               generate(ds,1);
               ds.oldest();
               end = std::chrono::system_clock::now();
               elapsed = end-start;
               oldsum += elapsed.count();
               
               start = std::chrono::system_clock::now();
               generate(ds,1);
               ds.youngest();
               end = std::chrono::system_clock::now();
               elapsed = end-start;
               youngsum += elapsed.count();
               
            }
            
            addfile << ds.size() << ' ' << addsum/5.0 << std::endl;
            oldfile << ds.size() << ' ' << oldsum/5.0 << std::endl;
            youngfile << ds.size() << ' ' << youngsum/5.0 << std::endl;
            
         }
         
         addfile.close();
         oldfile.close();
         youngfile.close();

         ds.empty();
         std::ofstream delfile("del.dat");
         delfile << "#x y" << std::endl;
         
         for ( unsigned int j = 1; j <= 30; ++j )
         {
            double sum = 0.0;
            generate(ds, j*250);
            size_t size = ds.size();
            for ( int i = 0; i < 5; ++i)
            {
               start = std::chrono::system_clock::now();
               ds.empty();
               end = std::chrono::system_clock::now();
               std::chrono::duration<double> elapsed = end-start;
               sum += elapsed.count();
            }
            
            
            delfile << size << ' ' << sum/5.0 << std::endl;
         }
         
         delfile.close();
         
      }
      
      else if(command == MAXMIN) {
         std::string parameter;
         std::cin >> parameter;
         std::cout << SEPARATOR << parameter << std::endl;
         if(parameter == OLD) {
            ds.oldest();
         }
         else if(parameter == YOUNG) {
            ds.youngest();
         }
      }
      else if(command == EMPTY) {
         std::cout << std::endl;
         ds.empty();
      }
      else if (command == COUNT) {
         std::cout << std::endl << ds.size() << std::endl;
      }
      else if (command == GENERATE)
      {
         unsigned int amount = 0;
         std::cin >> amount;
         //std::cout << SEPARATOR << "Generating " << amount << " nodes to datatructure" << std::endl;
         generate(ds, amount);
         
      }
      
   } while(command != QUIT);
   
   std::cout << std::endl;
   

   return EXIT_SUCCESS;
}

// Function creates filestream from given filename and checks file
// existence. If file could not be read, prints error message. Then
// function reads all data from file and adds the data to the data
// structure.
void readFile(std::string const& filename, Datastructure& ds) {
  
   std::ifstream file(filename.c_str());
  
   if(!file) {
      file.close();
      std::cout << E_FILE_NOT_FOUND;
      return;
   }
  
   std::string line;
   std::string rank;
   std::string name;
   unsigned int birthYear;
   unsigned int enlistingYear;
   std::string shirtColor;
   
  
   // While loop reads file line by line and adds one ship at time to
   // the data structure.
   while(std::getline(file, line)) {

      std::istringstream linestream(line); 
      linestream >> rank >> birthYear >> enlistingYear >> shirtColor;    
      std::getline(linestream, name);
      name = name.substr(1);
      ds.add(rank, birthYear, enlistingYear, shirtColor, name);
   }

   file.close();
}


void generate(Datastructure& ds, unsigned int amount)
{
   static const char alphas[] = "abcdefghijklmnopqrstuvwxyz";
   
   for( unsigned int j = 0; j < amount; ++j )
   {
      std::string rank (8, ' ');
      for ( size_t i = 0; i < 8; ++i )
      {
         rank[i] = alphas[(unsigned int)std::rand() % (sizeof(alphas) - 1)];
      }
      
      std::string name (10, ' ');
      
      for ( size_t i = 0; i < 10; ++i )
      {  
         name[i] = alphas[(unsigned int)std::rand() % (sizeof(alphas) - 1)];
      }
      
      unsigned int year = (unsigned int)std::rand() % 200000;
      unsigned int enlistingYear = (unsigned int)std::rand() % 60 + 2200 + year;
      unsigned int birthYear = year + 2200;
      
      static const std::string colors[3] = {"gold", "blue", "red" };
      
      std::string shirtColor = colors[(unsigned int)std::rand() % 3];
      

      
      ds.add(rank, birthYear, enlistingYear, shirtColor, name);
   }
   
   
}
