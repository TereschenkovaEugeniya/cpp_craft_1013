#include "Deals.h"

Deals::Deals( string & fileName )
{
	fileErr = false;
	string fileN1 = fileName + ".in"; 
	in.open(fileN1.c_str(), fstream::in | fstream::binary);
	string fileN2 = fileName + ".out"; 
	out.open(fileN2.c_str(), fstream::out | fstream::binary);
	if(!in.is_open() || !out.is_open())
	{
		cout << "File not found! "<<endl;
		fileErr = true;
		return;
	}


}

Deals::~Deals(void)
{
	in.close();
	out.close();
}

int Deals::createOutput()
{
	if (fileErr)
	{
		return -1;
	}
	int n = 0; //total number of objects
	UINT32 type, time, len, currentTime = 0;
	char* str = NULL;
	bool toWrite = false;
	DealsElem* de = NULL;
	while (!in.eof())
	{
		type = read_uint32(in);
		toWrite = (type>=MARKET_OPEN && type<=MARKET_CLOSE);
		time = read_uint32(in);
		if(currentTime != 0) //test of first read
		{
			if (time<=currentTime-2)
			{
				toWrite |= false;
			}
			else 
			{
				toWrite |= true;
				currentTime = time;
			}

		}
		else
		{
			toWrite |= true;
			currentTime = time;
		}
		len = read_uint32(in);
		if(len > 1000)
			break;
		if(str)
		{
			delete[] str;
		}
		if (de)
		{
			delete de;
		}
		str = new char[len+1];
		for (int i = 0; i < len; i++)
		{
			str[i] = in.get();
		}
		if( in.peek() == EOF )
		{
			break;
		}
		str[len] = '\0';
		n++;
		de = new DealsElem(type, time, len, str);
		if (toWrite)
		{
			de->operator <<(out);
		}
	}

	if(str)
	{
		delete[] str;
	}
	return n;
}

