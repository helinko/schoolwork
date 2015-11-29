#ifndef BUFFER_HH
#define BUFFER_HH
#include "rinn2015.h"
#include <memory>
#include <string>
#include <vector>
#include <queue>

/*Tuottaja buffer voidaan toteuttaa tällä luokalla.
Korkein mahdollinen prioriteetti on 0 ja matalin 9. Prioriteetti 10 annetaan prioriteetittomille pyynnöille.
*/

class Buffer {
  public:
    Buffer();
    ~Buffer();
    /*Lisätään uudis client-pyyntöjä jonoon*/
    void lisaa(std::unique_ptr<rinn2015::Client> uusi);
    std::unique_ptr<rinn2015::Client> anna();
    int koko(); /*aputulostuksia varten, ei muuta käyttötarkoitusta*/
    
  private:
  	/*11-alkion mittainen vektori, jossa jonot pyynnöille*/
    std::vector< std::queue<std::unique_ptr<rinn2015::Client> >* > priority_queue_;
    int koko_;
};

#endif
