#ifndef _TRADES_FILTER_
#define _TRADES_FILTER_
#include <iostream>
#include <fstream>
#include <stdint.h>

using namespace std;
enum Types {
	MARKET_OPEN = 1u,
	TRADE = 2u,
	QUOTE = 3u,
	MARKET_CLOSE = 4u
};
struct Trade
{
public:
	uint32_t typ;
	uint32_t time;
	uint32_t len;
	string str;
	Trade(uint32_t t = 0, uint32_t time = 0, uint32_t l = 0, char * st = ""): typ(t), time(time), len(l)
	{
		str = string(st);
	}
	void change(uint32_t t = 0, uint32_t time = 0, uint32_t l = 0, char * st = "")
	{
		typ = t;
		time = time;
		len = l;
		str = string(st);
	}
	int write(ofstream& o);
	int read(ifstream& i);
	
};


#endif // _TRADES_FILTER_
