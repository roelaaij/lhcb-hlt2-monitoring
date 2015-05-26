CXX=g++
CXXFLAGS=-O3 -Werror -Wall -pedantic -std=c++14
LDLIBS+=`pkg-config --libs libzmq`
LDLIBS+=-lboost_serialization

all: producer consumer relay

clean:
	rm -f producer
	rm -f consumer
