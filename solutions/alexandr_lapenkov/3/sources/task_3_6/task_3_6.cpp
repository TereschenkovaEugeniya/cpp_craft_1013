#include <iostream>
#include <math.h>
#include <algorithm>
#include <fstream>
#include <vector>
#include <string>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>
#include "readlib.h"
#include "writelib.h"
#include <exception>

using namespace std;

class Solution
{ 
	
	static const string prefix;
	binreader in;
	binwriter out;
       
	struct Data
	{
		double f1,t1,f2,f3,f4;
		unsigned volume;
		string stock_name;
		string date_time;
		double price, vwap;

		
		friend binreader& operator>>(binreader&, Data&); 
		friend binwriter& operator<<(binwriter&, const Data&);
	
	};

	friend binreader& operator>>(binreader& in, Data& a)
	{
		a.stock_name = in.get_line(8);
		a.date_time = in.get_line(8);
		in>>a.price>>a.vwap>>a.volume>>a.f1>>a.t1>>a.f2>>a.f3>>a.f4;
		return in;
	}

	friend binwriter& operator<<(binwriter& out, const Data& a)
	{
		unsigned day,month,year;
		sscanf(a.date_time.c_str(),"%4d%2d%2d",&year,&month,&day);
			   
		char c[9];
		strcpy(c,a.stock_name.c_str());
		out.write_line(c,9);
		out<<((year-1)*372u+(month-1)*31u+day)<<a.vwap<<a.volume<<a.f2;

		return out;
	}

public:

       Solution()
       {
		   in.open(SOURCE_DIR"/input.txt");
		   if(!in.is_open())
			   throw logic_error("Can't open input.txt");
	   }
            
       ~Solution()
       {
		   in.close();
	   }                 	
       
       void process()
       {
		   Data x;

		   while(in.good())
		   {
			   in>>x;
			   if(!in.good())break;

			   out.open((prefix+"output_"+x.stock_name+".txt").c_str(), ios_base::app);
			   out<<x;
			   out.close();
		   }

	   }
            
};

const string Solution::prefix = SOURCE_DIR"/";

int main()
{
	try
	{
		Solution s;
		s.process();
	}
	catch(exception &e)
	{
		cerr<<e.what();
	}
	return 0;  
}
