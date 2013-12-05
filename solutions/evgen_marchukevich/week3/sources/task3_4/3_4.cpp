#include <iostream>
#include <fstream>
#include <cstdio>
#include <math.h>
#include <string>
#include <cstring>
#include <list>
#include <queue>
#include <set>
#include <map>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include "BinRW.h"
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;


class Solution
{

private:
	
	static int file_id;
	static const int file_cnt = 999;
	static const size_t threads_count = 4;	
	static const string input_prefix;
	static const string output_prefix;
	bool error;
	boost :: mutex Mutex;

	bool check(const BinRW ::msg &t, const unsigned int max,const bool first)
		{
			if (t.type<1 || t.type>4) return 0;
			if (first) return 1;
			if (t.time+2 <=max) return 0;
			return 1;
		}

	
	string get_in_file(const int id)
		{
			char s[4];
			sprintf(s,"%03d",id);			
			return input_prefix+boost :: lexical_cast<string>(s)+".txt";
		}

	string get_out_file(const int id)
		{
			char s[4];
			sprintf(s,"%03d",id);			
			return output_prefix+boost :: lexical_cast<string>(s)+".txt";
		}

public:

	Solution()
	{
	}

	~Solution()
	{		
	}


	
	void flows()
	{		
			boost::thread_group threads_group;
			for( size_t i = 0; i < threads_count; ++i )
			{
				threads_group.create_thread( boost::bind( &Solution::process, this ));
			}			
			threads_group.join_all();
	}

	void process()
		{
			while (file_id<=file_cnt)
			{
				BinRW RR;
				bool open_file=false;
				{				
					boost :: mutex :: scoped_lock lock(Mutex);
					while(file_id<=file_cnt)
					{
						RR.BinOpen_inFile((SOURCE_DIR+get_in_file(file_id)).c_str());								
						if (!RR.Bin_isOpen_inFile())
							{						
								file_id++;
								continue;
							}
						RR.BinOpen_outFile((SOURCE_DIR+get_out_file(file_id)).c_str());						
						file_id++;
						open_file=true;
						break;
					}
				}
				if (!open_file) return;
				unsigned int T = 0;
				bool first = true;
				vector<BinRW :: msg> ans;						
				ans.clear();
				while (1)
				{
					BinRW :: msg x;
					RR.BinReader(x);
					if (!RR.Bin_nice()) break;
					if (!check(x,T,first)) continue;
					first = false;
					ans.push_back(x);
					T=max(T,x.time);
				}		
					for(vector<BinRW :: msg> :: iterator it=ans.begin();it!=ans.end();it++)
						RR.BinWriter(*it);						
					RR.BinClose_in();
					RR.BinClose_out();
			}
		}
	

};


int Solution :: file_id = 1;
const string Solution :: input_prefix =  "/input_";
const string Solution :: output_prefix =  "/output_";

int main()
{
	Solution x;
	x.flows();	
	return 0;
}