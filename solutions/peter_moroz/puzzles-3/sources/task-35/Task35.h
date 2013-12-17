#ifndef TASK_35_H_
#define TASK_35_H_

#include <iostream>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#include "ThreadsafeStatistics.h"
#include "MessagesProcessor.h"

class Task35 {

public:
  Task35() : 
    start_file_number_(0), 
    number_of_files_(0) {}
  ~Task35() {}

  void Initialize();  
  void Perform();

  void set_start_file_number(unsigned start_file_number) {
    start_file_number_ = start_file_number;
  }
  void set_number_of_files(unsigned number_of_files) {
    number_of_files_ = number_of_files;
  }

private:
  unsigned start_file_number_;
  unsigned number_of_files_;
  ThreadsafeStatistics statistics_;
  boost::thread_group workgroup_;
  std::vector<boost::shared_ptr<MessagesProcessor> > workers_;

private:
  Task35(const Task35&);
  const Task35& operator=(const Task35&);
};


#endif // TASK_35_H_
