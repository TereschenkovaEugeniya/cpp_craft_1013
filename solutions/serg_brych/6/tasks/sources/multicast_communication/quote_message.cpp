#include "quote_message.h"
#include "msg_utility.h"

multicast_communication::quote_message::quote_message():
	security_symbol_(""), bid_price_(0.0), bid_volume_(0.0),offer_price_(0.0), offer_volume_(0.0), type_(quote_type::unknown_quote)
{

}

bool multicast_communication::quote_message::parse_quote(std::istream &data)
{
	unsigned char msg_type;
	data.seekg(1, std::istream::cur);
	msg_type = data.get();

	switch(msg_type)
	{
	case quote_signature::sig_short_quote:
		parse_short_quote(data);
		type_ = quote_type::short_quote;
		return true;
	case quote_signature::sig_long_quote:
		parse_long_quote(data);
		type_ = quote_type::long_quote;
		return true;
	default:
		type_ = quote_type::unknown_quote;
		return false;
	}
}

void multicast_communication::quote_message::parse_short_quote(std::istream &data)
{
	data.seekg(sq_security_symbol_pos, std::istream::cur);
	security_symbol_ = get_string<sq_security_symbol_len>(data);
		
	data.seekg(sq_bid_price_denominator_indicator_pos, std::istream::cur);
		
	double bid_price_denominator = denominator_map.at(get_string<sq_denominato_len>(data)[0]);
	bid_price_ = get_number<sq_price_len>(data)/ bid_price_denominator;
	bid_volume_ = get_number<sq_volume_len>(data);

	data.seekg(sq_offer_price_denominator_indicator_pos, std::istream::cur);
		
	double offer_price_denominator = denominator_map.at(get_string<sq_denominato_len>(data)[0]);
	offer_price_ = get_number<sq_price_len>(data)/ offer_price_denominator;
	offer_volume_ = get_number<sq_volume_len>(data);

	data.seekg(sq_short_quote_end_pos, std::istream::cur);
}

void multicast_communication::quote_message::parse_long_quote(std::istream &data)
{
	data.seekg(lq_security_symbol_pos, std::istream::cur);
	security_symbol_ = get_string<lq_security_symbol_len>(data);

	data.seekg(lq_bid_price_denominator_indicator_pos, std::istream::cur);
		
	double bid_price_denominator = denominator_map.at(get_string<lq_denominato_len>(data)[0]);
	bid_price_ = get_number<lq_price_len>(data)/ bid_price_denominator;
	bid_volume_ = get_number<lq_volume_len>(data);

	data.seekg(lq_offer_price_denominator_indicator_pos, std::istream::cur);
		
	double offer_price_denominator = denominator_map.at(get_string<lq_denominato_len>(data)[0]);
	offer_price_ = get_number<lq_price_len>(data)/ offer_price_denominator;
	offer_volume_ = get_number<lq_volume_len>(data);

	data.seekg(lq_short_quote_end_pos, std::istream::cur);
}

std::string multicast_communication::quote_message::security_symbol() const
{
	return security_symbol_;
}

double multicast_communication::quote_message::bid_price() const
{
	return bid_price_;
}

double multicast_communication::quote_message::bid_volume() const
{
	return bid_volume_;
}

double multicast_communication::quote_message::offer_price() const
{
	return offer_price_;
}

double multicast_communication::quote_message::offer_volume() const
{
	return offer_volume_;
}

multicast_communication::quote_message::quote_type multicast_communication::quote_message::type() const
{
	return type_;
}

std::ostream& multicast_communication::operator<<( std::ostream& output, quote_message& msg )
{
   output  << std::fixed << "Q " << msg.security_symbol() << " " <<
        std::setprecision( 2 )  << msg.bid_price() << " " <<
        std::setprecision( 1 ) << msg.bid_volume() << " " <<
        std::setprecision( 2 ) << msg.offer_price() << " " <<
        std::setprecision( 1 ) << msg.offer_volume() << std::endl;
    return output;
}

bool multicast_communication::quote_message::parse_block(const std::string & data, quote_message_list_ptr &result)
{
	std::stringstream input(data, std::stringstream::in);
	quote_message_ptr message;
	char sep = input.get();
	if( sep != Signatures::StartMessage)
	{
		return false;
	}

	do
	{
		message.reset(new quote_message());
		message->parse_quote(input);
		if(message->type() != unknown_quote)
		{
			result.push_back(message);
		}
		sep = input.get();

		while(input)
		{
			if(sep == Signatures::SeparatorMessage)
				break;
			if(sep == Signatures::EndMessage)
				return true;

			sep = input.get();
		}
	}while(input && sep == Signatures::SeparatorMessage);

	return false;
}