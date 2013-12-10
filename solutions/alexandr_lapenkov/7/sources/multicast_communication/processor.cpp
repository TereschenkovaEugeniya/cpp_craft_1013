#include "processor.h"

namespace multicast_communication
{
	const char processor::SOH = 0x01;
	const char processor::EOT = 0x03;
	const char processor::US = 0x1F;

	processor::processor( const std::string& filename )
	{
		out.open( SOURCE_DIR + std::string( "/" ) + filename );
		quote_callback = boost::bind( &multicast_communication::processor::new_quote, this, _1 );
		trade_callback = boost::bind( &multicast_communication::processor::new_trade, this, _1 );
	}

	processor::processor( quote_callback_type quote_callback, trade_callback_type trade_callback )
		: quote_callback( quote_callback )
		, trade_callback( trade_callback )
	{}

	processor::~processor()
	{
		if( out.is_open() )
			out.close();
	}

	void processor::parse( std::string& message, message_type mt )
	{
		std::stringstream s( message );
		binreader in( s );
		bool in_block = false;
		while( in.good() )
		{
			if( !in_block )
			{
				const char c = in.get();
				if( !in.good() || !c )
					break;
				if( c != SOH )
					return ;
				in_block = true;
			}
			else
			{
				const char category = in.get();
				const char type = in.get();

				if( mt == QUOTE )
				{
					quote_message_ptr msg( new quote_message ( quote_callback ) );
					msg->parse( in, category, type );
				}
				else
				{
					trade_message_ptr msg( new trade_message ( trade_callback ) );
					msg->parse( in, category, type );
				}
				
				char after_message = in.get();
				while( after_message != EOT && after_message != US && in.good() )
					after_message = in.get();

				if( after_message == EOT )
					in_block = false;
				else
					return ;
			}
		}
	}

	void processor::new_trade( const trade_message_ptr& msg )
	{
		boost::mutex::scoped_lock lock( new_message_protector );
		out << std::fixed << "T " << msg->get_security_symbol() << " " << msg->get_price() << " " << msg->get_volume() << "\n";
	}

	void processor::new_quote( const quote_message_ptr& msg )
	{
		boost::mutex::scoped_lock lock( new_message_protector );
		out << std::fixed << "Q " << msg->get_security_symbol() << " " << msg->get_bid_price() << " " << msg->get_bid_volume() << " ";
		out << std::fixed << msg->get_offer_price() << " " << msg->get_offer_volume() << "\n";
	}
}