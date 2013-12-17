#include "StockDealer.h"
#include <sstream>
#include <iostream>
#include <boost/thread.hpp>

typedef std::vector< StockDealer::StockDeal >::iterator vector_iterator;
typedef std::map< std::string, std::vector< StockDealer::StockDeal > >::const_iterator const_map_iterator;

StockDealer::StockDealer()
{
	this->stock_info.open( input_file_path, std::ios::binary );
	if ( !this->stock_info.is_open() )
	{
		throw std::runtime_error( "Failed to open input file" );
	}
}

StockDealer::~StockDealer()
{
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

	return true;
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
		this->sort_deal( buffer );
	}

	this->output_catalogue();
}

void StockDealer::sort_deal( const StockDealer::StockDeal& deal )
{
	std::string stock_name( deal.stock_name );

	if ( this->deals_catalogue.find( stock_name ) == this->deals_catalogue.end() )
	{
		std::vector< StockDeal > initial;
		this->deals_catalogue[ stock_name ] = initial;
	}

	this->deals_catalogue[ stock_name ].push_back( deal );
}

void StockDealer::write_stock( std::string stock_name )
{
	std::string out_file_path = SOURCE_DIR "/tests/3_6/output_";
	out_file_path += stock_name + ".txt";
	
	std::ofstream out_file ( out_file_path.c_str(), std::ios_base::binary );

	if ( !out_file.is_open() )
	{
		std::cerr << "Failed to create file " << out_file_path << std::endl;
		return;
	}

	for ( vector_iterator it = this->deals_catalogue[ stock_name ].begin();
		it != this->deals_catalogue[ stock_name ].end(); it++ )
	{
		it->write_deal( out_file );
	}
}

void StockDealer::output_catalogue()
{
	boost::thread_group output_threads;

	for ( const_map_iterator it = this->deals_catalogue.begin(); 
		it != this->deals_catalogue.end(); it++ )
	{
		output_threads.create_thread( boost::bind( &StockDealer::write_stock, this, it->first ) );
	}

	output_threads.join_all();
}