#include "buf.h"


buf::buf( string & in_name, string & out_name ):TotalTypes(100000)
{
	fileErr = false;
	in.open(in_name.c_str(), fstream::in | fstream::binary);
	out.open(out_name.c_str(), fstream::out | fstream::binary);
	if(!in.is_open() || !out.is_open())
	{
		cout << "File not found! "<<endl;
		fileErr = true;
		return;
	}
	numBytes.resize(TotalTypes + 1, 0);
	messages.resize(TotalTypes + 1, 0);

}

buf::~buf(void)
{
	numBytes.clear();
	messages.clear();
	if(in)
		in.close();
	if(out)
		out.close();
}


int buf::createOutput()
{
	if (fileErr)
	{
		return -1;
	}
	int n = 0; //total number of objects
	UINT32 time, len, type;
	UINT32 curTime = 0;
	char* str = NULL;
	bool toWrite = false;
	messages.assign(TotalTypes + 1, 0);
	while (in)
	{
		if( in.peek() == EOF )
		{
			break;
		}
		type = read_uint32(in);
		time = read_uint32(in);
		if(time>curTime)
		{
			curTime = time;
			numBytes.assign(TotalTypes + 1, 0);
		}		
		len = read_uint32(in);
		if ((type<TotalTypes)&&((numBytes[type] + len + 3 * sizeof(UINT32)) <= 2048))
		{
			numBytes[type] += len + 3 * sizeof(UINT32);
			messages[type] ++;
		}
		str = new char[len+1];
		for (size_t i = 0; i < len; i++)
		{
			in.read(&(str[i]), 1);
		}
		str[len] = '\0';
		n++;
		if(str)
		{
			delete[] str;
		}
		if((len == 0) || (type > TotalTypes) )
		{
			continue;
		}
	}
	type = 0;

	for(vector<size_t>::iterator msgs = messages.begin(); msgs != messages.end(); ++msgs)
	{
		if (*msgs == 0)
		{
			type++;
			continue;
		}
		write_uint32(out, type);
		double val = (double)*msgs/curTime;
		write_double(out, val);
		type++;
	}
	return n;
}
