#include "RatioNum.h"
#include <math.h>
#include <string>
#include <cmath> 
#include <limits>

RatioNum::RatioNum(const string& fileName)
{
	string str = BINARY_DIR + string("/") + fileName;
	fs.open(str.c_str(), fstream::in);
	
	int n = 0;
	if(!fs.is_open()) {
		printf("fs failed to open!");
	}
	fs >> n;
	code_d cod;
	for (int i = 0; i < n; i++)
	{
		fs >> cod;
		push_code(cod);
	}
	string out_s = BINARY_DIR + string("/") + "Output2.txt";
	of.open(out_s.c_str(), fstream::out  | fstream::trunc );

	//str = path + "Output.txt";
	if(!of.is_open()) {
		printf("Output2 failed to open!");
	}
}

RatioNum::~RatioNum(void)
{
	fs.close();
	of.close();
}

int RatioNum::check()
{
	code_d pass;
	bool concur;
	while (!fs.eof())
	{
		concur = false;
		char c[20];
		if(fs.peek() == EOF)
			break;
		fs>>pass;
		val_t check_this = validate(pass);
		for(vector<val_t>::iterator it = codes.begin(); it < codes.end(); it++)
		{
			concur |= (*it == check_this);
		}
		string ans = concur ? "Yes\n" : "No\n";
		of << ans.c_str();
		fs.getline(c, 20); //read all spaces after float and \n symbol
	}
	
	return count;
}

void RatioNum::trunc( double& num )
{
	num = floor(num * 10000) / 10000;

}


void RatioNum::push_code( code_d num)
{
	codes.push_back(validate(num));
}
val_t RatioNum::validate(code_d num)
{
	code_d next = num + fabs(num) * std::numeric_limits<double>::epsilon();
	if(convert(num) != convert(next) )
	{		
		return convert(next);
	}
	return convert(num);
}
val_t RatioNum::convert( double d)
{
	return (val_t) (d * 10000);
}
