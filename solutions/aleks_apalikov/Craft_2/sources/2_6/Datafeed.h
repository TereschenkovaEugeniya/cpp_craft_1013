#pragma once
#include <fstream>
#include <string>
#include <iostream>
#include "DealsElem.h"
//solution of task 2.6

struct Data
{
	string stock_name;
	UINT32 date_time; //over 9 digits - 4billions
	UINT32 ds;
	double price;
	double vwap;
	UINT32 volume; 
	double f1, t1, f2, f3, f4;
	UINT32 days(); //after 1.1.1
};

class Datafeed
{
	fstream in;
	ofstream out;
	bool fileErr;
public:
	Datafeed(string & in_name, string & out_name);
	int createOutput(); //read elements one by one and output average values
public:
	~Datafeed(void);
};
