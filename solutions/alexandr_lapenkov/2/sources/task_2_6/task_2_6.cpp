#include <iostream>
#include <math.h>
#include <algorithm>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include "readlib.h"
#include "writelib.h"

using namespace std;

class Solution
{ 
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
	
	};

	friend binreader& operator>>(binreader& in, Data& a)
	{
		a.stock_name = in.get_line(8);
		a.date_time = in.get_line(8);
		in>>a.price>>a.vwap>>a.volume>>a.f1>>a.t1>>a.f2>>a.f3>>a.f4;
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
		   Data x;
		   
		   while(in.good())
		   {
			   in>>x;
			   if(!in.good())break;

			   unsigned day,month,year;
			   sscanf(x.date_time.c_str(),"%4d%2d%2d",&year,&month,&day);
			   
			   char c[9];
			   strcpy(c,x.stock_name.c_str());
			   out.write_line(c,9);
			   out<<((year-1)*372u+(month-1)*31u+day)<<x.vwap<<x.volume<<x.f2;
		   }
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
