#include "communication.h"

#include <exception>
#include <string>
#include <boost/lexical_cast.hpp>

multicast_communication::communication::communication
			(	const size_t trade_thread_size,
				const size_t quote_thread_size,
				const std::vector< address >& trade_ports,
				const std::vector< address >& quote_ports,
				processor_ptr processor )
				: trade_thread_size( trade_thread_size )
				, quote_thread_size( quote_thread_size )
				, trade_ports( trade_ports )
				, quote_ports( quote_ports )
				, processor( processor )
				, interrupt( false )
		{}

multicast_communication::communication::~communication()
{}

void multicast_communication::communication::start()
{
	for( size_t i = 0; i < quote_thread_size; ++i )
		quote_threads.create_thread( boost::bind( &multicast_communication::communication::process_quote, this ) );

	for( size_t i = 0; i < trade_thread_size; ++i )
		trade_threads.create_thread( boost::bind( &multicast_communication::communication::process_trade, this ) );

	BOOST_FOREACH( const address& con_addr, quote_ports )
	{
		service_ptr io( new boost::asio::io_service() );
		connections.push_back( std::make_pair( io, async_udp::udp_listener_ptr( new async_udp::udp_listener( *io, con_addr.first, con_addr.second, *this, QUOTE ) ) ) );
		connection_threads.create_thread( boost::bind( &multicast_communication::communication::new_connection, this, io ) );
	}

	BOOST_FOREACH( const address& con_addr, trade_ports )
	{
		service_ptr io( new boost::asio::io_service() );
		connections.push_back( std::make_pair( io, async_udp::udp_listener_ptr( new async_udp::udp_listener( *io, con_addr.first, con_addr.second, *this, TRADE ) ) ) );
		connection_threads.create_thread( boost::bind( &multicast_communication::communication::new_connection, this, io ) );
	}

}

void multicast_communication::communication::new_connection( service_ptr io )
{
	io->run();
}

void multicast_communication::communication::stop()
{
	std::pair< service_ptr, async_udp::udp_listener_ptr > var;
	BOOST_FOREACH( var, connections )
	{
		var.first->stop();
	}
	{
		boost::mutex::scoped_lock lock( interrupt_protector );
		interrupt = true;
	}
	trade_queue.interrupt();
	quote_queue.interrupt();
	trade_threads.join_all();
	quote_threads.join_all();
}

void multicast_communication::communication::new_message( const std::string& msg, message_type mt )
{
	if( mt == TRADE )
		trade_queue.push( msg );
	else quote_queue.push( msg );
}

void multicast_communication::communication::process_trade()
{
	std::string str;
	while( true ) 
	{
		if( interrupt )
			return ;
		if( trade_queue.pop( str ) )
			processor->parse( str, TRADE );
		else
			return ;
	}
}

void multicast_communication::communication::process_quote()
{	
	std::string str;
	while( true )
	{
		if( interrupt )
			return ;
		if( quote_queue.pop( str ) )
			processor->parse( str, QUOTE );
		else
			return ;
	}
}

