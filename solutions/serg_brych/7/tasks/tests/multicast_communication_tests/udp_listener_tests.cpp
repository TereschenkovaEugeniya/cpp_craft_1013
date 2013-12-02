#include "test_registrator.h"

#include <udp_listener.h>


namespace async_udp
{
	namespace tests_
	{
		namespace detail
		{
			void service_thread( boost::asio::io_service& service );
		}
	}
}

void async_udp::tests_::detail::service_thread( boost::asio::io_service& service )
{
	service.run();
}


void async_udp::tests_::udp_listner_tests()
{
	{	
		BOOST_CHECK_NO_THROW
		( 
			boost::asio::io_service service;
			udp_listener uw( service, "224.0.0.0", 50000, [](std::string &){});
		);
	}

	{
		boost::asio::io_service service;
		//std::string data("\x01""EDEO A  003759032T:J_073ADMR  B00004147006 B00004148004 12""\x03");

		std::string data("Hello");
		
		udp_listener uw( service, "224.0.0.1", 50000, [&](std::string &str)
		{
			BOOST_CHECK_EQUAL(str, data);
		});


		const std::string buffer( data );
		boost::asio::ip::udp::endpoint endpoint( boost::asio::ip::address::from_string( "224.0.0.1" ), 50000 ); 
		boost::asio::ip::udp::socket socket( service, endpoint.protocol() );
		boost::thread receive_messages( boost::bind( detail::service_thread, boost::ref( service ) ) );
		
		socket.send_to( boost::asio::buffer( buffer ), endpoint );
		boost::this_thread::sleep_for( boost::chrono::milliseconds( 3000 ) );
		
		service.stop();
		receive_messages.join();
		
	}


}