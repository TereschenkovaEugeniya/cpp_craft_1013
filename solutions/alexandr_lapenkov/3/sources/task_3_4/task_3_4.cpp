#include <iostream>
#include <math.h>
#include <algorithm>
#include <fstream>
#include <vector>
#include <string>
#include "readlib.h"
#include "writelib.h"
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;

class Solution
{ 
	int file_id;
	static const int thread_cnt = 4;
	static const int files_cnt = 1000;
	static const string prefix;
	boost::mutex mtx;
       
	struct Data
	{
		unsigned type,time,len;
		string msg;
		
		friend binreader& operator>>(binreader&, Data&);
		friend binwriter& operator<<(binwriter&, const Data&);                                 
	
	};

	friend binreader& operator>>(binreader& in, Data& a)
	{
		in>>a.type>>a.time>>a.len;
			a.msg = in.get_line(a.len);	
		return in;
	}

	friend binwriter& operator<<(binwriter& out, const Data& a)
	{
		out<<a.type<<a.time<<a.len;
		out.write_line(a.msg.c_str(),a.len);

		return out;
	}

public:

       Solution()
       {}
            
       ~Solution()
       {}         

	   string get_filename(const string& type, const int _file_id)
	   {
		   char c[4];
		   sprintf(c,"%03d",_file_id);
		   return prefix+type+"_"+boost::lexical_cast<string>(c)+".txt";
	   }


	   void run()
	   {
		   boost::thread_group t;
		   file_id = 0;

		   for(int i = 0;i < thread_cnt; ++i)
			   t.create_thread( boost::bind(&Solution::process, this) );

		   t.join_all();
	   }
       
       void process()
       {
		   binreader in;
		   binwriter out;

		   {
			   boost::mutex::scoped_lock lock(mtx);
			
			   while(!in.is_open()&&file_id<files_cnt)
			   {
				   in.open(get_filename("input",file_id).c_str());
				   file_id++;
			   } 

			   if(!in.is_open())return ;
			   else out.open(get_filename("output",file_id-1).c_str());
		   }

		   Data x;
		   bool f=0;
		   long long max_t;
		   vector<Data>a;

		   while(in.good())
		   {
			   in>>x;
			   if(!in.good())break;
			   if(x.type<1u||x.type>4u)continue;

			   if(!f)
			   {
				   max_t = (long long)x.time;
				   a.push_back(x);
				   f=1;
			   }
			   else
			   {
				   if((long long)x.time>(max_t-2))
					   a.push_back(x);
				   max_t=max(max_t,(long long)x.time);
			   }
				
		   }
		   
		   for(vector<Data>::iterator it = a.begin(); it<a.end(); it++)
			   out<<it->type<<it->time<<it->len<<it->msg;

		   in.close();
		   out.close();
	   }	
            
};

const string Solution::prefix = SOURCE_DIR"/";

int main()
{
	Solution s;
	s.run();

	return 0;  
}
