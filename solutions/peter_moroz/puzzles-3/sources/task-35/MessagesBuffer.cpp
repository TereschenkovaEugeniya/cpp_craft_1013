#include "Restrictions.h"
#include "MessagesBuffer.h"

using namespace std;

MessagesBuffer::MessagesBuffer() {}
MessagesBuffer::~MessagesBuffer() {}

bool MessagesBuffer::Accept(const Message& m) {
  size_t msg_size = m.GetTotalSize();
  if (occupied_space_counters_[m.type()] + msg_size 
      < restrictions::kBufferCapacity) {
    occupied_space_counters_[m.type()] += msg_size;
    // here put message into buffer, when it will be real project
    return true;
  }
  return false;
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
