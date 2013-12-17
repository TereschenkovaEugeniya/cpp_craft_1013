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

#define pb(VALUE) push_back(VALUE)
#define mp(FST,SEC) make_pair(FST,SEC)
#define len(STR) STR.length()
#define F first
#define S second

using namespace std;

bool check(const string &s,const string &s1, int p )
{
	int u=len(s1);
	for(int i=p;i<p+u;i++)
	{
		if (s[i]!=s1[i-p]) return 0;
	}
	return 1;	
}

int main()
{
	ifstream f1("input.txt");
	ofstream f2("output.txt");
	if (!f1.is_open() || !f2.is_open()) return 0;
	setlocale(LC_ALL,"");
	string s,s1,s2;
	s="";
	getline(f1,s1);
	for(int i=0;i<len(s1);i++)
	if (s1[i]!='-' && s1[i]!=' ' && s1[i]!=92)
	{
		s+=tolower(s1[i]);
	}
	while (!(f1.eof()))
	{
		getline(f1,s1);
		s2="";
		for(int i=len(s1)-1;i>=0;i--)
			if(s1[i]!='-' && s1[i]!=' ' && s1[i]!=92)
				s2+=tolower(s1[i]);	
		bool f=0;
		int kk=len(s2);
		for(int i=0;i<len(s)-kk+1;i++)
		{
			if (check(s,s2,i))
				{
					f=1;
					break;
				}
		}
		if (f) f2 << "YES" << endl; else f2 << "NO" << endl;
	}
	f1.close();
	f2.close();
	return 0;
}
