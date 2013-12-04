#include "market_data_receiver.h"

void service_thread( boost::asio::io_service& service )
{
	service.run();
}

int main()
{
    using namespace multicast_communication;
    boost::asio::io_service service;
    market_data_receiver receiver(service, "224.0.0.0", 50000);

    const std::string buffer( "hello world" );
	boost::asio::ip::udp::endpoint endpoint( boost::asio::ip::address::from_string( "224.0.0.0" ), 50000 ); 
	boost::asio::ip::udp::socket socket( service, endpoint.protocol() );
	boost::thread receive_messages( boost::bind( service_thread, boost::ref( service ) ) );
	while( receiver.messages().empty() )
	{
		socket.send_to( boost::asio::buffer( buffer ), endpoint );
		boost::this_thread::sleep_for( boost::chrono::nanoseconds( 1 ) );
	}
	service.stop();
	receive_messages.join();
}
