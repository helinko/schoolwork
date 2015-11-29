// MAT-72006 Advanced Algorithms and Data Structures
// Programming assignment
//
// Risto Helinko
// 211661
// risto.helinko@student.tut.fi
//
// Settlers of Catan
//
// Implements a board of hexagon tiles that is built in a spiral. User gives as input
// how many tiles he/she wants to know about, and then the indices of the tiles.
// The program then outputs the type of tile in the given index (1 through 5).
//
// The implementation is mainly based on the hexagonal pattern that is formed,
// and doing simple arithmetic on the indices.
//


#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

// The max amount of tiles
const unsigned long MAX_COUNT = 10000;

struct Tile {
   // These are explained in the pdf.
   unsigned long nr_overall; // from 1
   unsigned long ring; // the single tile is 0
   unsigned long side; // 1 through 6
   unsigned long nr_in_ring; // 1 through 6*ring +1
   unsigned long nr_in_side; // 1 through ring
};

std::vector<unsigned long> neighbors_of_first(Tile& current) {
   std::vector<unsigned long> neighbors;

   // I don't know how I would mathematically justify this formula,
   // I just worked it out from the picture.

   unsigned long this_index = current.nr_overall;
   unsigned long tiles_in_previous = (current.ring - 1) * 6;

   neighbors.push_back(this_index - tiles_in_previous);
   return neighbors;
}
std::vector<unsigned long> neighbors_of_last(Tile& current) {
   std::vector<unsigned long> neighbors;

   // Same here, I just worked this out from the picture.

   unsigned long this_index = current.nr_overall;
   unsigned long tiles_in_this = current.ring * 6;

   neighbors.push_back(this_index - tiles_in_this);
   // The other neighbor is the next tile from that
   neighbors.push_back(this_index - tiles_in_this + 1);

   return neighbors;
}
std::vector<unsigned long> neighbors_of_rest(Tile& current) {
   std::vector<unsigned long> neighbors;
   unsigned long first_neighbor = 0;
   unsigned long this_index = current.nr_overall;
   unsigned long tiles_in_previous = (current.ring - 1) * 6;

   // There are two cases in this, as the "corners" behave a bit differently
   // from the rest, and the index of the side has to be considered too.

   first_neighbor = this_index - tiles_in_previous - current.side;
   neighbors.push_back(first_neighbor);
   // If it's not a corner piece, add the next one too. The length of a side
   // is the same as the ring's index.
   if (! (current.nr_in_side == current.ring)) {
      neighbors.push_back(first_neighbor + 1);
   }
   return neighbors;
}
unsigned long determine_tile_type(Tile& current, 
				  const std::vector<unsigned long>& tiles,
				  const std::vector<unsigned long>& 
				  tilecounts) {
   std::vector<unsigned long> neighbors;
   
   // First, we have to determine what kind of place the current tile
   // is going to be in, and call the right function.
   if (current.nr_in_ring == 1) {
      neighbors = neighbors_of_first(current);
   }
   
   else if (current.nr_in_ring == current.ring * 6) {
      // That last part is how many tiles are in this ring
      neighbors = neighbors_of_last(current);
   }
   else {
      neighbors = neighbors_of_rest(current);
   }
   
   // Also, the previous tile is a neighbor, so add that to the vector
   neighbors.push_back(current.nr_overall - 1);
   
   // Now we have to check the resource IDs for all those tiles
   
   std::vector<unsigned long>::iterator it = neighbors.begin();
   std::vector<unsigned long> resources;
   
   while (it != neighbors.end()) {
      resources.push_back(tiles.at(*it));
      it++;
   }
   // Now go through the possible resource IDs. If an ID is not found
   // in that previous vector, add it to candidates.
   std::vector<unsigned long> candidates;
   
   for (unsigned long id = 1; id <= 5; id++) {
      it = std::find(resources.begin(), resources.end(), id);
      if (it == resources.end()) {
	 candidates.push_back(id);
      }
   }
   
   // Now we have all possible resource options, which have to be compared.
   unsigned long lowest_count = MAX_COUNT;
   unsigned long lowest_count_id = 0;
   unsigned long count = 0;
   it = candidates.begin();
   while (it != candidates.end()) {
      count = tilecounts.at(*it);
      if ( count < lowest_count ) {
	 // If there are equal counts, the lower resource ID will remain
	 // as the lowest_count_id.
	 lowest_count = count;
	 lowest_count_id = *it;
      }
      it++;
   }
   return lowest_count_id;      
}

void update_counts(Tile& current) {
   // There are 3 cases that effect the way the counts are updated
   // 
   // First, if the added tile was last in the ring
   if (current.nr_in_ring == current.ring * 6) {
      current.ring++;
      current.nr_in_ring = 1;
      current.side = 1;
      current.nr_in_side = 1;
   }
   // Then if it was a corner tile
   else if (current.nr_in_side == current.ring) {
      current.side++;
      current.nr_in_side = 1;
      current.nr_in_ring++;
   }
   else {
      
      // It's not a corner and not the last, so just increment the position
      // in the ring and side.
      current.nr_in_ring++;
      current.nr_in_side++;
   }
   
   
   // For all cases the overall number is of course incremented too.
   current.nr_overall++;
}
int main() {
   
   // The resource ID is saved here for every tile
   std::vector<unsigned long> tiles; 
   // The first index isn't used, so add 0 there
   tiles.push_back(0);
   // These are the counts for every resource
   std::vector<unsigned long> tilecounts;
   for (int i = 0; i <= 5; i++) 
      // First index isn't used
      tilecounts.push_back(0);
   
   // First, take user input. 
   
   std::vector<unsigned long> inputs;
   std::string input_str;   
   unsigned long input_ul = 0;
   
   std::cout << "How many test cases? ";
   std::getline(std::cin, input_str);
   
   input_ul = std::stoul(input_str);
   
   // Check input
   if ((input_ul > 200) or (input_ul < 1) ) {
      std::cerr << "Error: Unallowed amount of test cases!" << std::endl;
      return EXIT_FAILURE;
   }
   inputs.push_back(input_ul);
 
   // Loop as long as the user had specified
   for (unsigned long i = 0; i < inputs.at(0); i++) {
      std::cout << "Case number " << i + 1 << ": ";
      std::getline(std::cin, input_str);
      input_ul = std::stoul(input_str);
      if (input_ul < 1) {
	 std::cerr << "Error: Unallowed tile number!" << std::endl;
	 return EXIT_FAILURE;
      }
      inputs.push_back(input_ul);
   }
   
   // The first 7 tiles are added "by hand".
   
   for (unsigned long i = 1; i <= 5; i++) {
      tiles.push_back(i); // i = resource ID
      tilecounts.at(i)++;
   }
   // Two more
   tiles.push_back(2);
   tilecounts.at(2)++;
   tiles.push_back(3);
   tilecounts.at(3)++;
   
   // Now the first ring is done, and we move on to the actual algorithm.
   
   // Initialize the struct for current tile
   
   Tile current = { 8, 2, 1, 1, 1};
   
   // Determine the largest index that was queried.

   unsigned long largest = *std::max_element(inputs.begin() + 1, inputs.end());
  
   // Check the largest index
   if (largest > 10000) {
      std::cerr << "Error: Too many tiles!" << std::endl;
      return EXIT_FAILURE;
   }
      
   unsigned long tile_type = 0;
   
   // The main loop
   while (current.nr_overall <= largest) {
      tile_type = determine_tile_type(current, tiles, tilecounts);
      // "Add" tile
      tiles.push_back(tile_type);
      tilecounts.at(tile_type)++;
      
      // Update the info in the struct for the next round (includes the loop
      // variable)
      update_counts(current);    
	
   }
   // Print the asked values
   for (unsigned long i = 1; i <= inputs.at(0); i++) {
      std::cout << tiles.at(inputs.at(i)) << std::endl;
   }
}


   
