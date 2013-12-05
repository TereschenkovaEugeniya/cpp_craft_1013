#include <exception>
#include <boost/shared_ptr.hpp>

#include "solution.h"

using namespace std;

namespace task4_5
{

FindMinMaxElementsInMatrix::FindMinMaxElementsInRow::FindMinMaxElementsInRow(
        FindMinMaxElementsInMatrix* parent, const row_type& row) 
    : parent_(parent), row_(row) { assert(parent != NULL); }

void FindMinMaxElementsInMatrix::FindMinMaxElementsInRow::Run() {
  int minimal_element = numeric_limits<int>().max();
  int maximal_element = numeric_limits<int>().min();
  for (size_t i = 0; i < row_.size(); ++i) {
    if (row_[i] < minimal_element)
      minimal_element = row_[i];
    if (row_[i] > maximal_element)
      maximal_element = row_[i];
  }

  parent_->PutRowResults(make_pair(minimal_element, maximal_element));
}


FindMinMaxElementsInMatrix::FindMinMaxElementsInMatrix(const matrix_type& matrix)
   : complete_(false), minimal_element_(0), 
     maximal_element_(0), matrix_(matrix) {}

void FindMinMaxElementsInMatrix::Perform() {
  if (matrix_.size() == 0) {
	  complete_ = true;
	  return;
  }

  boost::unique_lock<boost::mutex> lock(instance_guard_);

  Reset();

  vector<RowProcessorPtr> row_processors;
  for (size_t i = 0; i < matrix_.size(); ++i) {
	  RowProcessorPtr row_processor(new FindMinMaxElementsInRow(this, matrix_[i]));
	  row_processors.push_back(row_processor);
	  thg_.create_thread(boost::bind(&FindMinMaxElementsInRow::Run, row_processor));
  }
  
  thg_.join_all();
  {
     boost::mutex::scoped_lock lock(rows_results_guard_);
     for (size_t i = 0; i < rows_results_.size(); ++i) {
       if (rows_results_[i].first < minimal_element_)
         minimal_element_ = rows_results_[i].first;
       if (rows_results_[i].second > maximal_element_)
         maximal_element_ = rows_results_[i].second;
     }
  }

  complete_ = true;
  ready_.notify_all();
}
int FindMinMaxElementsInMatrix::GetMinElement() const {
  boost::unique_lock<boost::mutex> lock(instance_guard_);
  while (!complete_)
	ready_.wait(lock);
  return minimal_element_;
}
int FindMinMaxElementsInMatrix::GetMaxElement() const {
  boost::unique_lock<boost::mutex> lock(instance_guard_);
  while (!complete_)
	ready_.wait(lock);
  return maximal_element_;
}

void FindMinMaxElementsInMatrix::PutRowResults(const pair<int, int>& rr) {
  boost::mutex::scoped_lock lock(rows_results_guard_);
  rows_results_.push_back(rr);
}

void FindMinMaxElementsInMatrix::Reset() {
  complete_ = false;
  rows_results_.clear();
  minimal_element_ = numeric_limits<int>().max();
  maximal_element_ = numeric_limits<int>().min();
}

}

// solution class implementation
task4_5::solution::solution( const data_type& data ) 
  : data_processor_(data)
{
	data_processor_.Perform();
}

int task4_5::solution::get_min() const
{
	int data_element = 0;
	try {
		data_element = data_processor_.GetMinElement();
	} catch (exception& ex) {
		cerr << "solution::get_min() - exception has been caught."
			<< " reason: " << ex.what() << endl;
	}
	return data_element;
}
int task4_5::solution::get_max() const
{
	int data_element = 0;
	try {
		data_element = data_processor_.GetMaxElement();		
	} catch (exception& ex) {
		cerr << "solution::get_min() - exception has been caught."
			<< " reason: " << ex.what() << endl;
	}
	return data_element;
}
