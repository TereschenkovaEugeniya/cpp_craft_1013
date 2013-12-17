#include <iostream>
#include <fstream>
#include <cstdio>  
#include <string>
#include <cstring>
#include <math.h> 
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


void check(int x,int y,queue <pair <int,int> > &q, int **b,vector<string> &a)
{
	if (x<0 || x>= a.size() || y<0 || y>=len(a[0])) return;
	if (b[x][y]==1 || a[x][y]=='~') return;	
	q.push(mp(x,y));
	b[x][y]=1;

}

void wfs(queue <pair <int,int> > &q, int **b,vector<string> &a)
{	
	while (!q.empty())
	{
		int x=q.front().F;
		int y=q.front().S;
		q.pop();
		check(x-1,y,q,b,a);
		check(x,y-1,q,b,a);
		check(x,y+1,q,b,a);
		check(x+1,y,q,b,a);
	}
}

int main()
{
	ifstream f1("input.txt");
	ofstream f2("output.txt");
	int ans=0;
	int **b;
	queue< pair <int,int> > q;
	vector<string> a;
	while (!f1.eof())
	{
		string s;		
		f1 >> s;		
		a.pb(s);
	}
	b = new int*[a.size()+1];
	for(int i=0;i<a.size();i++)
		b[i] = new int [len(a[i])];	
	for(int i=0;i<a.size();i++)
		for(int j=0;j<len(a[i]);j++)
			b[i][j]=0;
	for(int i=0;i<a.size();i++)
		for(int j=0;j<len(a[i]);j++)
		 if (b[i][j]==0 && a[i][j]!='~')
		 {
			q.push(mp(i,j));
			b[i][j]=1;
			ans++;						
			wfs(q,b,a);
		 }
	f2 << ans << endl;
	return 0;
}
