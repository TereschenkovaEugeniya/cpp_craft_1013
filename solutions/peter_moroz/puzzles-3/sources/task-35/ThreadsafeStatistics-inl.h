#ifndef THREADSAFE_STATISTICS_INL_H_
#define THREADSAFE_STATISTICS_INL_H_

inline void ThreadsafeStatistics::CheckAndCorrectTimeRange(boost::uint32_t t) {
  boost::mutex::scoped_lock lock(timerange_guard_);
  if (t <= start_time_) {
    start_time_ = t;
    return;
  }
  if (t > finish_time_)
    finish_time_ = t;
}

inline void ThreadsafeStatistics::IncrementCounter(MessageType id) {
  boost::mutex::scoped_lock lock(counters_guard_);
  ++counters_[id];
}

inline void ThreadsafeStatistics::Reset() {
  {
    boost::mutex::scoped_lock lock(timerange_guard_);
    start_time_ = 0;
    finish_time_ = 0;
  }

  {
    boost::mutex::scoped_lock lock(counters_guard_);
    counters_.clear();
  }
}

#endif // THREADSAFE_STATISTICS_INL_H_
