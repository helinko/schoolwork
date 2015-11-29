#include "semafori.hh"
#include <iostream>


semafori::semafori(int init_val, std::string name)
{
  val_ = init_val;

  /*testausta ja aputulostusta varten semaforille voi antaa nimen*/
  name_ = name;
  
  pthread_mutex_init(&suojaus_, 0);
  pthread_cond_init(&ehto_, 0);
}


//Lukitsee semaforin
void semafori::lukitse()
{
  pthread_mutex_lock( &suojaus_ ); 

  // Jos puskurissa on maksimimäärä/liian vähän alkioita, odotetaan.     
  while ( val_ <= 0 ) {
     pthread_cond_wait( &ehto_, &suojaus_);
  };
  val_--;

}

//Avaa semaforin
void semafori::avaa()
{
  pthread_mutex_unlock( &suojaus_ );

}

/*signaloi*/
void semafori::signaloi(){
	val_++;
	pthread_cond_signal( &ehto_);

}

