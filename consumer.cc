#include "monitoring.h"

#include <sstream>
#include <chrono>
#include <vector>
#include <unordered_map>

#include <boost/archive/text_iarchive.hpp>
#include <zmq.hpp>

int main() {
  zmq::context_t context{1};
  zmq::socket_t sub{context, ZMQ_SUB};
  sub.bind("tcp://*:31338");
  sub.setsockopt(ZMQ_SUBSCRIBE, "", 0);

  std::unordered_map<Monitoring::HistId, Monitoring::Chunk> histograms;

  auto start = std::chrono::high_resolution_clock::now();

  int interval{10};

  while (true) {
    // Receive message
    zmq::message_t in_msg;
    sub.recv(&in_msg);

    // Deserialize
    std::stringstream ss{static_cast<char*>(in_msg.data())};
    boost::archive::text_iarchive ia{ss};
    Monitoring::Chunk c;
    ia >> c;
    std::cout << " received " << c << std::endl;

    // Add to internal store
    if (!histograms.count(c.histId)) {
      histograms[c.histId] = Monitoring::Chunk{c.runNumber, c.tck, c.histId, 0,
                                               c.start + c.data.size()};
    }
    histograms[c.histId] += c;

    // If not displayed for interval seconds, show all current histograms
    auto now = std::chrono::high_resolution_clock::now();
    if (std::chrono::duration_cast<std::chrono::seconds>(now - start).count() >
        interval) {
      for (const auto& h : histograms) {
        std::cout << h.second << std::endl;
      }
    }
  }
}
