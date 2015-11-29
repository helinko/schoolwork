
#include "buffer.hh"
#include "rinn2015.h"
#include <memory>
#include <iostream>

#include <queue>
#include <vector>

Buffer::Buffer()
{
  for (int i = 0; i <= 10; ++i)
  {
    std::queue<std::unique_ptr<rinn2015::Client> > *tyhja = new std::queue<std::unique_ptr<rinn2015::Client> >;
    priority_queue_.push_back(tyhja);
  }
}

Buffer::~Buffer(){
  for (int i = 0; i <= 10; ++i)
  {
    delete priority_queue_.at(i);
  }
}

/*antaa pienimmän prioriteetin omaavan odottavan clientin*/
std::unique_ptr<rinn2015::Client> Buffer::anna(){
  for (int i = 0; i <= 10; ++i)
  {
    if (priority_queue_.at(i)->size() != 0)
    {
      std::unique_ptr<rinn2015::Client> client(std::move(priority_queue_.at(i)->front()));
      priority_queue_.at(i)->pop();
      koko_--;
      return client;
    }
  }
  return NULL;
}

/*lisää uuden clientin jonoon oikean prioriteetin kohdalle*/
void Buffer::lisaa(std::unique_ptr<rinn2015::Client> uusi){

  int priority = 10;

  /*jos polusta löytyy prioriteetti*/
  if (!uusi->path.empty())
  {
    try{
      priority = std::stoi(uusi->path[0]);
    }
    catch(std::invalid_argument){
      // älä tee mitään 
    }
  }

  /*Jos polussa ei ole prioriteettia, lisätään viimeiseksi*/
  if (priority > 10)
  {
    priority = 10;
  }
  priority_queue_.at(priority)->push(std::move(uusi));

  koko_++;
}

/*Lähinnä aputulostuksia varten tehty laskuri puskurin koolle.*/
int Buffer::koko(){
  return koko_;
}


