#include "hist.h"

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <random>
#include <string>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <zmq.hpp>

int main() {
  //auto context = zmq::context_t{0};
  //auto socket = zmq::socket_t{context, ZMQ_REQ};
 
  std::random_device rd;
  auto rng = std::mt19937{rd()};
  auto dist = std::uniform_int_distribution<Hist::bin_t>{0u, 10000u};

  // Fill hist with random values
  auto hist = Hist{123, 123456};
  auto& bins = hist.bins();
  std::generate(std::begin(bins), std::end(bins),
                [&rng, &dist]() { return dist(rng); });

  { // Write out
    std::ofstream ofs{"serialized.txt"};
    boost::archive::text_oarchive oa{ofs};
    oa << hist;
  }

  Hist inHist;
  { // Read back in
    std::ifstream ifs{"serialized.txt"};
    boost::archive::text_iarchive ia{ifs};
    ia >> inHist;
  }
  std::cout << inHist << '\n';

  assert(hist == inHist);
}
