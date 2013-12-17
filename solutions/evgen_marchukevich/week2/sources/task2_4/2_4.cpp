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


using namespace std;


class Solution
{

private:
	BinRW RR;

	bool error,inn1,inn2;
	int T;
	vector<BinRW :: msg> a;			

	void writer(int T)
		{
			vector<BinRW ::msg> b;
			vector<BinRW ::msg> :: iterator it;
			for(it=a.begin();it!=a.end();it++)
			{
				if (it->time!=T) b.push_back(*it); else
				{
					RR.BinWriter(*it);
				}
			}
			a.clear();
			for(it=b.begin();it!=b.end();it++)
				a.push_back(*it);
			b.clear();
		}
	
	bool check(const BinRW ::msg &t, const int max)
		{
			if (t.type<1 || t.type>4) return 0;
			if (max==-1) return 1;
			if (t.time <=max-2) return 0;
			return 1;
		}

public:

	Solution()
	{
		RR.BinOpen(SOURCE_DIR"/input.txt",SOURCE_DIR"/output.txt");
		if (!RR.Bin_isOpen()) 
		{
			throw new exception();
		}
		error=0;
		inn1=inn2=1;
		T=-1;
	}

	~Solution()
	{		
		a.clear();
		RR.BinClose();
	}

	void process()
		{
			while (1)
			{
				BinRW :: msg x;
				RR.BinReader(x);
				if (!RR.Bin_nice()) break;
						if (!check(x,T)) 
							continue;
						if (inn1)
						{
							T=x.time;
							inn1=0;
							a.push_back(x);
							continue;
						}
						if (x.time>T+1)
						{
							writer(T-1);
							writer(T);			
							a.push_back(x);
							T=x.time;
							continue;
						}
						if (x.time==T+1 && !inn2)
						{
							writer(T-1);
							a.push_back(x);
							T++;
							continue;
						}
						if (x.time==T+1) 
						{
							inn2=0;
							T++;
						}
						a.push_back(x);
				}
			if (a.size()!=0)
			{
				writer(T-1);
				writer(T);
			}	
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