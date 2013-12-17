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

private :	

		map<int,int> a,kol;
		map<int,size_t> z;
		map<int,int> u;
		map<int,double> ans;
		int time;
		vector<int> types,all_types;

		BinRW RR;

		void writer()
		{
			sort(all_types.begin(),all_types.end());
			vector<int> :: iterator it;
			for(it=all_types.begin();it!=all_types.end();it++)
			{
				RR.BinWriter(*it);
				double w=0;
				if (kol[*it]!=0) w=ans[*it]/(1.0*kol[*it]);
				RR.BinWriter(w);
			}

		}

		bool check(const BinRW :: msg &x)
		{
			return (z[x.type]+sizeof(x.time)+sizeof(x.type)+sizeof(x.len)+sizeof(char)*x.len<=2048);
		}

		void calc(const BinRW :: msg &x)
		{
			if (time!=x.time) 
				{
					vector<int> :: iterator it;
					for(it=types.begin();it!=types.end();it++)
					{					
						ans[*it]+=a[*it];
						kol[*it]++;
						a[*it]=0;
						z[*it]=0;
					}
					types.clear();
					time=x.time;
				} 
				if (u[x.type]==0 && x.type!=-1)
						{
							all_types.push_back(x.type);
							u[x.type]++;
						}
				if (!check(x)) return;
				a[x.type]++;
				if (a[x.type] == 1 && x.type!=-1) 
					{
						types.push_back(x.type);						
					}
				z[x.type]+=sizeof(x);
		}

public :

		Solution()
		{
			RR.BinOpen(SOURCE_DIR"/input.txt",SOURCE_DIR"/output.txt");
			if (!RR.Bin_isOpen()) 
			{
				throw new exception();				
			}				
			time=-1;	
		}

		~Solution()
		{
			types.clear();
			RR.BinClose();
		}


		void process()
		{
			while (1)
			{
				BinRW :: msg x;
				RR.BinReader(x);
				if (!RR.Bin_nice()) break;
				calc(x);
			}
			BinRW :: msg x;
			x.time=-1;
			x.type=-1;
			calc(x);
			writer();
		}
					
};


int main()
{
	try
	{
		Solution s;
		s.process();
	}

	catch(...)
	{
		cerr << "Can't open file" << endl;
	}
	return 0;
}