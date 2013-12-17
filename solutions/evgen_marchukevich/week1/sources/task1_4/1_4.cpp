#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>
#include <cstring>
#include <math.h>
#include <list>
#include <queue>
#include <deque>
#include <set>
#include <map>
#include <stdlib.h>
#include <algorithm>
#include <vector>


using namespace std;


int main()
{
	ifstream f1("input.txt");
	ofstream f2("output.txt");
	if (!f1.is_open() || !f2.is_open()) return 0;
	int n;
	f1 >> n;
	vector<long long> a;
	for(int i=0;i<n;i++)
	{
		double x;
		f1 >> x;
		long long X =  (long long) (x*10000);
		a.push_back(X);
	}
	sort(a.begin(),a.end());
	while (!f1.eof())
	{
		double x;
		f1 >> x;
		long long X = (long long) (x*10000);
		vector<long long> :: iterator tr=lower_bound(a.begin(),a.end(),X);
		if (tr!=a.end() && *tr==X) 
			f2 << "YES" << endl; 
			else 
			f2 << "NO" << endl; 
	}
	f1.close();
	f2.close();
	return 0;
}
