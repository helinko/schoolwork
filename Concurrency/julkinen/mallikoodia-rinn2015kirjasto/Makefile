BOOST_ROOT ?= /usr/local/lang/boost-1.57
RINN2015LIB ?= /home/rinn/pub/2015

CCBIN = g++ -pthread 
FLAGS = -std=c++11 -Wall -pedantic -I$(RINN2015LIB)
LDFLAGS= -L$(RINN2015LIB) -lrinn2015
#LDFLAGS+= -L$(BOOST_ROOT)/lib -lboost_system 

LDFLAGS+= -L$(BOOST_ROOT)/lib -Wl,-rpath,$(BOOST_ROOT)/lib -lboost_thread -lboost_thread-mt -lboost_system -lboost_system-mt 

CC = $(CCBIN) $(FLAGS) 

SOURCES = $(wildcard *.cc)
OBJS = $(patsubst %.cc, %.o, $(SOURCES))

.PHONY: default clean realclean

omamain.o : omamain.cc
	$(CC) -c $<

default: testipalvelin
	-echo Done

testipalvelin: $(OBJS)
	$(CCBIN) -o $@ $(OBJS) $(LDFLAGS)

clean:
	rm -f $(OBJS)

realclean: clean
	rm -f *~ testipalvelin

