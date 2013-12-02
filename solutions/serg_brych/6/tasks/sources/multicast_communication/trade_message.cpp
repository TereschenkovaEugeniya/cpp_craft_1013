#include "trade_message.h"


multicast_communication::trade_message::trade_message():
	security_symbol_(""), price_(0.0), volume_(0.0), type_(trade_type::unknown_trade)
{
	
}

std::string multicast_communication::trade_message::security_symbol() const
{
	return security_symbol_;
}

double multicast_communication::trade_message::price() const
{
	return price_;
}

double multicast_communication::trade_message::volume() const
{
	return volume_;
}

multicast_communication::trade_message::trade_type multicast_communication::trade_message::type() const
{
	return type_;
}

bool multicast_communication::trade_message::parse_trade(std::istream &data)
{

	unsigned char msg_type;
	data.seekg(1, std::istream::cur);
	msg_type = data.get();

	switch(msg_type)
	{
	case trade_signature::sig_short_trade:
		parse_short_trade(data);
		type_ = trade_type::short_trade;
		return true;
	case trade_signature::sig_long_trade:
		parse_long_trade(data);
		type_ = trade_type::long_trade;
		return true;
	default:
		type_ = trade_type::unknown_trade;
		return false;
	}
	
}

void multicast_communication::trade_message::parse_short_trade(std::istream &data)
{
	data.seekg(st_security_symbol_pos, std::istream::cur);
	security_symbol_ = get_string<st_security_symbol_len>(data);
		
	data.seekg(st_trade_volume_pos, std::istream::cur);
	
	volume_ = get_number<st_trade_volume_len>(data);

	double denominator = denominator_map.at(get_string<st_price_denominator_indicator_len>(data)[0]);
	price_ = get_number<st_trade_price_len>(data)/ denominator;	

	data.seekg(st_trade_end_pos, std::istream::cur);

}

void multicast_communication::trade_message::parse_long_trade(std::istream &data)
{
	data.seekg(lt_security_symbol_pos, std::istream::cur);
	security_symbol_ = get_string<lt_security_symbol_len>(data);

	data.seekg(lt_price_denominator_indicator_pos, std::istream::cur);

	double denominator = denominator_map.at(get_string<lt_price_denominator_indicator_len>(data)[0]);
	price_ = get_number<lt_trade_price_len>(data)/ denominator;	
	
	volume_ = get_number<lt_trade_volume_len>(data);

	data.seekg(lt_trade_end_pos, std::istream::cur);
}

std::ostream& multicast_communication::operator<<( std::ostream& output, trade_message& msg )
{
    output  << std::fixed << "T " << msg.security_symbol() << " " << 
		std::setprecision( 2 )  << msg.price() << " " << 
		std::setprecision( 1 ) << msg.volume() << std::endl;
    return output;
}

bool multicast_communication::trade_message::parse_block(const std::string & data, trade_message_list_ptr &result)
{
	std::stringstream input(data, std::stringstream::in);
	trade_message_ptr message;
	char sep = input.get();
	if( sep != Signatures::StartMessage)
	{
		return false;
	}

	do
	{
		message.reset(new trade_message());
		message->parse_trade(input);
		if(message->type() != unknown_trade)
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