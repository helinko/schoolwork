#include <iostream>
#include <pthread.h>
#include <stdexcept>
#include <memory>

class OskuOlio {
public:
  void heippa(void) {
    std::cerr << "Heippa maailma" << std::endl;
  }
};

// saierutiini
void* suorita(void* arg)
{
    // unique_ptr:n sisalta otettua osoitinta pitaa kayttaa nyt
    // tavallisena osoittimena:
    OskuOlio* olio = reinterpret_cast<OskuOlio*>(arg);
    olio->heippa();

    return reinterpret_cast<void*>(42);
}

void kaynnista(void)
{
  std::unique_ptr<OskuOlio> olio( new OskuOlio );

  pthread_t saie;
  // toimitetaan unique_ptr void* parametrina
  // koska tama on hyvin kankeaa, niin kannattaa miettia haluaako sita
  // tehda. Kaikille saikeille nakyva tietorakenne (vector?) saattaa olla
  // selkeampi rakenne...
  auto status = pthread_create( &saie, NULL,
                                &suorita, reinterpret_cast<void*>(olio.release()));
  if( status != 0 ) throw std::runtime_error( "pthread_create failed" );
}
