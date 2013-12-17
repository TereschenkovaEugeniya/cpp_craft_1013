#ifndef STATISTICS_H_
#define STATISTICS_H_

#include <map>
#include "../common/Message.h"

class Statistics {
public:
  Statistics() : start_time_(0) {}
  
  void set_start_time(boost::uint32_t start_time) {
    start_time_ = start_time;
  }
  
  void Reset();
  void IncrementCounter(const MessageType id);
  void DumpOnTime(std::ostream& os, boost::uint32_t t);

private:
  boost::uint32_t start_time_;
  std::map<MessageType, unsigned> counters_;
};

#include "Statistics-inl.h"

#endif // STATISTICS_H_
