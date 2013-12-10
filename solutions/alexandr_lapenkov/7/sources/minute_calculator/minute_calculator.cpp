#include "minute_calculator.h"

minute_calculator::datafeed_calculator::datafeed_calculator( datafeed_callback_type handler )
	: datafeed_handler( handler )
{}

void minute_calculator::datafeed_calculator::new_trade( const multicast_communication::trade_message_ptr& msg )
{
	std::string stock_name = msg->get_security_symbol();
	double price = msg->get_price();

	if( data.count( stock_name ) != 0 && data[stock_name]->minute > msg->get_minute() )
		return ;

	if( data.count( stock_name ) != 0 && data[stock_name]->minute == msg->get_minute() )
	{
		minute_datafeed_ptr& current_data = data[stock_name]; 
		
		current_data->high_price = std::max( current_data->high_price, price );
		current_data->low_price = std::min( current_data->low_price, price );

		current_data->close_price = price;
		current_data->volume += msg->get_volume();
	}
	else
	{
		if( data.count( stock_name ) != 0 )
		{
			datafeed_handler( data[stock_name] );
			data.erase( stock_name );
		}
		data.insert( std::make_pair( stock_name, minute_datafeed_ptr( new minute_datafeed( stock_name ) ) ) );
		minute_datafeed_ptr& current_data = data[stock_name]; 
		current_data->open_price = price;
		current_data->high_price = price;
		current_data->low_price = price;
		current_data->close_price = price;
		current_data->volume = msg->get_volume();
	}
}

void minute_calculator::datafeed_calculator::new_quote( const multicast_communication::quote_message_ptr& msg )
{
	std::string stock_name = msg->get_security_symbol();

	if( data.count( stock_name ) != 0 && data[stock_name]->minute > msg->get_minute() )
		return ;

	if( data.count( stock_name ) != 0 && data[stock_name]->minute == msg->get_minute() )
	{
		minute_datafeed_ptr& current_data = data[stock_name]; 
		
		current_data->ask += msg->get_offer_volume();
		current_data->bid += msg->get_bid_volume();
	}
	else
	{
		if( data.count( stock_name ) != 0 )
		{
			datafeed_handler( data[stock_name] );
			data.erase( stock_name );
		}
		data.insert( std::make_pair( stock_name, minute_datafeed_ptr( new minute_datafeed( stock_name ) ) ) );
		minute_datafeed_ptr& current_data = data[stock_name]; 

		current_data->ask = msg->get_offer_volume();
		current_data->bid = msg->get_bid_volume();
	}
}

void minute_calculator::datafeed_calculator::stop()
{
	std::pair< std::string, minute_datafeed_ptr > var;
	BOOST_FOREACH( var, data )
		datafeed_handler( var.second );
}