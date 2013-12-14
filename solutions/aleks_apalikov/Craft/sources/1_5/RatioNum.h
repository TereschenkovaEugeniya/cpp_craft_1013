#pragma once
#include <fstream>
#include <vector>
#include <utility>
#include <stdint.h>

typedef double code_d;
typedef uint64_t val_t ;
struct code_t
{
	std::pair<val_t, val_t> num;
	bool operator== (code_t b)
	{
		return (num.first == b.num.first) && (num.second == b.num.second) ;
	}

	friend std::istream& operator>> ( std::istream & ostr, code_t& c )
	{
		val_t a , b , denom = 1, d, count = 0;
		char str[16];//, str2[16];
		ostr.getline(str, 10);
		ostr.getline(str, 10, '.'); //think about doubles without .dot
		a = atoi(str);
		ostr>>b;
		d = b;
		while(d /= 10)
		{
			count++;
			denom *= 10;
		}
		for(int i = 0; i < 4; i++)
		{
			denom /= 1000;
			if(denom != 0)
				b = b / denom;
		}
		c.num = std::make_pair(a, b);
		return ostr;
	}

};
using namespace std;
class RatioNum
{
	ifstream fs;
	ofstream of;
	int count;
	vector<val_t> codes; 
public:
	RatioNum(const string& fileName = "Input2.txt");
	static void trunc(double& num);
	void push_code(code_d num);
	int check();
	~RatioNum(void);
	val_t validate(code_d num);
	val_t convert( double d);
};
