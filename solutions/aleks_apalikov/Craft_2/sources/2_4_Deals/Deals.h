#pragma once
#include <fstream>
#include <string>
#include <iostream>
#include "DealsElem.h"

using namespace std;
class Deals
{
	fstream in;
	ofstream out;
	bool fileErr;
public:
	Deals(string & file_name, string & out_file);
	int createOutput(); //read one element and output if condtions are true
	~Deals(void);
};
