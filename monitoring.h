#ifndef MONITORING_H
#define MONITORING_H

#include <algorithm>
#include <array>
#include <ostream>

#include <boost/serialization/vector.hpp>

namespace Monitoring {
using TCK = unsigned int;
using HistId = unsigned int;
using RunNumber = unsigned long;
using BinContent = unsigned long long;
using Seconds = std::size_t;

static const Seconds RUN_PERIOD = 70 * 60;

/** A serialize-able chunk of data that spans DELTA_T seconds.
 */
struct Chunk {
  Chunk() = default;

  Chunk(RunNumber rn, TCK t, HistId id, Seconds s, size_t n = 10)
      : runNumber{rn}, tck{t}, histId{id}, start{s}, data(n) {}

  Chunk& operator+=(const Chunk& c) {
    auto size = data.size();
    auto new_size = c.start + c.data.size();
    if (new_size > data.size()) {
      data.resize(new_size);
      for (unsigned int i = size; i < new_size; ++i) data[i] = 0;
    }
    for (unsigned int i = 0; i < c.data.size(); ++i)
      data[c.start + i] += c.data[i];
    return *this;
  }

  RunNumber runNumber;
  TCK tck;
  HistId histId;
  Seconds start;
  std::vector<BinContent> data;
};


/**
 */
struct Histogram {
  Histogram() = default;
  Histogram(RunNumber runNumber, TCK tck, HistId histId)
      : runNumber{runNumber}, tck{tck}, histId{histId}, data{} {}

  auto addChunk(const Chunk& c) noexcept -> void {
    assert(runNumber == c.runNumber && tck == c.tck && histId == c.histId);
    assert(c.start + c.data.size() < RUN_PERIOD);
    std::transform(std::begin(c.data), std::end(c.data),
                   std::begin(data) + c.start, std::begin(data) + c.start,
                   std::plus<>());
  }

  RunNumber runNumber;
  TCK tck;
  HistId histId;
  std::array<BinContent, RUN_PERIOD> data;
};


/** Write a chunk of data to a std::ostream.
 */
auto operator<<(std::ostream& os, const Chunk& chunk) -> std::ostream & {
  os << "Monitoring::Chunk [Run: " << chunk.runNumber << ", TCK: " << chunk.tck
     << ", HistId: " << chunk.histId << ", Start: " << chunk.start
     << "]\n                  [";
  std::copy(std::begin(chunk.data), std::end(chunk.data),
            std::ostream_iterator<BinContent>(os, " "));
  os << ']';
  return os;
}

/** Write histogram to a std::ostream.
 */
auto operator<<(std::ostream& os, const Histogram& h) -> std::ostream& {
  os << "Monitoring::Histogram [Run: " << h.runNumber << ", TCK: " << h.tck
     << ", HistId: " << h.histId << "]";
  return os;
}
}

/** Serialize a chunk of data using boost::serialization.
 */
namespace boost {
namespace serialization {
template <typename Archive>
auto serialize(Archive& archive, Monitoring::Chunk& chunk,
               const unsigned int version) -> void {
  archive& chunk.runNumber;
  archive& chunk.histId;
  archive& chunk.tck;
  archive& chunk.start;
  archive& chunk.data;
}
}
}

#endif  // MONITORING_H
