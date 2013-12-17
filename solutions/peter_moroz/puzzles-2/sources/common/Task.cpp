#include "Task.h"

Task::~Task() {
  ifs_.close();
  ofs_.close();
}

Task::Task(const char* input_filename, const char* output_filename)
  : input_fname_(input_filename), output_fname_(output_filename) {}
