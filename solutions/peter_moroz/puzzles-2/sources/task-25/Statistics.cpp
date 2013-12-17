#include "Statistics.h"
#include <iostream>
#include <sstream>

using namespace std;

void Statistics::DumpOnTime(ostream& os, boost::uint32_t t) {
  stringstream text;
  if (t < start_time_) {
    text << "Statistics::DumpOnTime : passed argument " 
      << t << " is less than start_time, start_time = "
      << start_time_ << endl;
    throw invalid_argument(text.str());
  }
  boost::uint32_t duration = t - start_time_ + 1;
  map<MessageType, unsigned>::iterator it = counters_.begin();
  while (it != counters_.end()) {
    MessageType type = it->first;
    double avg = static_cast<double>(it->second) / duration;

    os.write(reinterpret_cast<const char*>(&type), sizeof(type));
    os.write(reinterpret_cast<const char*>(&avg), sizeof(avg));

    ++it;
  } // while (it != counters_.end())
}
