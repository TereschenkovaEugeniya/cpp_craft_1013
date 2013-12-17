#include <iostream>
#include <math.h>
#include <algorithm>
#include <fstream>
#include <vector>
#include <string>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>
#include "readlib.h"
#include "writelib.h"

using namespace std;

class Solution
{ 
	static const unsigned buffer_size = 2048u;
	static const int thread_cnt = 4;
	static const int files_cnt = 1000;
	static const string prefix;
	int file_id;

	boost::mutex mtx;

	boost::unordered_map<unsigned, int>saved;
	boost::unordered_set<unsigned>diff;
       
	struct Data
	{
		unsigned int type,time,len;
		
		friend binreader& operator>>(binreader&, Data&); 

		size_t getSize()
		{
			return sizeof(int)*3+sizeof(char)*len;
		}
	
	};

	friend binreader& operator>>(binreader& in, Data& a)
	{
		in>>a.type>>a.time>>a.len;
		in.get_line(a.len);	
		return in;
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
		   binwriter out;
		   boost::thread_group t;
		   
		   file_id = 0;
		   out.open(SOURCE_DIR"/output.txt");

		   for(int i = 0 ;i < thread_cnt; ++i)
			   t.create_thread( boost::bind(&Solution::process, this) );
		   
		   t.join_all();

		   for(boost::unordered_map<unsigned, int>::iterator it = saved.begin();it!=saved.end();it++)
			   out<<it->first<<double(it->second)/diff.size();

   		   out.close();
	   }

       void process()
       {
		   binreader in;

		   {
			   boost::mutex::scoped_lock lock(mtx);
			
			   while(!in.is_open()&&file_id<files_cnt)
			   {
				   in.open(get_filename("input",file_id).c_str());

				   file_id++;
			   }  
			   
			   if(!in.is_open())return ;
		   }

		   boost::unordered_map<unsigned, size_t> takes;
		   boost::unordered_map<unsigned, int> last;
		   Data x;
		   
		   in>>x;
		   if(x.getSize()<=buffer_size)
		   {
		      takes[x.type]+=x.getSize();
			  {
				  boost::mutex::scoped_lock lock(mtx);
				  saved[x.type]++;
			  }
		   }
		   else saved[x.type] = 0;
		   {
			   boost::mutex::scoped_lock lock(mtx);
			   diff.insert(x.time);
		   }
		   last[x.type] = x.time;

		   while(in.good())
		   {
			   in>>x;
			   if(!in.good())break;

			   size_t size = x.getSize();
			   if(takes.count(x.type))
				   size+=takes[x.type];
			   
			   if(!last.count(x.type)||x.time!=last[x.type])
				   takes[x.type] = 0;

			   if(size<=buffer_size)
			   {
				   takes[x.type]+=size;
				   {
					   boost::mutex::scoped_lock lock(mtx);
					   saved[x.type]++;
				   }
			   }
			   
			   last[x.type] = x.time;
			   {
				   boost::mutex::scoped_lock lock(mtx);
				   diff.insert(x.time);
				   if(!saved.count(x.type))
					   saved[x.type] = 0;
			   }
		   }
		   in.close();
	   }	
            
};

const string Solution::prefix = SOURCE_DIR"/";

int main()
{
	Solution s;
	s.run();
	
	return 0;  
}
