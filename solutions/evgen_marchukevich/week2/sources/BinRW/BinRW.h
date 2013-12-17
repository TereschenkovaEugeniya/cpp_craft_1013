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

class BinRW
{
private:

ifstream f1;
ofstream f2;

bool error;

public :
	
struct msg{
		int type;
		int time;
		int len;
		string m;
		};
void BinOpen(const char*,const char*);
void BinClose();
bool Bin_isOpen();
bool Bin_nice();
void BinReader(msg &);
void BinWriter(int&);
void BinWriter(double&);
void BinWriter(msg &);
//BinRW();
//~BinRW();
};

