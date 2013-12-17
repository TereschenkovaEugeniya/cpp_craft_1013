#ifndef STATISTICS_INL_H_
#define STATISTICS_INL_H_

inline void Statistics::IncrementCounter(const MessageType id) {
  ++counters_[id];
}

inline void Statistics::Reset() {
  start_time_ = 0;
  counters_.clear();
}

#endif // STATISTICS_INL_H_
