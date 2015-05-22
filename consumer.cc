#include "monitoring.h"

#include <sstream>

#include <boost/archive/text_iarchive.hpp>
#include <zmq.hpp>

int main() {
  zmq::context_t context(1);
  zmq::socket_t subscriber(context, ZMQ_SUB);
  subscriber.connect("tcp://localhost:31337");
  subscriber.setsockopt(ZMQ_SUBSCRIBE, "", 0);  

  for (auto i = 0u; i < 100u; ++i) {  // process 100 chunks for testing
    zmq::message_t msg;
    subscriber.recv(&msg);

    std::stringstream ss(static_cast<char*>(msg.data()));
    boost::archive::text_iarchive ia{ss};
    Monitoring::Chunk c;
    ia >> c;
    std::cout << c << std::endl;
  }
}
