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
		};


	ifstream f1;
	ofstream f2;

	bool error;

public :


	void reader(msg &t)
	{	
		for(unsigned int i=0;i<8;i++)
		{
			char c;
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


	void writer(msg &x, unsigned int &t)
	{
		for(int i=0;i<9;i++)
			f2.write(reinterpret_cast<char*>(&x.name[i]),sizeof (char));
		f2.write(reinterpret_cast<char*>(&t),sizeof (t));
		f2.write(reinterpret_cast<char*>(&x.vwap),sizeof (x.vwap));
		f2.write(reinterpret_cast<char*>(&x.volume),sizeof (x.volume));
		f2.write(reinterpret_cast<char*>(&x.f2),sizeof (x.f2));
	}

	void calc(msg &x)
	{
		int year, month, day;
		sscanf(x.date.c_str(),"%4d%2d%2d",&year,&month,&day);
		unsigned int pp= 1ll*(year-1)*372+1ll*(month-1)*31+1ll*day;
		writer(x,pp);
	}


	void process()
	{
		while (1)
		{
			msg x;
			reader(x);
			if (error) break;
			calc(x);
		}
	}	

	Solution()
	{
		f1.open(SOURCE_DIR"/input.txt", ios::in | ios::binary);
		f2.open(SOURCE_DIR"/output.txt",ios::out| ios::binary);
		error=0;
		if (!f1.is_open() || !f2.is_open()) 
			throw new exception();	
	}

	~Solution()
	{
		f1.close();
		f2.close();	
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
		cerr << "Can't open file" << endl;
	}
	return 0;
}