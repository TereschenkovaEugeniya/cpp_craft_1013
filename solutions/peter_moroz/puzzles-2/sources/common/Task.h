#ifndef TASK_H_
#define TASK_H_

#include <fstream>
#include <string>

class Task {

public:
  virtual ~Task();

protected:
  Task(const char* input_filename, const char* output_filename);

  virtual void Perform() = 0;
  virtual void Initialize() {}
  
protected:
  const std::string& input_fname() const { return input_fname_; }
  const std::string& output_fname() const { return output_fname_; }
  
protected:
  std::ifstream ifs_;
  std::ofstream ofs_;

private:
  std::string input_fname_;
  std::string output_fname_;

private:
  Task(const Task&);
  const Task& operator=(const Task&);
};

#endif // TASK_H_
