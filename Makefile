CXX=clang++
CXXFLAGS=-O3 -Werror -Wall -pedantic -std=c++11
LDLIBS+=`pkg-config --libs libzmq`
LDLIBS+=-lboost_serialization

all: test

test: test.cc

clean:
	rm -f test
	rm -f serialized.txt
