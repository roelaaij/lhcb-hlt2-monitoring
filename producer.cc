#include "monitoring.h"

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <random>
#include <sstream>
#include <thread>

#include <boost/archive/text_oarchive.hpp>
#include <zmq.hpp>

template <typename RNG>
auto randomChunk(RNG& rng) -> Monitoring::Chunk {
  static std::uniform_int_distribution<Monitoring::TCK> dist_run(31335, 31338);
  static std::uniform_int_distribution<Monitoring::TCK> dist_tck(12345, 12347);
  static std::uniform_int_distribution<Monitoring::HistId> dist_id(0, 10);
  static std::uniform_int_distribution<Monitoring::Seconds> dist_start(0, 3600);
  static std::uniform_int_distribution<Monitoring::BinContent> dist_bin(0, 50);

  auto c = Monitoring::Chunk{dist_run(rng), dist_tck(rng), dist_id(rng),
                             dist_start(rng), 10};
  std::generate(std::begin(c.data), std::end(c.data),
                [&rng]() { return dist_bin(rng); });
  return c;
}

template <typename RNG>
auto publishRandomChunk(RNG& rng, zmq::socket_t& socket) -> void {
  using namespace std::literals;
  auto end = std::chrono::high_resolution_clock::now() + 1s;  // temp

  auto c = randomChunk(rng);
  // Serialize with boost
  std::stringstream ss;
  boost::archive::text_oarchive oa{ss};
  oa << c;
  auto s = ss.str();

  std::cout << s.size() << std::endl;
  // Prepare message and send
  zmq::message_t msg(s.size());
  memcpy(static_cast<void*>(msg.data()), s.c_str(), s.size());
  socket.send(msg);
  std::cout << c << std::endl;
  std::this_thread::sleep_until(end);
}

int main() {
  std::random_device rd;
  std::mt19937 rng{rd()};

  auto context = zmq::context_t{1};
  auto socket = zmq::socket_t{context, ZMQ_PUB};
  socket.connect("tcp://localhost:31337");

  while (true) {
    publishRandomChunk(rng, socket);
  }
}
