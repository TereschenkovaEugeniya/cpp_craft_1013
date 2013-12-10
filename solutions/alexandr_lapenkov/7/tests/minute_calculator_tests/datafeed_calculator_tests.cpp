#include "test_registrator.h"

#include <minute_calculator.h>
#include <trade_message.h>
#include <communication.h>

#include <fstream>
#include <set>

#include <boost/bind.hpp>
#include <boost/filesystem.hpp>
#include <boost/asio.hpp>

namespace minute_calculator
{
	namespace tests_
	{
		std::set< std::string > delete_filenames;

		void simple_callback_( const minute_datafeed_ptr& datafeed )
		{
			BOOST_CHECK_EQUAL( datafeed->stock_name, "ADM" );
		}

		void output_callback_( const minute_datafeed_ptr& datafeed )
		{
			std::ofstream out;
			const std::string filename = SOURCE_DIR + std::string( "/" ) + std::string( datafeed->stock_name ) + ".dat";
			out.open( filename, std::ios_base::app );

			out << datafeed->bid << '\n';

			out.close();

			delete_filenames.insert( filename );
		}
	}
}

void minute_calculator::tests_::datafeed_calculator_tests()
{
	{
		datafeed_calculator calc( simple_callback_ );

		using multicast_communication::quote_message;
	
		std::string message;
		std::ifstream in( SOURCE_DIR"/tests/data/quote_messages" );
		std::getline( in, message );

		multicast_communication::quote_message_ptr msg( new quote_message( boost::bind( &minute_calculator::datafeed_calculator::new_quote, &calc, _1 ) ) );
		std::stringstream stream( message.substr( 3 ) );
		binreader reader( stream );

		BOOST_CHECK_NO_THROW( msg->parse( reader, message[1], message[2] ) );

		BOOST_CHECK_NO_THROW( calc.stop() );
	}


	{
		using namespace multicast_communication;

		datafeed_calculator calc( output_callback_ );

		boost::asio::io_service io;
		std::vector< address >trade_ports, quote_ports;
		quote_ports.push_back( std::make_pair( "224.0.0.0", 49000 ) );

		communication receiver
			( 2
			, 2
			, trade_ports
			, quote_ports
			, processor_ptr( new processor
							( boost::bind( &minute_calculator::datafeed_calculator::new_quote, &calc, _1 )
							, boost::bind( &minute_calculator::datafeed_calculator::new_trade, &calc, _1 ) ) 
							) 
			);

		receiver.start();

		std::string message;
		std::ifstream in( SOURCE_DIR"/tests/data/233.200.79.0.udp" );
		boost::asio::ip::udp::endpoint endpoint( boost::asio::ip::address::from_string( "224.0.0.0" ), 49000 ); 
		boost::asio::ip::udp::socket socket( io, endpoint.protocol() );

		BOOST_CHECK_NO_THROW
			(
				for( size_t i = 0; i < 500; ++i )
				{
					message.clear();
					do
					{
						message += in.get();
					} while( message[ message.length() - 1 ] != 0x03 );

					socket.send_to( boost::asio::buffer( message ), endpoint );	
					boost::this_thread::sleep_for( boost::chrono::milliseconds( 5 ) );
				}
				boost::this_thread::sleep_for( boost::chrono::milliseconds( 150 ) );
			);

		BOOST_CHECK_NO_THROW( receiver.stop() );

		BOOST_CHECK_NO_THROW( calc.stop() );

		for( std::set< std::string >::iterator it = delete_filenames.begin(); it != delete_filenames.end(); ++it )
			boost::filesystem::remove( *it );
	}


}