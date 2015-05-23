CXX=clang++
CXXFLAGS=-O3 -Werror -Wall -pedantic -std=c++14
CXXFLAGS+=`pkg-config --cflags protobuf`
LDLIBS+=`pkg-config --libs protobuf libzmq`

all: buffers producer

producer: monitoring.pb.cc producer.cc

buffers: monitoring.pb.cc monitoring_pb2.py

monitoring.pb.cc: monitoring.proto
	protoc --cpp_out=./ $^

monitoring_pb2.py: monitoring.proto
	protoc --python_out=./ $^

clean:
	rm -rf __pycache__/
	rm -f monitoring.pb.* 
	rm -f monitoring_pb2.* 
	rm -f producer
