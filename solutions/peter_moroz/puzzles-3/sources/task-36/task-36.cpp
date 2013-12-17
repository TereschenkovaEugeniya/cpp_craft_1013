#include <iostream>
#include <exception>
#include "Task36.h"

using namespace std;

static const char* kInputFilename = "Input.txt";

int main() {

  Task36 task(kInputFilename);

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
