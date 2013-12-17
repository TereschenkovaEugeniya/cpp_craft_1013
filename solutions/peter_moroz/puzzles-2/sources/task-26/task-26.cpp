#include <iostream>
#include <exception>
#include "Task26.h"

using namespace std;

static const char* kInputFilename = "input.txt";
static const char* kOutputFilename = "output.txt";

int main() {

  Task26 task(kInputFilename, kOutputFilename);

  try {
    task.Initialize();
    task.Perform();
  } catch (const exception& ex) {
    cout << "An exception has been caught. "
      << "Reason: " << ex.what() << endl;
    return -1;
  }

  return 0;
}
