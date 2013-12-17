#ifndef MESSAGES_BUFFER_H_
#define MESSAGES_BUFFER_H_

#include <map>
#include "Message.h"
#include <boost/noncopyable.hpp>

class MessagesBuffer : virtual protected boost::noncopyable {

public:
  MessagesBuffer();
  ~MessagesBuffer();

  bool Accept(const Message& m);
  void Clear();
  void Flush();

private:
  void ClearSpaceCounters();

private:
  std::map<MessageType, size_t> occupied_space_counters_;
};

#endif // MESSAGES_BUFFER_H_
