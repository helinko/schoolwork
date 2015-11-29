#ifndef SEMAFORI_HH
#define SEMAFORI_HH
#include <pthread.h>
#include <exception>
#include <string>


//Luokka semaforin luomiseen
class semafori {
  public:
    semafori(int init_val, std::string name);
    void lukitse();
    void avaa();
    void signaloi();
  private:
    pthread_mutex_t suojaus_;
    pthread_cond_t ehto_;
    int val_;
    std::string name_; /*Nimi vain aputulostuksia varten*/
};

#endif
