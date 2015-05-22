#ifndef MONITORING_H
#define MONITORING_H

#include <algorithm>
#include <array>
#include <ostream>

#include <boost/serialization/array.hpp>

namespace Monitoring {
  using TCK = unsigned int;
  using HistId = unsigned int;
  using RunNumber = unsigned long;
  using BinContent = unsigned long long;
  using Seconds = std::size_t;
  static const Seconds DELTA_T = 10;  // 10 s spacing
  static const Seconds RUN_PERIOD = 70 * 60;  // 70 minute maximum run period

  /** A serialize-able chunk of data that spans DELTA_T seconds.
   */
  struct Chunk {
    RunNumber runNumber;
    TCK tck;
    HistId histId;
    Seconds start;
    std::array<BinContent, DELTA_T> data;
  };

  /** Write a chunk of data to a std::ostream.
   */
  auto operator<<(std::ostream& os, const Chunk& chunk) -> std::ostream & {
    os << "Monitoring::Chunk [Run: " << chunk.runNumber
       << ", TCK: " << chunk.tck << ", HistId: " << chunk.histId
       << ", Start: " << chunk.start << "]\n                  [";
    std::copy(std::begin(chunk.data), std::end(chunk.data),
              std::ostream_iterator<BinContent>(os, " "));
    os << ']';
    return os;
  }

  class Histogram {};
}

/** Serialize a chunk of data using boost::serialization.
 */
namespace boost {
namespace serialization {
template <typename Archive>
auto serialize(Archive& archive, Monitoring::Chunk& chunk,
               const unsigned int version) -> void {
  archive& chunk.runNumber;
  archive& chunk.tck;
  archive& chunk.histId;
  archive& chunk.start;
  archive& chunk.data;
}
}
}

#endif  // MONITORING_H
