#include <stock_data.h>
#include "Trade.h"
#include "boost/thread.hpp"
#include <boost/lexical_cast.hpp>
#include <vector>

class processor{
	string& num;
public:
	processor(string& name):
	  num (name)  // "YZ" or "Z" or "XYZ"
	{
	}
	~processor()
	{
	}
	int process_data()
	{
		stringstream input;
		stringstream output;
		if(num.size() == 1)
		{
			input << BINARY_DIR << "/input_00" << num << ".txt";
			output << BINARY_DIR << "/ouput_00" << num << ".txt";
		}
		else if (num.size() == 2 )
		{
			input << BINARY_DIR << "/input_0" << num << ".txt";
			output << BINARY_DIR << "/ouput_0" << num << ".txt";
		}
		else if (num.size() == 3)
		{
			input << BINARY_DIR << "/input_" << num << ".txt";
			output << BINARY_DIR << "/ouput_" << num << ".txt";
		}
		else {
			cout<< "Your changing part of name has wrong size"; 
			return -1;
		}
		ifstream inp( input.str().c_str(), fstream::binary | fstream::in);
		ofstream outp;
		if( inp.is_open() )
		{
			outp.open(output.str().c_str(), fstream::binary | fstream::out);
		}
		else 
		{
			std::cout << input.str() << " File not found! "<<endl;
			return 1;
		}
		
		uint32_t currentTime = 0;
		bool toWrite = false;
		Trade trade;
		while (!inp.eof())
		{
			if( trade.read(inp) == -1 )
				break;
			toWrite = (trade.typ>=MARKET_OPEN && trade.typ<=MARKET_CLOSE);
			if(currentTime != 0) //test of first read
			{
				if (trade.time <= currentTime-2)
				{
					toWrite |= false;
				}
				else 
				{
					toWrite |= true;
					currentTime = trade.time;
				}

			}
			else
			{
				toWrite |= true;
				currentTime = trade.time;
			}
			if(trade.len > 1000)
				break;
			if (toWrite)
			{
				trade.write(outp);
			}
			if( inp.peek() == EOF )
			{
				break;
			}
		}
		inp.close();
		outp.close();
		return 0;
	}
	void operator() ()
	{
		process_data();
	}
};
int main()
{
	size_t quantity = 3; //your number of files, from 0
	vector<string*> nums;
	boost::thread_group thr_gr;
	for(size_t i = 0; i < quantity; i++)
	{
		nums.push_back (new string(boost::lexical_cast<string>(i) ) );
		processor process (*nums[i]);
		thr_gr.create_thread(process);
	}
	thr_gr.join_all();
}

