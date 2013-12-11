#include <communication.h>
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

namespace multicast_communication
{

	class market_data_receiver
	{
	private:
		std::ifstream in;
		std::vector< address > trade_ports, quote_ports;

		size_t trade_thread_size, quote_thread_size, 
			trade_ports_amount, quote_ports_amount;
		unsigned short trade_port;

		std::string ip_address; 
		static boost::shared_ptr< communication > c;

		static void button_handler( int )
		{
			exit( 0 );
		}

	public:
		explicit market_data_receiver()
		{
			in.open( SOURCE_DIR"/config.ini", std::ios_base::in );
			if( !in.is_open() )
				throw std::logic_error( "Can't open config file");
			signal( SIGINT, button_handler );
		}

		virtual ~market_data_receiver()
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

			c = boost::shared_ptr< communication > 
				( new communication 
					( trade_thread_size
					, quote_thread_size
					, trade_ports
					, quote_ports
					, processor_ptr( new processor( "market_data.dat" ) ) ) );

			c->start();
			c->connection_threads.join_all();
		}
	};
	boost::shared_ptr< communication > market_data_receiver::c;
}

int main()
{
	multicast_communication::market_data_receiver s;
	s.process();

	return 0;
}


