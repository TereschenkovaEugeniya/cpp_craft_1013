#ifndef _SMALL_BUFFER_
#define _SMALL_BUFFER_

#include <vector>
#include <stock_data.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Trade.h"
#include "boost/thread.hpp"

typedef unsigned int UINT32;
using namespace std;
typedef vector<size_t> messages_t;
class processor{
	string& num;
	boost::mutex& mut;
	static size_t count;

	boost::mutex& max_mut;
	static size_t max_time;
	messages_t numBytes; //used number of Bytes of idx type
	boost::mutex & total_mut;
	messages_t& total_messages;
	messages_t file_msgs;
	messages_t total_bytes; //total used Bytes

public:
	static const UINT32 TotalTypes;
	processor(string& name, boost::mutex & mut, messages_t& msgs, boost::mutex & max_m, boost::mutex& count_mut):num (name), total_mut(mut), total_messages(msgs), 
		max_mut(max_m) , mut(count_mut) 													// "YZ"			
	{
		{
			boost::mutex::scoped_lock lock(mut);
			count ++;
		}
		numBytes.resize(TotalTypes + 1, 0);
		file_msgs.resize(TotalTypes + 1, 0);
		total_bytes.resize(TotalTypes + 1, 0);
	}
	static int output(messages_t total_messages) //just for creating output
	{
		stringstream output;
		output << BINARY_DIR << "/output" << ".txt";
		ofstream outp( output.str().c_str(), fstream::binary | fstream::out);

		int type = 0;
		for(vector<size_t>::iterator tb = total_messages.begin(); tb != total_messages.end(); ++tb)
		{
			if (*tb == 0)
			{
				type++;
				continue;
			}
			binary_reader::write_binary(outp, type);
			double val = (double)*tb/max_time;
			binary_reader::write_binary(outp, val);
			type++;
		}
		outp.close();
		return type;
	}
	~processor()
	{
		{
			boost::mutex::scoped_lock lock(mut);
			count --;
			
		}

		numBytes.clear();
		file_msgs.clear();
		total_bytes.clear();
	}
	int process_data();
	void operator() ()
	{
		process_data();
	}
};
#endif