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
#include <boost/thread.hpp>


using namespace std;


class Solution{

	struct msg{
		char name[9];
		string date;
		double price;
		double vwap;
		unsigned int volume;
		double f1;
		double t1;
		double f2;
		double f3;
		double f4;
		unsigned int t;
		};

	ifstream f1;
	map<string,ofstream> out_files;
	vector<string> files_to_close;

	bool error;

public :


	void reader(msg &t)
	{	
		for(unsigned int i=0;i<8;i++)
		{
			if(f1.read(reinterpret_cast<char*>(&t.name[i]),sizeof (char))==0) 
				{
					error=1;
					return;
				}
		}
		t.name[8]='\0';
		for(unsigned int i=0;i<8;i++)
		{
			char c;
			f1.read(reinterpret_cast<char*>(&c),sizeof (char));
			t.date+=c;
		}
		f1.read(reinterpret_cast<char*>(&t.price),sizeof (double));
		f1.read(reinterpret_cast<char*>(&t.vwap),sizeof (double));
		f1.read(reinterpret_cast<char*>(&t.volume),sizeof (unsigned int));
		f1.read(reinterpret_cast<char*>(&t.f1),sizeof (double));
		f1.read(reinterpret_cast<char*>(&t.t1),sizeof (double));
		f1.read(reinterpret_cast<char*>(&t.f2),sizeof (double));
		f1.read(reinterpret_cast<char*>(&t.f3),sizeof (double));
		f1.read(reinterpret_cast<char*>(&t.f4),sizeof (double));

	}


	void writer(msg &x, ofstream &f2)
	{
		for(int i=0;i<9;i++)
			f2.write(reinterpret_cast<char*>(&x.name[i]),sizeof (char));
		f2.write(reinterpret_cast<char*>(&x.t),sizeof (x.t));
		f2.write(reinterpret_cast<char*>(&x.vwap),sizeof (x.vwap));
		f2.write(reinterpret_cast<char*>(&x.volume),sizeof (x.volume));
		f2.write(reinterpret_cast<char*>(&x.f2),sizeof (x.f2));
	}

	void calc(msg &x)
	{
		int year, month, day;
		sscanf(x.date.c_str(),"%4d%2d%2d",&year,&month,&day);
		unsigned int pp= 1ll*(year-1)*372+1ll*(month-1)*31+1ll*day;		
		x.t=pp;
	}

	void process()
	{
		while (true)
		{
			msg x;
			reader(x);
			if (error) break;
			calc(x);
			write_flow(x);			
		}
	}	
	
	string get_out_file(const char* t)
	{
		string s=SOURCE_DIR;
		s+="/output_";
		s+=t;
		s+=".txt";
		return s;	
	}
	
	void write_flow(msg &x)
	{
			string write_file = get_out_file(x.name);
			if (out_files.count(write_file)==0)
			{
				out_files[write_file].open(get_out_file(x.name).c_str(), std::ios_base :: out | std::ios_base :: binary);
				files_to_close.push_back(write_file);
			}
			writer(x,out_files[write_file]);			
	}


	Solution()
	{	
		
		f1.open(SOURCE_DIR"/input.txt", ios::in | ios::binary);
		if (!f1.is_open()) 
			{
				throw logic_error("Can't open file");
			}
		error=0;		
	}

	~Solution()
	{		
		f1.close();
		for(vector<string> :: iterator it = files_to_close.begin();it!=files_to_close.end();it++)
			out_files[*it].close();
	}


};


int main()
{
	try
	{
		Solution x;
		x.process();
	}
	catch(...)
	{
		cerr << "Can't open file!" << endl;
	}
	return 0;
}