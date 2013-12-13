#ifndef _BINARY_DATAFEED_
#define _BINARY_DATAFEED_
#include <fstream>
#include <map>
#include <vector>
#include "stock_data.h"
#include "boost\thread.hpp"
#include <boost/shared_ptr.hpp>
#include <queue>

using namespace std;
typedef binary_reader::stock_data stock_data;
typedef boost::shared_ptr<binary_reader::stock_data> shared_stock;
typedef queue<boost::shared_ptr<binary_reader::stock_data>> queue_of_pointers;
typedef boost::shared_ptr<queue_of_pointers> shared_queue;
class processor{
	string num;
	boost::mutex& que_mtx;
	bool& finish; // end of input file reached

	shared_queue que_;

public:
	processor(string name, boost::mutex & queue_mut, shared_queue& que, bool & fin): que_mtx(queue_mut), que_(que), finish(fin)												// "YZ"			
	{
		num = name;
	}
	~processor()
	{
		boost::mutex::scoped_lock lock(que_mtx);
		while(!que_->empty())
			que_->pop();
	}
	int process_data();
	void operator() ()
	{
		process_data();
	}
};

class Datafeed
{
	ifstream inp;
	ofstream ofp;
	bool fileErr;
	typedef map<string, shared_queue> map_stock;
	map_stock tasks;

public:
	Datafeed()
	{
		fileErr = false;
		stringstream ss;
		ss << BINARY_DIR << "/" << "input.txt";
		string in_name = ss.str();
		inp.open(in_name.c_str(), fstream::in | fstream::binary);
		
		if(!inp.is_open() )
		{
			cout << "File not found! "<<endl;
			fileErr = true;
			return;
		}
	}
	int run();
	int operator() ()
	{

	}
	~Datafeed()
	{

	}
};
#endif