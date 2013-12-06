#include <config_reader.h>

#include <fstream>

#include <boost/lexical_cast.hpp>

common::config_reader::config_reader( const std::string& filename )
{
	std::ifstream ifs( filename.c_str(), std::ios::in );
	
	ifs >> trade_receive_threads_size_;
	ifs >> quote_receive_threads_size_;

	check_thread_size_( trade_receive_threads_size_ );
	check_thread_size_( quote_receive_threads_size_ );

	read_addresses_( ifs, trade_listen_addresses_ );
	read_addresses_( ifs, quote_listen_addresses_ );
}
void common::config_reader::check_thread_size_( const size_t readed_size )
{
	if ( readed_size > max_receive_thread_size )
		throw std::logic_error( "currently maximum amount of receiving threads could be " + 
			boost::lexical_cast<std::string>( max_receive_thread_size ) );
}
void common::config_reader::read_addresses_( std::istream& ifs, addresses& read_addresses )
{
	size_t address_count;

	ifs >> address_count;
	if ( address_count > max_listen_addresses_size )
		throw std::logic_error( "currently maximum size for listen address array " + 
			boost::lexical_cast<std::string>( max_listen_addresses_size ) );

	for( size_t i = 0 ; i < address_count ; ++i )
	{
		std::string ip_address;
		unsigned short port;
		ifs >> ip_address >> port;
		trade_listen_addresses_.push_back( address( ip_address, port ) );
	}
}
