#ifndef MESSAGES_SORTER_H_
#define MESSAGES_SORTER_H_

#include <map>
#include <string>
#include <boost/shared_ptr.hpp>
#include "MessagesContainer.h"

class MessagesSorter {

  typedef 
    std::map<std::string, 
             boost::shared_ptr<MessagesContainer> >::iterator ContainersIterator;
  typedef 
    std::map<std::string, 
             boost::shared_ptr<MessagesContainer> >::const_iterator
                 ConstContainersIterator;

public:
  MessagesSorter();
  ~MessagesSorter();

public:
  void PutMessage(const Message& m);
  void DumpMessages() const;
  
private:
  boost::shared_ptr<MessagesContainer> GetMessagesContainer(const Message& m);

private: 
  std::map<std::string, boost::shared_ptr<MessagesContainer> > containers_;
}; // MessagesSorter

#endif // MESSAGES_SORTER_H_