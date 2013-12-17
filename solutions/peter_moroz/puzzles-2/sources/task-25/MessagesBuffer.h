#ifndef MESSAGES_BUFFER_H_
#define MESSAGES_BUFFER_H_

#include <map>
#include "../common/Message.h"

class Statistics;

class MessagesBuffer {
  static const size_t kBufferCapacity = 2048;

public:
  MessagesBuffer() : statistics_(NULL) {}
  ~MessagesBuffer() {}

  void PutMessage(const Message& m);
  void Clear();
  void Flush();
  void set_statistics(Statistics* statistics) {
    statistics_ = statistics;
  }

private:
  void ClearSpaceCounters();

private:
  Statistics* statistics_;
  std::map<MessageType, size_t> occupied_space_counters_;

private:
  MessagesBuffer(const MessagesBuffer&);
  const MessagesBuffer& operator=(const MessagesBuffer&);
};

#endif // MESSAGES_BUFFER_H_
