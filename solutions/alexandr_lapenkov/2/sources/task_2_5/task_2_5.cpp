#include <iostream>
#include <math.h>
#include <algorithm>
#include <fstream>
#include <vector>
#include <string>
#include <boost/unordered_map.hpp>
#include "readlib.h"
#include "writelib.h"

using namespace std;

class Solution
{ 
	static const unsigned int buffer_size = 2048u;

	binreader in;
	binwriter out;
       
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
       {
		   in.open(SOURCE_DIR"/input.txt");
		   out.open(SOURCE_DIR"/output.txt");
		   if(!in.is_open()||!out.is_open())
			   throw logic_error("Input or output can't be opened");
       }
            
       ~Solution()
       {
		   in.close();
		   out.close();
       }                 	
       
       void process()
       {
		   boost::unordered_map<unsigned, size_t> takes;
		   boost::unordered_map<unsigned, int> saved, diff, last;
		   Data x;
		   
		   in>>x;
		   if(x.getSize()<=buffer_size)
		   {
		      takes[x.type]+=x.getSize();
			  saved[x.type]++;
		   }
		   else saved[x.type] = 0;
		   diff[x.type] = 1;
		   last[x.type] = x.time;

		   while(in.good())
		   {
			   in>>x;
			   if(!in.good())break;

			   size_t size = x.getSize();
			   if(takes.count(x.type))
				   size+=takes[x.type];
			   
			   if(!last.count(x.type)||x.time!=last[x.type])
			   {
				   takes[x.type] = 0;
				   diff[x.type]++;
			   }

			   if(size<=buffer_size)
			   {
				   takes[x.type]+=size;
				   saved[x.type]++;
			   }
			   
			   last[x.type] = x.time;
			   if(!saved.count(x.type))saved[x.type] = 0;
		   }
		   for(boost::unordered_map<unsigned, int>::iterator it = saved.begin();it!=saved.end();it++)
			   out<<it->first<<double(it->second)/diff[it->first];
	   }	
            
};


int main()
{
	try
	{
		Solution s;
		s.process();
	}catch(exception& e)
	{
		cerr<<e.what();
	}
	
	return 0;  
}
