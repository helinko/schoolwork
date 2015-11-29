// kurssikirjasto:
#include "rinn2015.h"
#include "client.h"
#include "semafori.hh"
#include "buffer.hh"
// c++11 kirjastoja:
#include <iostream>
#include <memory>
#include <cstdint>
#include <string>
#include <sstream>
#include <chrono>
#include <deque>
#include <queue>
#include <vector>
#include <map>
#include <fstream>
//Nukkumista varten
#include <unistd.h>


/*Alustukset*/

const int OPNRO = 218948;
const int PUSKURIN_KOKO = 100;
const int PUSKURIN_2_KOKO = 100;
const int TUOTTAJASAIKEITA = 3;
const int KULUTTAJASAIKEITA = 4;


namespace {
   //Buffer http-yhteyksien tallentamiseen
   Buffer buffer;

   enum Virhekoodi 
     {
	OK, // 200 OK
	FAIL // 404 Not Found
     };

   /*Yhteys-struct, kirjoittajaa varten*/
   struct Yhteys {
      Virhekoodi koodi;
      uint32_t ID;
      std::vector<std::string> polku; // pyydetyn dokumentin polku
   };
            
   /*Bufferi käsiteltyjen yhteyksien lokitiedostoon kirjoittamista varten*/
   std::queue<Yhteys> buffer2;
   
   /*Kaikki palvelimen tuntemat polut ja avain-arvo-parit tallenetaan tänne*/
   std::map< std::string, std::map<std::string, std::string> > polut;
   
   /*Käytetyt semaforit*/
   semafori tuottaja_semafori(PUSKURIN_KOKO, "tuottaja");
   semafori kuluttaja_semafori(0, "kuluttaja");

   semafori lukija_semafori(PUSKURIN_2_KOKO, "lukija");
   semafori kirjoittaja_semafori(0, "kirjottaja");

   /*Mutex uuden yhteyden hakemiselle */
   pthread_mutex_t yhteys_mutex;

   // testitulostuksia varten
   int yhteyksia_palveltu = 0;
}


   // Tuottaja hakee uusia http-yhteyksiä palvelimelta
void* Tuottaja(void* arg){
      
   while(true){
      // Haetaan yksi http-yhteys käsittelyyn
      pthread_mutex_lock(&yhteys_mutex);
      std::unique_ptr<rinn2015::Client> client( rinn2015::get_connection() );
      pthread_mutex_unlock(&yhteys_mutex);
         
      // Lukitaan tuottaja-semafori.
      tuottaja_semafori.lukitse();
      
      // lisätään alkio puskuriin.
      buffer.lisaa(std::move(client));

      // herätetään kuluttaja
      kuluttaja_semafori.signaloi();
      
      // avataan semafori
      tuottaja_semafori.avaa();
   }
      
}
//Kuluttaja palvelee bufferiin lisätyt yhteydet
void* Kuluttaja(void* arg){
      
   while(true){
      // Lukitaan kuluttaja-semafori
      kuluttaja_semafori.lukitse();

      // Otetaan alkio puskurista. 
      std::unique_ptr<rinn2015::Client> client = buffer.anna();

      // herätetään tuottaja
      tuottaja_semafori.signaloi();
      
      // avataan semafori
      kuluttaja_semafori.avaa();
         
      // Luodaan yhteysmuuttuja myöhempää lokikirjoitusta varten.
      Yhteys yhteys;
         
      // Tutkitaan clientin path-osiota ja polkua johon pyyntö tuli
      std::string polku; //path-osiosta löytynyt polku
      std::string id; // Id on Get-pyynnöissä path osion viimeinen alkio
      
      polku += "/";
      if ( client->path.size() > 1 )
	{
	   polku += client->path.at(1);
	}

      if ( polku == "/" ) {
	 if(client->type == "GET") {
	    client->reply_status = "200";
	    std::string vastaus = "Etusivu \n";
	    client->reply_data = vastaus;
	    // Kirjataan yhteyden tietoihin, että se palveltiin
	    yhteys.koodi = OK;
	 } else {
	    // Ei ollut tunnettu -> palautetaan 404 Not Found ja muokataan
	    // lokikirjoituksen tiedot samoin.
	    client->reply_status = "404";
	    client->reply_data = "Ei mahdollinen \n"; 
	    // Kirjataan yhteyden tietoihin, että ei palveltu
	    yhteys.koodi = FAIL;
	 }
      }
         
      /* GET-pyynnön käsittelymekanismi */
      else if (client->type == "GET")
	{
	   /*Etsitään prioriteetin jälkeistä polkua*/
	   if ( client->path.size() > 2 )
	     {
		id = client->path.at(2);
	     }
	   /*katsotaan löytyykö etsitty avain polut-tietorakenteesta*/
	   std::map< std::string, std::map<std::string, std::string> >::iterator iter = polut.find(polku);
	   if (iter != polut.end()) {
	      std::map<std::string, std::string>::iterator iter2 = iter->second.find(id);
	      
	      /*Mikäli avain löytyy tietorakenteesta*/
	      if (iter2 != iter->second.end() )
		{
		   client->reply_status = "200";
		   std::string vastaus = iter2->second + "\n";
		   client->reply_data = vastaus;
		   // Kirjataan yhteyden tietoihin, että se palveltiin
		   yhteys.koodi = OK;
		}
	      /*Kun avainta ei löydy*/
	      else {
		 // Ei ollut tunnettu -> palautetaan 404 Not Found ja muokataan
		 // lokikirjoituksen tiedot samoin.
		 client->reply_status = "404";
		 client->reply_data = "Not Found \n";
		 // Kirjataan yhteyden tietoihin, että ei palveltu
		 yhteys.koodi = FAIL;
	      }
	   }
	   
	   /*Kun polku oli tuntematon*/
	   else {
	      // Ei ollut tunnettu -> palautetaan 404 Not Found ja muokataan
	      // lokikirjoituksen tiedot samoin.
	      client->reply_status = "404";
	      client->reply_data = "Not Found \n";
	      // Kirjataan yhteyden tietoihin, että ei palveltu
	      yhteys.koodi = FAIL;
	   }
	}

      /*POST-pyynnön käsittely*/
      else if (client->type == "POST" )
	{
	   /*Jos post pyynnölle on annettu paremetreja, ne löytyy post-muuttujasta*/
	   if (client->post != "NULL")
	     {
		/*Käsitellään post-muuttujan sisältämä string.
		 Muodon täytyy olla "avain=arvo". Ei virhetarkastelua vääränlaisille post-parameterille!
		 */
		std::string s = client->post;
		std::string delimiter1 = "=";
		std::string first = s.substr(0, s.find(delimiter1));
		std::string second = s.substr(s.find(delimiter1)+1);

		std::map< std::string, std::map<std::string, std::string> >::iterator iter = polut.find(polku);
		
		/*Kun tunnetaan polku, voidaan lisätä rakenteeseen*/
		if (iter != polut.end()) {

		   std::pair<std::string, std::string> lisattava = make_pair(first, second);
		   iter->second.insert(lisattava);
		   
		   client->reply_status = "200";
		   std::string vastaus = "Lisays onnistui! \n";
		   client->reply_data = vastaus;
		   // Kirjataan yhteyden tietoihin, että se palveltiin
		   yhteys.koodi = OK;
		}
		/*Kun polkua ei oltu määritelty*/
		else {
		   // Ei ollut tunnettu -> palautetaan 404 Not Found ja muokataan
		   // lokikirjoituksen tiedot samoin.
		   client->reply_status = "404";
		   client->reply_data = "Not found \n";
		   // Kirjataan yhteyden tietoihin, että ei palveltu
		   yhteys.koodi = FAIL;
		}
		
	     }
	}
      /*PUT-pyynnön käsittely*/
      else if (client->type == "PUT" )
	{
	   /*Kun parametrit löytyvät extra-path -muuttujasta*/
	   if (client->extra_path != "NULL")
	     {
		std::string s = client->extra_path;
		std::string delimiter1 = "=";
		std::string first = s.substr(0, s.find(delimiter1));
		std::string second = s.substr(s.find(delimiter1)+1);

		std::map< std::string, std::map<std::string, std::string> >::iterator iter = polut.find(polku);

		/*Kun polku on tunnettu*/
		if (iter != polut.end()) {

                   std::map<std::string, std::string>::iterator iter2 = iter->second.find(first);
		   
		   /*Kun avain on jo rakenteessa, voidaan sen tietoja muokata*/
		   if (iter2 != iter->second.end() )
		     {
			iter2->second = second;
			
			client->reply_status = "200";
			std::string vastaus = "Päivitys onnistui! \n";
			client->reply_data = vastaus;
			// Kirjataan yhteyden tietoihin, että se palveltiin
			yhteys.koodi = OK;

			/*Avainta ei löytynyt rakenteesta*/
		     }else {
			// Ei ollut tunnettu -> palautetaan 404 Not Found ja muokataan
			// lokikirjoituksen tiedot samoin.
			client->reply_status = "404";
			client->reply_data = "Not found \n"; 
			// Kirjataan yhteyden tietoihin, että ei palveltu
			yhteys.koodi = FAIL;
		     }
		}
		/*Kun polkua ei tunneta*/
		else {
		   // Ei ollut tunnettu -> palautetaan 404 Not Found ja muokataan
		   // lokikirjoituksen tiedot samoin.
		   client->reply_status = "404";
		   client->reply_data = "Not found \n"; // tähän ei kai tule mitään?
		   // Kirjataan yhteyden tietoihin, että ei palveltu
		   yhteys.koodi = FAIL;
		}
		
	     }
	}

      /*DELETE-pyynnön käsittely*/
      else if (client->type == "DELETE")
	{
	   /*Polusta prioriteetin jälkeen löytyy mihin polkuun delete pyyntö on kohdistettu*/
	   if ( client->path.size() > 2 )
	     {
		id = client->path.at(2);
	     }

	   /*Etsitään avainta tietrakenteesta*/
	   std::map< std::string, std::map<std::string, std::string> >::iterator iter = polut.find(polku);
	   if (iter != polut.end()) {
	      std::map<std::string, std::string>::iterator iter2 = iter->second.find(id);
	      
	      /*Kun avain on tietorakenteessa, voidaan se poistaa*/
	      if (iter2 != iter->second.end() )
		{
		   iter->second.erase(iter2);
		   client->reply_status = "200";
		   std::string vastaus = "Poistettu onnistuneesti! \n";
		   client->reply_data = vastaus;
		   // Kirjataan yhteyden tietoihin, että se palveltiin
		   yhteys.koodi = OK;
		}
	      /*Kun avainta ei löydy*/
	      else {
		 // Ei ollut tunnettu -> palautetaan 404 Not Found ja muokataan
		 // lokikirjoituksen tiedot samoin.
		 client->reply_status = "404";
		 client->reply_data = "Not Found \n"; 
		 // Kirjataan yhteyden tietoihin, että ei palveltu
		 yhteys.koodi = FAIL;
	      }
	   }

	   /*Kun polku on tuntematon*/
	   else {
	      // Ei ollut tunnettu -> palautetaan 404 Not Found ja muokataan
	      // lokikirjoituksen tiedot samoin.
	      client->reply_status = "404";
	      client->reply_data = "Not Found \n"; 
	      // Kirjataan yhteyden tietoihin, että ei palveltu
	      yhteys.koodi = FAIL;
	   }
	}

      // Lisätään pyydetyn dokumentin polku yhteyden tietoihin.
      yhteys.polku = client->path;
      
      // lähetetään vastaus kirjaston avulla (tämä on säieturvallista, 
      // joten mutexia ei tarvita.)
      uint32_t clientId = rinn2015::send_reply( std::move(client) );

      // Voidaan simuloidaan oikeata käsittelyä, jos laitetaan käsittely kestämään
      //usleep( 1 * 1000 * 100 );
      
      yhteys.ID = clientId;      

      /*lukitaan lukija*/
      lukija_semafori.lukitse();

      // lisätään alkio puskuriin.
      buffer2.push(yhteys);
      
      // herätetään kirjoittaja
      kirjoittaja_semafori.signaloi();
      
      // avataan mutex
      lukija_semafori.avaa();  
      
   }
   
}

// Lokikirjoittaja saa kuluttajalta buffer2:n välityksellä id:n ja viestin, 
// jonka perusteella se kirjoittaa lokitiedostoon tiedot käsitellystä 
// yhteydestä, tai mahdollisesti siitä, mikä meni pieleen.
// 
void* Lokikirjoittaja(void* arg) {
      
   // Luodaan tiedostovirtamuuttuja
   std::ofstream loki;
   
   // Avataan tiedosto.
   loki.open("HTTP.LOKI");
      
   while (true) {
      // Otetaan puskurista tiedot ja kirjoitetaan rivi tiedostoon. Ensin
      // lukitaan semafori.
      kirjoittaja_semafori.lukitse();

      // Otetaan alkio puskurista.
      Yhteys yhteys = buffer2.front();
      buffer2.pop();
      
      // Tarkistetaan, että tiedosto on auki. Jos ei, virheilmoitus ja exit
      if (! loki.is_open()) {
	 std::cerr << "Tiedosto ei ollut auki." << std::endl;
	 exit(EXIT_FAILURE);
      }
      // Jos tiedosto oli auki, kirjoitetaan sinne mitä tehtiin. Erotellaan onnistunut
      // ja epäonnistunut tapaus.
         
      if (yhteys.koodi == OK) {
	 // Yhteys palveltiin onnistuneesti.      
	 loki << "Palveltiin yhteys: " << rinn2015::peer_info( yhteys.ID )
	   << std::endl << "Polku oli: ";
	 // tulostetaan polku
	 for (unsigned int i = 0; i < yhteys.polku.size(); ++i) {
	    loki << "/" << yhteys.polku.at(i);
	 }

	 loki << std::endl;
      }
      else if (yhteys.koodi == FAIL) {
	 // Yhteyden palveleminen epäonnistui.
	 loki << "Yhteys pyysi tuntematonta dokumenttia: " << rinn2015::peer_info( yhteys.ID )
	   << std::endl;
	 // tulostetaan polku
	 for (unsigned int i = 0; i < yhteys.polku.size(); ++i) {
	    loki << "/" << yhteys.polku.at(i);
	 }
	 loki << std::endl;
      }
      // herätetään lukija
      lukija_semafori.signaloi();

      // avataan semafori
      kirjoittaja_semafori.avaa();    
   }
      
}



int main(int argc, char* argv[]) {

   /*käsitellään ohjelmalle annetut paremtrit*/
   if (argc > 1)
     {
	std::ifstream file(argv[1]);
	if(!file) 
	  {
	     std::cout<<"Virhe tiedoston lukemisessa. Polkuja ei tallennettu!"<<std::endl;
	  }
	else{
	   while(!file.eof())
	     {
		std::string line = "";
		getline(file, line, '\n');
		
		std::map<std::string, std::string> uusi_map;
		std::pair<std::string, std::map<std::string, std::string> > lisattava = make_pair(line, uusi_map);
		polut.insert(lisattava);

		if (polut.find(line) != polut.end()){
		   std::cout << "Lisays onnistui: " << line << std::endl;
		}
	     }
	}
	std::cout << "Luettu tiedosto: "
	  << argv[1] << std::endl;
     }

   try {
         
      // alustetaan mutex 
      pthread_mutex_init(&yhteys_mutex, 0);
      
      // alustetaan kirjasto omalla opiskelijanumerolla
      rinn2015::init_server( OPNRO );
      
      std::vector<pthread_t> tuottajat;
      std::vector<pthread_t> kuluttajat;

      //Tuottaja-säikeet käyntiin ja tunniste talteen taulukkoon
      for( auto i = 1; i <= TUOTTAJASAIKEITA; i++ ) {
	 auto num = reinterpret_cast<void*>( i );
	 pthread_t saie;
	 auto status = pthread_create( &saie, NULL, &Tuottaja, num );
	 if( status != 0 ) {
	    perror( "pthread_create" );
	    exit(1);
	 }
	 tuottajat.push_back( saie );
      }
      
      //Kuluttaja-säikeet käyntiin ja tunniste talteen taulukkoon
      for( auto i = 1; i <= KULUTTAJASAIKEITA; i++ )
	{
	   auto num = reinterpret_cast<void*>( i );
	   pthread_t saie;
	   auto status = pthread_create( &saie, NULL, &Kuluttaja, num );
	   if( status != 0 ) {
	      perror( "pthread_create" );
	      exit(1);
	   }
	   kuluttajat.push_back( saie );
	}
      // Lokikirjoittaja käyntiin
      pthread_t lokikirjoittaja;
      int i = TUOTTAJASAIKEITA + KULUTTAJASAIKEITA + 1;      
      auto num = reinterpret_cast<void*>( i );      
      auto status  = pthread_create( &lokikirjoittaja, NULL, &Lokikirjoittaja,
				     num );
      if( status != 0 ) {
	 perror( "pthread_create" );
	 exit(1);
      }
            
      while( true ) {
	 // ohjelma jää looppaamaan tähän, ellei poikkeuksia tule
      }

   } 
   catch( std::exception &e ) {
      std::cerr << "exception in main(): " << e.what() << std::endl;
   }
}
