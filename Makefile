CXXFLAGS=-O3 -Werror -Wall -pedantic -std=c++14
LDLIBS+=`pkg-config --libs libzmq`
LDLIBS+=-lboost_serialization

all: producer relay consumer 

clean:
	rm -f producer
	rm -f relay 
	rm -f consumer
