#include "MessagesBuffer.h"
#include "Statistics.h"

using namespace std;

void MessagesBuffer::PutMessage(const Message& m) {
  size_t msg_size = m.GetTotalSize();
  if (occupied_space_counters_[m.type()] + msg_size < kBufferCapacity) {
    occupied_space_counters_[m.type()] += msg_size;
    // here put message into buffer, when it will be real project
    if (statistics_)
      statistics_->IncrementCounter(m.type());
  }
}

void MessagesBuffer::Clear() {
  ClearSpaceCounters();
}
void MessagesBuffer::Flush() {
  ClearSpaceCounters();
}
void MessagesBuffer::ClearSpaceCounters() {
  map<MessageType, size_t>::iterator it = occupied_space_counters_.begin();
  while (it != occupied_space_counters_.end()) {
    (*it).second = 0;
    ++it;
  }
}
