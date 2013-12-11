#include "test_registrator.h"
#include <udp_listener.h>

#include <fstream>
#include <string>

#include <boost/filesystem.hpp>

namespace multicast_communication
{
	namespace tests_
	{
		size_t calculate_lines()
		{	
			size_t res = 0;
			std::ifstream in;
			in.open( SOURCE_DIR"/market_data.dat", std::ios_base::in );
			
				std::string s;
				while( getline( in, s ) )
				{
					if( s.length() == 0 )
						break;
					++res;
				}

			in.close();

			return res;
		}
	}
}

void multicast_communication::tests_::communication_and_listener_tests()
{
	{
		boost::asio::io_service io;
		std::vector< address >trade_ports, quote_ports;
		quote_ports.push_back( std::make_pair( "224.0.0.0", 49000 ) );

		communication receiver( 2, 2, trade_ports, quote_ports, processor_ptr( new processor( "market_data.dat" ) ) );
		receiver.start();

		std::string message;
		std::ifstream in( SOURCE_DIR"/tests/data/quote_messages" );
		std::getline( in, message );
		boost::asio::ip::udp::endpoint endpoint( boost::asio::ip::address::from_string( "224.0.0.0" ), 49000 ); 
		boost::asio::ip::udp::socket socket( io, endpoint.protocol() );
     
		BOOST_CHECK_NO_THROW
			(
				for( size_t i = 0; i < 500; ++i )
				{
					socket.send_to( boost::asio::buffer( message ), endpoint );	
					boost::this_thread::sleep_for( boost::chrono::milliseconds( 5 ) );
				}
				boost::this_thread::sleep_for( boost::chrono::milliseconds( 150 ) );
			);
		BOOST_CHECK_NO_THROW
			(
				message = "abc";
				receiver.processor->parse( message, QUOTE );
			);
	}
	
	BOOST_CHECK_EQUAL( calculate_lines(), 1000 );

}