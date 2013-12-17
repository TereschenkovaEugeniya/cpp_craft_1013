#include <algorithm>
#include "MessagesSorter.h"

using namespace std;

MessagesSorter::MessagesSorter() {}

MessagesSorter::~MessagesSorter() {}

void MessagesSorter::PutMessage(const Message& m) {
  boost::shared_ptr<MessagesContainer> pmc = GetMessagesContainer(m);
  pmc->PutMessage(m);
}
void MessagesSorter::DumpMessages() const {
  ConstContainersIterator it = containers_.begin();
  while (it != containers_.end()) {
    (*it).second->DumpContent();
    ++it;
  }
}
  
boost::shared_ptr<MessagesContainer> MessagesSorter::GetMessagesContainer(const Message& m) {
  ContainersIterator it = containers_.find(m.stock_name());
  if (it != containers_.end())
    return it->second;

  boost::shared_ptr<MessagesContainer> pmc(new MessagesContainer(m.stock_name()));
  it = containers_.insert(make_pair(m.stock_name(), pmc)).first;
  return it->second;
}
