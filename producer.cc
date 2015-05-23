#include "monitoring.pb.h"

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <random>
#include <sstream>
#include <thread>

#include <zmq.hpp>

template <typename RNG>
auto randomChunk(RNG& rng) -> monitoring::Chunk {
  using int32 = ::google::protobuf::int32;
  static std::uniform_int_distribution<int32> dist_run(31335, 31338);
  static std::uniform_int_distribution<int32> dist_tck(12345, 12347);
  static std::uniform_int_distribution<int32> dist_id(0, 10);
  static std::uniform_int_distribution<int32> dist_start(0, 3600);
  static std::uniform_int_distribution<int32> dist_bin(0, 50);

  auto c = monitoring::Chunk{};
  c.set_runnumber(dist_run(rng));
  c.set_tck(dist_tck(rng));
  c.set_id(dist_id(rng));
  c.set_start(dist_start(rng));
  for (auto i = 0u; i < 10; ++i) {
    c.add_data(dist_bin(rng));
  }
  return c;
}

template <typename RNG>
auto publishRandomChunk(RNG& rng, zmq::socket_t& socket) -> void {
  using namespace std::literals;
  auto end = std::chrono::high_resolution_clock::now() + 10s;  // temp

  auto c = randomChunk(rng);

  std::string buf;
  c.SerializeToString(&buf);

  zmq::message_t msg(buf.length());
  memcpy(msg.data(), buf.c_str(), buf.length());
  socket.send(msg);
  std::cout << "Sent\n";
  std::this_thread::sleep_until(end);
}

int main() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  std::random_device rd;
  std::mt19937 rng{rd()};

  auto context = zmq::context_t{1};
  auto socket = zmq::socket_t{context, ZMQ_PUB};
  socket.bind("tcp://*:31337");

  while (true) {
    publishRandomChunk(rng, socket);
  }
}
