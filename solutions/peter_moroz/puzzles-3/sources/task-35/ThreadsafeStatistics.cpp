#include "ThreadsafeStatistics.h"
#include <iostream>

using namespace std;

ThreadsafeStatistics::ThreadsafeStatistics() 
  : start_time_(0), finish_time_(0) {}
ThreadsafeStatistics::~ThreadsafeStatistics() {}
  
void ThreadsafeStatistics::ReportAverageCounters(ostream& os) {
  boost::uint32_t duration = finish_time_ - start_time_ + 1;
  map<MessageType, unsigned>::iterator it = counters_.begin();
  while (it != counters_.end()) {
    MessageType type = it->first;
    double avg = static_cast<double>(it->second) / duration;

    os.write(reinterpret_cast<const char*>(&type), sizeof(type));
    os.write(reinterpret_cast<const char*>(&avg), sizeof(avg));

    ++it;
  } // while (it != counters_.end())
}
