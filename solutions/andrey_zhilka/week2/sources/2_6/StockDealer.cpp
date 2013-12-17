#include "StockDealer.h"
#include <sstream>
#include <iostream>

StockDealer::StockDealer()
{
	this->stock_info.open( input_file_path, std::ios::binary );
	if ( !this->stock_info.is_open() )
	{
		throw std::runtime_error( "Failed to open input file" );
	}

	this->info_output.open( output_file_path, std::ios::binary );
	if ( !this->info_output.is_open() )
	{
		throw std::runtime_error( "Failed to open output file" );
	}
}

StockDealer::~StockDealer()
{
	this->info_output.close();
	this->stock_info.close();
}

StockDealer::StockDeal::StockDeal()
{
	this->stock_name = new char[9];
}

StockDealer::StockDeal::~StockDeal()
{
	delete [] this->stock_name;
}

bool StockDealer::StockDeal::read_deal( std::ifstream& stock_info )
{
	char* day_buffer = new char[9];

	int years, 
		monthes,
		days;

	stock_info.read( this->stock_name, 8 );

	if ( stock_info.eof() )
	{
		return false;
	}
	else 
	{
		this->stock_name[8] = 0;
		stock_info.read( day_buffer, 8 );
		sscanf( day_buffer, "%4d%2d%2d", &years, &monthes, &days );

		this->date = ( years - 1 ) * 372 + ( monthes - 1) * 31 + days;

		stock_info.seekg( sizeof( double ), std::ios_base::cur );
		stock_info.read( reinterpret_cast< char* > ( &this->price ), sizeof( double ) );

		stock_info.read( reinterpret_cast< char* > ( &this->volume ), sizeof( unsigned ) );

		stock_info.seekg( 2 * sizeof( double ), std::ios_base::cur );
		stock_info.read( reinterpret_cast< char* > ( &this->f2 ), sizeof( double ) );

		stock_info.seekg( 2 * sizeof( double ), std::ios_base::cur );	
	}
}

void StockDealer::StockDeal::write_deal( std::ofstream& info_output )
{
	info_output.write( this->stock_name, 9 );
	info_output.write( reinterpret_cast< const char* > ( &this->date ), sizeof( unsigned ) );
	info_output.write( reinterpret_cast< const char* > ( &this->price ), sizeof( double ) );
	info_output.write( reinterpret_cast< const char* > ( &this->volume ), sizeof( unsigned ) );
	info_output.write( reinterpret_cast< const char* > ( &this->f2 ), sizeof( double ) );
}

void StockDealer::process_info()
{
	StockDeal buffer;

	while ( buffer.read_deal( this->stock_info ) )
	{
		buffer.write_deal( this->info_output );
	}
}