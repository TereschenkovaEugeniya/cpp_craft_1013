#include "small_buffer.h"
#include <boost/lexical_cast.hpp>

size_t processor::count = 0;
const UINT32 processor::TotalTypes = 10000; //100000
int main()
{
	size_t quantity = 4;
	vector<string*> nums;
	boost::thread_group thr_gr;
	boost::mutex total_mut;
	boost::mutex max_mut , count_mut;
	messages_t total_msgs;
	vector<processor*> processes;
	processes.reserve(quantity);
	total_msgs.assign(processor::TotalTypes + 1, 0);
	for(size_t i = 0; i < quantity; i++)
	{
		nums.push_back (new string(boost::lexical_cast<string>(i) ) );
		processes.push_back(new processor(*nums[i], total_mut, total_msgs, max_mut, count_mut));
	}
	for(size_t k = 0; k < quantity; k++)
	{
		thr_gr.create_thread(*processes[k]);
	}
	thr_gr.join_all();
	processor::output(total_msgs);
	for(vector<processor*>::iterator it = processes.begin(); it != processes.end(); it++)
	{
		delete *it;
	}
	processes.clear();

}


int processor::process_data()
{
#pragma region 123
	stringstream input;
	if(num.size() == 1)
	{
		input << BINARY_DIR << "/input_00" << num << ".txt";
	}
	else if (num.size() == 2 )
	{
		input << BINARY_DIR << "/input_0" << num << ".txt";
	}
	else if (num.size() == 3)
	{
		input << BINARY_DIR << "/input_" << num << ".txt";
	}
	else {
		cout<< "Your changing part of name has wrong size"; 
		return -1;
	}
#pragma endregion 123

	ifstream inp( input.str().c_str(), fstream::binary | fstream::in);
	if(!inp.is_open())
	{
		cout << "File not found! "<<endl;
		return 1;
	}

	int n = 0; //total number of objects
	UINT32 cur_time = 0;
	char* str = NULL;
	bool toWrite = false;
	Trade trade;
	file_msgs.assign(TotalTypes + 1, 0);
	while (inp)
	{
		if( inp.peek() == EOF )
		{
			break;
		}
		trade.read(inp);
		if(trade.time > cur_time)
		{
			cur_time = trade.time;
			{
				boost::mutex::scoped_lock lock(max_mut);
				if(max_time < cur_time)
				{
					max_time = cur_time;
				}
			}
			/*{
				boost::mutex::scoped_lock lock(total_mut);
				for(vector<size_t>::iterator it = file_msgs.begin(), tb = file_msgs.begin(); it != numBytes.end(); it++, tb++)
				{
					*tb += *it;
				}
			}*/
			numBytes.assign(TotalTypes + 1, 0);
		}		
		if ((trade.typ<TotalTypes)&&((numBytes[trade.typ] + trade.len + 3 * sizeof(UINT32)) <= 2048))
		{
			numBytes[trade.typ] += trade.len + 3 * sizeof(UINT32);
			file_msgs[trade.typ] ++;
		}
		n++;
		if((trade.len == 0) || (trade.typ > TotalTypes) )
		{
			continue;
		}
	}
	{
		boost::mutex::scoped_lock lock (total_mut);
		for(vector<size_t>::iterator it = file_msgs.begin(), msgs = total_messages.begin(); it != file_msgs.end(); ++it, ++msgs)
		{
			*msgs += *it;
		}

	}
	inp.close();
	return n;
}
size_t processor::max_time = 0;