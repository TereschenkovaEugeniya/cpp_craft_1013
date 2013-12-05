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

private :	

		map<unsigned int,unsigned int> kol,u;		
		map<unsigned int,double> ans;
		static int file_id;
		static const int file_cnt = 999;
		static const size_t threads_count = 4;		
		static const string input_prefix;
		static const size_t buffer_size = 2048u;
		vector<unsigned int> all_types;
		BinRW RR;
		boost :: mutex Mutex,mtx;

		void writer()
		{
			sort(all_types.begin(),all_types.end());
			vector<unsigned int> :: iterator it;
			for(it=all_types.begin();it!=all_types.end();it++)
			{
				unsigned int TYPE = *it;
				RR.BinWriter(TYPE);
				
				double MID_CNT=0;
				if (kol[*it]!=0) MID_CNT=ans[*it]/(1.0*kol[*it]);
				
				RR.BinWriter(MID_CNT);
			}

		}

		size_t get_msg_size(const BinRW :: msg &x)
		{
			return (sizeof(x.time)+sizeof(x.type)+sizeof(x.len)+sizeof(char)*x.len);
		}		
		
		string get_in_file(const int id)
		{
			char s[4];
			sprintf(s,"%03d",id);			
			return input_prefix+boost :: lexical_cast<string>(s)+".txt";
		}



public :

		void flows()
		{
			
				boost::thread_group threads_group;
				for( size_t i = 0; i < threads_count; ++i )
					threads_group.create_thread( boost::bind( &Solution::process, this ));
				threads_group.join_all();
			RR.BinOpen_outFile(SOURCE_DIR"/output.txt");
			writer();
			RR.BinClose_out();
		}

		Solution()
		{
		}

		~Solution()
		{			
		}


		void process()
		{
			while(file_id<=file_cnt )
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
						file_id++;
						open_file=true;
						break;
					}
				}
				
				if (!open_file) return;
				map<unsigned int,size_t> BUFFER;
				map<unsigned int,unsigned int> TIME;			
				while (1)
				{
					BinRW :: msg x;
					RR.BinReader(x);			
					if (!RR.Bin_nice()) break;
					if (TIME.count(x.type)==0 || TIME[x.type]!=x.time)				
					{					
						BUFFER[x.type]=0;
						if (get_msg_size(x) <= buffer_size)
						{
							boost :: mutex :: scoped_lock lock(mtx);
							kol[x.type]++;
						} else
							TIME[x.type]=0;
					}
					if (BUFFER[x.type]+get_msg_size(x) <= buffer_size)
					{
						boost :: mutex :: scoped_lock lock(mtx);
						TIME[x.type]=x.time;
						BUFFER[x.type]+=get_msg_size(x);
						u[x.type]++;
						if (u[x.type]==1)
							all_types.push_back(x.type);				
						ans[x.type]++;
					}				
				}
				RR.BinClose_in();
			}
		}
					
};


int Solution :: file_id = 1;
const string Solution :: input_prefix = "/input_";


int main()
{
		Solution s;
		s.flows();
		return 0;
}