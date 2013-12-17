#include <cts_parser.h>
#include <cts_protocol.h>
//
multicast_communication::cts_parser::cts_parser( trade_processor& processor )
 : trade_processor_( processor )
{
}

multicast_communication::cts_parser::~cts_parser()
{
}

void multicast_communication::cts_parser::process_message( const common::buffer_ptr new_message, const size_t size )
{
	using namespace cts_protocol;

	if ( size > common::buffer::size )
		return;

	const char* parsing_ptr = new_message->data;

	if ( *parsing_ptr != cts_parser::SOH )
		return;
	if ( *(parsing_ptr + size - 1) != cts_parser::ETX )
		return;

	if ( size < sizeof( message_header ) )
		return;

	filter_messages_( parsing_ptr + 1, size - 2 ); // excluding SOH and ETX
}
void multicast_communication::cts_parser::filter_messages_( const char* parsing_ptr, size_t size )
{
	using namespace cts_protocol;

	while ( size > 0 )
	{
		const char* after_parsing_ptr = parsing_ptr + size;

		const message_header* const mh = reinterpret_cast< const message_header* const >( parsing_ptr );

		if ( mh->message_type[0] == 'I' )
		{
			if ( mh->message_category[0] == 'E' || mh->message_category[0] == 'L' )
				after_parsing_ptr = parse_short_trade_( parsing_ptr, size );
		}
		else
		if ( mh->message_type[0] == 'B' )
		{
			if ( mh->message_category[0] == 'B' || mh->message_category[0] == 'E' || mh->message_category[0] == 'L' )
				after_parsing_ptr = parse_long_trade_( parsing_ptr, size );
		}
		else 
			after_parsing_ptr = std::find( parsing_ptr, after_parsing_ptr, cts_parser::US );

		size_t diff = after_parsing_ptr - parsing_ptr;
		if ( *after_parsing_ptr == cts_parser::US )
		{
			diff += 1; // cts_parser::US
			parsing_ptr = after_parsing_ptr + 1; // cts_parser::US
		}
		else
			parsing_ptr = after_parsing_ptr;
		size -= diff;
	}
}

const char* multicast_communication::cts_parser::parse_short_trade_( const char* parsing_ptr, size_t )
{
	const cts_protocol::message_header* const mh = reinterpret_cast< const cts_protocol::message_header* const >( parsing_ptr );
	const cts_protocol::short_trade* const st = reinterpret_cast< const cts_protocol::short_trade* const >( parsing_ptr + sizeof( cts_protocol::message_header ) );
	try
	{
		trade_message_ptr new_message( new trade_message( *mh, *st ) );
		trade_processor_.new_trade( new_message );
	}
	catch( const std::exception& )
	{
		// skip bad message
	}

	return parsing_ptr + sizeof( cts_protocol::message_header ) + sizeof( cts_protocol::short_trade );
}
const char* multicast_communication::cts_parser::parse_long_trade_( const char* parsing_ptr, size_t )
{
	const cts_protocol::message_header* const mh = reinterpret_cast< const cts_protocol::message_header* const >( parsing_ptr );
	const cts_protocol::long_trade* const lt = reinterpret_cast< const cts_protocol::long_trade* const >( parsing_ptr + sizeof( cts_protocol::message_header ) );

	try
	{
		trade_message_ptr new_message( new trade_message( *mh, *lt ) );
		trade_processor_.new_trade( new_message );
	}
	catch( const std::exception& )
	{
		// skip bad message
	}

	return parsing_ptr + sizeof( cts_protocol::message_header ) + sizeof( cts_protocol::long_trade );
}
