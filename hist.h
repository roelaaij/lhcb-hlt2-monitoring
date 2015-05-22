#ifndef HIST_H
#define HIST_H

#include <ostream>
#include <vector>

#include <boost/serialization/vector.hpp>

class Hist {
 public:
  using index_t = std::size_t;
  using tck_t = unsigned;
  using run_number_t = unsigned long long;
  using bin_t = unsigned long;
  using bins_t = std::vector<bin_t>;  // use vector because we'd need to null
                                      // the array anyway
  static const index_t PERIOD = 60u * 70u;

  Hist(tck_t tck, run_number_t run_number) noexcept : tck_{tck},
                                                      run_number_{run_number},
                                                      bins_(PERIOD) {}
  Hist() noexcept : Hist(0, 0) {}

  // Getters
  auto tck() const noexcept -> tck_t { return tck_; }
  auto run_number() const noexcept -> run_number_t { return run_number_; }
  auto bins() const noexcept -> const bins_t & { return bins_; }
  auto bins() noexcept -> bins_t & { return bins_; }

  // Operators
  /** Access bin. Returns last bin (overflow bin) if index is too large.
   */
  auto operator[](index_t timeBin) noexcept -> bin_t & {
    if (timeBin >= PERIOD) {
      timeBin = PERIOD - 1u;
    }
    return bins_[timeBin];
  }

  /** Compare for equality.
   */
  auto operator==(const Hist& rhs) const noexcept -> bool {
    if (this == &rhs) {
      return true;
    }
    return (tck_ == rhs.tck_) && (run_number_ == rhs.run_number_) &&
           (bins_ == rhs.bins_);
  }
  /** Compare for inequality.
   * Uses operator== internally.
   */
  auto operator!=(const Hist& rhs) const noexcept -> bool {
    return !(*this == rhs);
  }

  // Modifiers
  /** Increment bin.
   * TODO: Does this need to be atomic?
   */
  auto inc(index_t index) noexcept -> void { ++(*this)[index]; }

  /** Add value to bin.
   * TODO: Is this actually used?
   */
  auto add(index_t index, bin_t value) noexcept -> void {
    (*this)[index] += value;
  }

  friend class boost::serialization::access;
  /** Enable serialisation via boost::serialize
   */
  template <typename Archive>
  auto serialize(Archive& ar, const unsigned int version) -> void {
    ar& tck_;
    ar& run_number_;
    ar& bins_;
  }

 private:
  tck_t tck_;
  run_number_t run_number_;
  bins_t bins_;
};

/** Pretty output that contains _some_ information.
 */
auto operator<<(std::ostream& os, const Hist& hist) -> std::ostream & {
  os << "Hist for run " << hist.run_number() << " (TCK: " << hist.tck() << ')';
  return os;
}

#endif  // HIST_H
