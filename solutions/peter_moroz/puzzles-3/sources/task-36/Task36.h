#ifndef TASK_36_H_
#define TASK_36_H_

#include <fstream>
#include <string>

class Task36 {

public:
  Task36(const char* input_filename);
  ~Task36();
  
  void Perform();
  void Initialize();

private:
  std::ifstream ifs_;
  std::string in_filename_;

private:
  Task36(const Task36&);
  const Task36& operator=(const Task36&);
};

#endif // TASK_36_H_
