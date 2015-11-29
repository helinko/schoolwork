#
# Esimerkki Makefile:stä Lintulassa (cs.tut.fi)
# Käännös tapahtuu ajamalla komentorivillä komento 'make'
#
# Lisätietoja make-ohjelmasta:
# http://www.cs.tut.fi/~ohjtuki/make/
#
# Käännösoptiot, joilla haluamme g++:n ajaa
CXXFLAGS += -g -W -Wall -pthread -std=c++11

# Suoritettavan ohjelmabinäärin nimi
BIN := TIE02500htyo

# luettelo projektiin kuuluvista lähdekooditiedostoista
# tämä Makefile ei osaa tehdä uudelleenkäännöstä jos jokin hh-tiedosto muuttuu
SRC_FILES := main.cc satunnaisluku.cc tulostus.cc testaa_satunnaislukuja.cc


O_FILES := $(SRC_FILES:%.cc=%.o)

$(BIN): $(O_FILES)
	$(CXX) $(LDFLAGS) -o $@ $^ -lpthread

.PHONY: clean

clean:
	rm -f $(O_FILES) *~ 
