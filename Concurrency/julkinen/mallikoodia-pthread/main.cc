#include <iostream>
#include <vector>
#include <pthread.h>
#include <time.h>
#include "tulostus.hh"
#include "satunnaisluku.hh"
#include "testaa_satunnaislukuja.hh"


// saikeen koodi.
// toiminnallisuus:
// arpoo luvun, tulostaa oman numeronsa, odottaa satunnaisen ajan ja palauttaa luvun
void* suorita(void* arg)
{
    Random arpoja;
    auto tunnus = reinterpret_cast<long int>( arg );
    auto arpa = arpoja.integer( 1, 38 );
    auto nukku = arpoja.integer( 999 );
    timespec aika = { 0, nukku*1000000 };
        
    { tulostus::lukko l;
      std::cerr << "Käynnissä säie: " << tunnus << std::endl;
    }
    if( nanosleep( &aika, nullptr ) != 0 ) {
      tulostus::lukko l;
      std::cerr << "nanosleep() error " << tunnus << std::endl;
    }

    return reinterpret_cast<void*>( arpa );
}

// pääohjelma luo 7 säiettä, odottaa niiden valmistumisen
// ja tulostaa säikeiden arpomat luvut
int main( int , char** )
{
  { tulostus::lukko l;
    std::cerr << "Käynnistys." << std::endl;
  }

  // säikeet käyntiin ja tunniste talteen taulukkoon
  std::vector<pthread_t> saikeet;
  for( auto i = 1; i <= 7; i++ )
  {
      auto num = reinterpret_cast<void*>( i );
      pthread_t saie;
      auto status = pthread_create( &saie, NULL, &suorita, num );
      if( status != 0 ) {
          perror( "pthread_create" );
          exit(1);
      }
      saikeet.push_back( saie );
  }
   
  // käydään läpi käynnistetyt säikeet ja kerrotaan kunkin
  // paluuarvo (satunnaisluku)
  for( auto& thr : saikeet )
  {
      void* retval = 0;
      auto status = pthread_join( thr, &retval  );
      if( status != 0 ) {
          perror( "pthread_join" );
          exit(2);
      }

      { tulostus::lukko l;
        auto luku = reinterpret_cast<long int>( retval );
        std::cerr << "Säikeen arpoma luku: " << luku << std::endl;
      }
  }
  { tulostus::lukko l;
    std::cerr << "Säikeet lopettivat, tutkitaan jakaumia..." << std::endl;
  }
  testaa_satunnaislukuja();
  { tulostus::lukko l;
    std::cerr << "Lopetus." << std::endl;
  }
  return 0;
}
