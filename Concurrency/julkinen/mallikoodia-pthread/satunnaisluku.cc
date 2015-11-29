
#include "satunnaisluku.hh"
#include <pthread.h>

#ifdef DEBUG
#include <iostream>
#endif

namespace {
  // moduulin sisäinen tiedosto-operaatioiden sarjallistamiseen:
  pthread_mutex_t urandom_mutex = PTHREAD_MUTEX_INITIALIZER;
}

Random::Random() : file_( "/dev/urandom", std::ios::in | std::ios::binary )
{
  // koodi toimii vain linux/unix alustalla, jossa on urandom
  if( !file_ ) {
    throw RandError( "cannot open /dev/urandom" );
  }
}

uint32_t Random::integer()
{
  uint32_t number;

  // tiedosto on yhteinen resurssi, joten sen luku suojataan:
  if( pthread_mutex_lock( &urandom_mutex ) != 0 )
    throw RandError( "urandom_mutex lock failed" );

  // luetaan 4 satunnaista tavua muuttujaan -> tulos on 32 bittinen satunnaisluku
  file_.read( reinterpret_cast<char*>(&number), 4 );
  if( pthread_mutex_unlock( &urandom_mutex ) != 0 )
    throw RandError( "urandom_mutex unlock failed" );

  // tarkistetaan, että tavujen luku oikeasti onnistui:
  if( !file_ ) {
    throw RandError( "/dev/urandom read(4) failed" );
  }
#ifdef DEBUG
  std::clog << "integer():" << number << std::endl;
#endif
  return number;
}

// hyvin jakautuneita satunnaislukuja on vaikea tuottaa.
// tämä koodi on peräisin lähteestä:
// https://www.mikeash.com/pyblog/friday-qa-2011-03-18-random-numbers.html
uint32_t Random::integer( uint32_t topPlusOne )
{
  uint32_t two31 = 1U << 31;
  uint32_t maxUsable = (two31 / topPlusOne ) * topPlusOne;

  while(true)
  {
    uint32_t num = integer();
    if( num < maxUsable )
      return num % topPlusOne;
  }
}

uint32_t Random::integer( uint32_t lower, uint32_t upper )
{
  uint32_t range = upper - lower;
  uint32_t num = integer( range );
  return num + lower;
}

Random::~Random()
{
  file_.close();
}
