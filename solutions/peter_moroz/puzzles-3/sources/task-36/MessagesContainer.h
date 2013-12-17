#ifndef MESSAGES_CONTAINER_H_
#define MESSAGES_CONTAINER_H_

#include "Message.h"
#include <string>
#include <vector>
#include <iostream>

class MessagesContainer {

public:
  MessagesContainer(const std::string& name) : name_(name) {}
  ~MessagesContainer() {}
  
  const std::string& name() const { return name_; }
  void PutMessage(const Message& m) {
    messages_.push_back(m);
  }
  void DumpContent() const;  

private:
  std::vector<Message> messages_;
  std::string name_;
  static unsigned instances_counter_;
};

#endif // MESSAGES_CONTAINER_H_
