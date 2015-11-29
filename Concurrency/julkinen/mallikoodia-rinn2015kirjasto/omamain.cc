// kurssikirjasto:
#include "rinn2015.h"
#include "client.h"
// c++11 kirjastoja:
#include <iostream>
#include <memory>
#include <cstdint>
#include <string>
#include <sstream>
#include <chrono>

int main()
{
  try {
    // 1. alustetaan kirjasto omalla opiskelijanumerolla
    rinn2015::init_server( 211661 );

    while( true ) {
      // 2. haetaan yksi http-yhteys käsittelyyn
      std::unique_ptr<rinn2015::Client> client( rinn2015::get_connection() );


      // tulostetaan olio:
      // client->print();

      auto start = std::chrono::high_resolution_clock::now(); 

      // 3. rakennetaan vastaustiedot
      client->reply_status = "200";
      std::string vastaus;
      vastaus = "Hello World!";
      client->reply_data = vastaus;

      // 4. lähetetään vastaus kirjaston avulla:
      uint32_t id = rinn2015::send_reply( std::move(client) );

      // 5. haetaan ja tulostetaan yhteyden tiedot:
      std::cout << "Palveltiin yhteys: " << rinn2015::peer_info( id )
		<< std::endl;

      auto stop = std::chrono::high_resolution_clock::now();
      auto diff = stop - start;
      std::clog.precision( 12 );
      std::clog << "Duration: " << std::fixed
		<< std::chrono::duration<double, std::micro>(diff).count() 
		<< " \u00B5s"
		<< std::endl;

    }

  } catch( std::exception &e ) {
    std::cerr << "exception in main(): " << e.what() << std::endl;
  }
}
