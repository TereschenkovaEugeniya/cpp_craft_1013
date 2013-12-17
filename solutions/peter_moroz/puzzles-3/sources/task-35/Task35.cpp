#include "Task35.h"
#include "Message.h"
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

void Task35::Initialize() {
  workers_.clear();

  statistics_.Reset();

  unsigned finish_file_number = start_file_number_;
  finish_file_number += number_of_files_;

  unsigned curr_file_number = start_file_number_;
  while (curr_file_number < finish_file_number) {
    stringstream msg;
    stringstream file_number;

    file_number << setw(3) << setfill('0') << curr_file_number;
    ++curr_file_number;

    string in_fname = "Input_";
    in_fname.append(file_number.str());
    in_fname.append(".txt");

	  try {
	    boost::shared_ptr<MessagesProcessor> pmp(new MessagesProcessor());
	    pmp->set_in_fname(in_fname);
	    pmp->set_statistics(&statistics_);
	    workers_.push_back(pmp);
	  } catch (const bad_alloc& ex) {
	    cout << "Task35::Initialize(): Insufficient memory. "
		    << "Details: " << ex.what() << endl;
	    ++curr_file_number;
	    continue;
	  } // try , catch

    ++curr_file_number;
  }
}

void Task35::Perform() {

  static const char* kOutputFilename = "Output.txt";

  cout << "Task35::Perform has started. " << endl;

  for (size_t i = 0; i < workers_.size(); ++i)
    workgroup_.create_thread(boost::bind(&MessagesProcessor::Workflow, workers_[i]));

  workgroup_.join_all();

  ofstream ofs;
  ofs.open(kOutputFilename, ios_base::binary);
  if (!ofs.is_open()) {
    cout << "Can't open file " << kOutputFilename << endl;
    return;
  }
  statistics_.ReportAverageCounters(ofs);

  cout << "Task35::Perform has finished.\n";
}
