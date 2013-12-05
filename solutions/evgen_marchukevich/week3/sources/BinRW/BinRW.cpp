#include "BinRW.h"


using namespace std;


void BinRW::BinReader(msg &t)
{
			if(f1.read(reinterpret_cast<char*>(&t.type),sizeof (int))==0) 
				{
					error=0;
					return;
				}
			f1.read(reinterpret_cast<char*>(&t.time),sizeof (int));
			f1.read(reinterpret_cast<char*>(&t.len),sizeof (int));
			for(unsigned int i=0;i<t.len;i++)
			{
				char c;
				f1.read(reinterpret_cast<char*>(&c),sizeof (char));
				t.m+=c;
			}	

}
void BinRW :: BinWriter(msg &a)
{
			f2.write(reinterpret_cast<char*>(&(a.type)),sizeof (int));
			f2.write(reinterpret_cast<char*>(&(a.time)),sizeof (int));
			f2.write(reinterpret_cast<char*>(&(a.len)),sizeof (int));
			for(unsigned int i=0;i<a.len;i++)
				{
					f2.write(reinterpret_cast<char*>(&(a.m[i])),sizeof (char));
				}
}


void BinRW :: BinWriter(unsigned int &a)
{
	f2.write(reinterpret_cast<char*>(&(a)),sizeof (a));		
}


void BinRW :: BinWriter(double &a)
{
	f2.write(reinterpret_cast<char*>(&(a)),sizeof (a));		
}




void BinRW :: BinOpen_inFile(const char* inp_f)
{
f1.open(inp_f, ios::binary |ios::in);
error = 1;
}

void BinRW :: BinOpen_outFile(const char* outp_f)
{
f2.open(outp_f, ios::binary | ios :: out);
}

bool BinRW :: Bin_isOpen_inFile()
{
	return f1.is_open();
}

bool BinRW :: Bin_isOpen_outFile()
{
	return f2.is_open();
}

bool BinRW :: Bin_nice()
{
	return error;
}

void BinRW:: BinClose_in()
{
	f1.close();
}

void BinRW:: BinClose_out()
{
	f2.close();
}

