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
			for(int i=0;i<t.len;i++)
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
			for(int i=0;i<a.len;i++)
				{
					f2.write(reinterpret_cast<char*>(&(a.m[i])),sizeof (char));
				}
}


void BinRW :: BinWriter(int &a)
{
	f2.write(reinterpret_cast<char*>(&(a)),sizeof (a));		
}


void BinRW :: BinWriter(double &a)
{
	f2.write(reinterpret_cast<char*>(&(a)),sizeof (a));		
}




void BinRW:: BinOpen(const char* inp_f,const char* outp_f)
{
f1.open(inp_f, ios::binary |ios::in);
f2.open(outp_f, ios::binary | ios :: out);
error = 1;
}

bool BinRW :: Bin_isOpen()
{
	return f1.is_open()&&f2.is_open();
}

bool BinRW :: Bin_nice()
{
	return error;
}

void BinRW:: BinClose()
{
f1.close();
f2.close();
}
