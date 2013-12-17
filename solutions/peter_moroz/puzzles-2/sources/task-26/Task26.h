#ifndef TASK_26_H_
#define TASK_26_H_

#include "../common/Task.h"

class Task26 : public Task {

public:
  Task26(const char* input_fname, const char* output_fname)
    : Task(input_fname, output_fname) {}
  ~Task26() {}
  
  virtual void Perform();
  virtual void Initialize();
};

#endif // TASK_26_H_