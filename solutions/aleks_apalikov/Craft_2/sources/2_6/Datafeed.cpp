#include "Datafeed.h"
//now working only with 0x20 symbols instead of 0x0

Datafeed::Datafeed( string & in_name, string & out_name )
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

}

Datafeed::~Datafeed(void)
{

	in.close();
	out.close();
}

int Datafeed::createOutput()
{
	if (fileErr)
	{
		return -1;
	}
	int n = 0; //total number of objects
	Data* d;
	char* str = NULL;
	bool toWrite = false;
	DealsElem* de = NULL;
	while (!in.eof())
	{
		d = new Data();
		d->stock_name = read_str(in);
		d->date_time = read_date(in);
		d->days();
		d->price = read_double(in);
		d->vwap = read_double(in);
		d->volume = read_uint32(in);
		d->f1 = read_double(in);
		d->t1 = read_double(in);
		d->f2 = read_double(in);
		d->f3 = read_double(in);
		d->f4 = read_double(in);
		
		write_str( out, d->stock_name.c_str() ); 
		if(write_uint32(out, d->ds) != d->ds)
			cout << "An error in write_uint32 () ";
		write_double(out, d->vwap);
		if(write_uint32(out, d->volume) != d->volume)
			cout << "An error in write_uint32 () ";
		write_double(out, d->f1);
		write_double(out, d->f4);
		write_double(out, d->f3);
		n++;
		delete d;
		if( in.peek() == EOF )
		{
			break;
		}
	}
	return n;
}

UINT32 Data::days()
{
	int total = ((date_time / 10000) - 1)* 372;
	int month = (date_time % 10000) /100;
	int days = date_time % 100;
	total += (month - 1) * 31 + days;
	ds = total;
	return total;
}
