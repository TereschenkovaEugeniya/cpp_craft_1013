#ifndef TASK_34_H_
#define TASK_34_H_

class Task34 {

public:
  Task34() : 
    start_file_number_(0), 
    number_of_files_(0) {}
  ~Task34() {}
  
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
};

#endif // TASK_34_H_