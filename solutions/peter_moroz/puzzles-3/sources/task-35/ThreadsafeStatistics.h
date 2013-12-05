#ifndef THREADSAFE_STATISTICS_H_
#define THREADSAFE_STATISTICS_H_

#include <map>
#include "Message.h"
#include <boost/thread.hpp>

class ThreadsafeStatistics : virtual protected boost::noncopyable {
public:
  ThreadsafeStatistics();
  ~ThreadsafeStatistics();
  

  void CheckAndCorrectTimeRange(boost::uint32_t t);
  void Reset();
  void IncrementCounter(MessageType id);
  void ReportAverageCounters(std::ostream& os);

private:
  boost::uint32_t start_time_;
  boost::uint32_t finish_time_;
  std::map<MessageType, unsigned> counters_;
  boost::mutex timerange_guard_;
  boost::mutex counters_guard_;
};

#include "ThreadsafeStatistics-inl.h"

#endif // THREADSAFE_STATISTICS_H_
