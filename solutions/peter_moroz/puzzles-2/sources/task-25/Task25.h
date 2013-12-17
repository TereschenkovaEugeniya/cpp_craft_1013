#ifndef TASK_25_H_
#define TASK_25_H_

#include "../common/Task.h"
#include "MessagesBuffer.h"
#include "Statistics.h"

class Task25 : public Task {

  static const unsigned kMaximalAllowedMessageType = 100000;

public:
  Task25(const char* input_fname, const char* output_fname)
    : Task(input_fname, output_fname) { 
        messages_buffer_.set_statistics(&statistics_);
      }
  ~Task25() {}
  
  virtual void Perform();
  virtual void Initialize();

private:
  MessagesBuffer messages_buffer_;
  Statistics statistics_;
};

#endif // TASK_25_H_