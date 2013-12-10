#include <communication.h>
#include <minute_calculator.h>
#include <processor.h>

#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <vector>
#include <utility>
#include <signal.h>

#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

class minute_market_data
{
	std::ofstream out;
	boost::mutex mtx;
	std::ifstream in;
	std::vector< multicast_communication::address > trade_ports, quote_ports;

	size_t trade_thread_size, quote_thread_size, 
		trade_ports_amount, quote_ports_amount;
	unsigned short trade_port;

	std::string ip_address; 
	static boost::shared_ptr< multicast_communication::communication > com;
	static boost::shared_ptr< minute_calculator::datafeed_calculator > calc;

	static void button_handler( int )
	{
		if( calc.get() != NULL )
			calc->stop();
		if( com.get() != NULL )
			com->stop();
		else 
			exit( 0 );
	}

	void print_datafeed( const minute_calculator::minute_datafeed_ptr& data )
	{
		boost::mutex::scoped_lock lock( mtx );
		out.open( SOURCE_DIR + std::string( "/" ) + std::string( data->stock_name ) + ".dat", std::ios_base::binary | std::ios_base::app );

		out.write( reinterpret_cast< char* >( &( *data ) ), sizeof( *data ) );

		out.close();
	}

public:

	explicit minute_market_data()
	{
		in.open( SOURCE_DIR"/config.ini", std::ios_base::in );
		if( !in.is_open() )
			throw std::logic_error( "Can't open config file" );
		signal( SIGINT, button_handler );
	}

	~minute_market_data()
	{
		in.close();
	}

	void process()
	{
		in >> trade_thread_size >> quote_thread_size;
		in >> trade_ports_amount;
		
		for ( size_t i = 0; i < trade_ports_amount; ++i )
		{
			in >> ip_address >> trade_port;
			trade_ports.push_back( std::make_pair( ip_address, trade_port ) );
		}

		in >> quote_ports_amount;

		for ( size_t i = 0; i < quote_ports_amount; ++i )
		{
			in >> ip_address >> trade_port;
			quote_ports.push_back( std::make_pair( ip_address, trade_port ) );
		}

		using namespace multicast_communication;
		
		calc = boost::shared_ptr< minute_calculator::datafeed_calculator >
			( new minute_calculator::datafeed_calculator ( boost::bind( &minute_market_data::print_datafeed, this, _1 ) ) );
		com = boost::shared_ptr< communication > 
			( new communication 
				( trade_thread_size
				, quote_thread_size
				, trade_ports
				, quote_ports
				, multicast_communication::processor_ptr( new processor( boost::bind( &minute_calculator::datafeed_calculator::new_quote, *calc, _1 )
																		,boost::bind( &minute_calculator::datafeed_calculator::new_trade, *calc, _1 ) ) ) ) );

		com->start();
		com->connection_threads.join_all();
	}
};

boost::shared_ptr< multicast_communication::communication > minute_market_data::com;
boost::shared_ptr< minute_calculator::datafeed_calculator > minute_market_data::calc;
	
int main()
{
	minute_market_data s;
	s.process();

	return 0;
}


