#include <cqs_parser.h>
#include <cqs_protocol.h>

multicast_communication::cqs_parser::cqs_parser( quote_processor& processor )
: quote_processor_( processor )
{
}

multicast_communication::cqs_parser::~cqs_parser()
{
}

void multicast_communication::cqs_parser::process_message( const common::buffer_ptr new_message, const size_t size )
{
	using namespace cqs_protocol;

	if ( size > common::buffer::size )
		return;

	const char* parsing_ptr = new_message->data;

	if ( *parsing_ptr != cqs_parser::SOH )
		return;
	if ( *(parsing_ptr + size - 1) != cqs_parser::ETX )
		return;

	if ( size < sizeof( message_header ) )
		return;

	filter_messages_( parsing_ptr + 1, size - 2 ); // excluding SOH and ETX
}
void multicast_communication::cqs_parser::filter_messages_( const char* parsing_ptr, size_t size )
{
	using namespace cqs_protocol;

	while ( size > 0 )
	{
		const char* after_parsing_ptr = parsing_ptr + size;

		const message_header* const mh = reinterpret_cast< const message_header* const >( parsing_ptr );

		if ( mh->message_type[0] == 'D' )
		{
			if ( mh->message_category[0] == 'E' || mh->message_category[0] == 'L' )
				after_parsing_ptr = parse_short_quote_( parsing_ptr, size );
		}
		else
		if ( mh->message_type[0] == 'B' )
		{
			if ( mh->message_category[0] == 'B' || mh->message_category[0] == 'E' || mh->message_category[0] == 'L' )
				after_parsing_ptr = parse_long_quote_( parsing_ptr, size );
		}
		else 
			after_parsing_ptr = std::find( parsing_ptr, after_parsing_ptr, cqs_parser::US );

		size_t diff = after_parsing_ptr - parsing_ptr;
		if ( diff > size )
			diff = size;
		if ( *after_parsing_ptr == cqs_parser::US )
		{
			diff += 1; // cts_parser::US
			parsing_ptr = after_parsing_ptr + 1; // cts_parser::US
		}
		else
			parsing_ptr = after_parsing_ptr;
		size -= diff;
	}
}

const char* multicast_communication::cqs_parser::parse_short_quote_( const char* parsing_ptr, size_t )
{
	const cqs_protocol::message_header* const mh = reinterpret_cast< const cqs_protocol::message_header* const >( parsing_ptr );
	const cqs_protocol::short_quote* const sq = reinterpret_cast< const cqs_protocol::short_quote* const >( parsing_ptr + sizeof( cqs_protocol::message_header ) );
	try
	{
		quote_message_ptr new_message( new quote_message( *mh, *sq ) );
		quote_processor_.new_quote( new_message );
	}
	catch( const std::exception& )
	{
		// skip bad message
	}

	return parsing_ptr + sizeof( cqs_protocol::message_header ) + sizeof( cqs_protocol::short_quote ) + sizeof_appendages( *sq );
}
const char* multicast_communication::cqs_parser::parse_long_quote_( const char* parsing_ptr, size_t )
{
	const cqs_protocol::message_header* const mh = reinterpret_cast< const cqs_protocol::message_header* const >( parsing_ptr );
	const cqs_protocol::long_quote* const lq = reinterpret_cast< const cqs_protocol::long_quote* const >( parsing_ptr + sizeof( cqs_protocol::message_header ) );

	try
	{
		quote_message_ptr new_message( new quote_message( *mh, *lq ) );
		quote_processor_.new_quote( new_message );
	}
	catch( const std::exception& )
	{
		// skip bad message
	}

	return parsing_ptr + sizeof( cqs_protocol::message_header ) + sizeof( cqs_protocol::long_quote ) + sizeof_appendages( *lq );
}
