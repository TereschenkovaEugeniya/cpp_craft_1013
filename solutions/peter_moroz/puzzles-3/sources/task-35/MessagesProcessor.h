#ifndef MESSAGES_PROCESSOR_H_
#define MESSAGES_PROCESSOR_H_

#include <string>
#include "MessagesBuffer.h"
#include <boost/noncopyable.hpp>

class ThreadsafeStatistics;


class MessagesProcessor : virtual protected boost::noncopyable {

public:
  MessagesProcessor();
  ~MessagesProcessor();

  void set_in_fname(const std::string& in_fname) {
    in_fname_ = in_fname;
  }
  void set_statistics(ThreadsafeStatistics* statistics) {
    statistics_ = statistics;
  }

  void Workflow();

private:
  std::string in_fname_;
  ThreadsafeStatistics* statistics_;
  MessagesBuffer messages_buffer_;
};

#endif MESSAGES_PROCESSOR_H_