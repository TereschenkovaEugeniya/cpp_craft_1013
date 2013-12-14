#include <stdio.h>

#include "minute_calculator.h"

boost::mutex mtx_minute_datafeed;
boost::mutex mtx_quote_data;
boost::mutex mtx_trade_data;

minute_calculator::minute_calculator_process::minute_calculator_process()
{

}

minute_calculator::minute_calculator_process::~minute_calculator_process()
{
	stop();
}

void minute_calculator::minute_calculator_process::start()
{
	calculate_group.create_thread( boost::bind( &minute_calculator_process::quote_calculate, this ));
	calculate_group.create_thread( boost::bind( &minute_calculator_process::trade_calculate, this ));
}

void minute_calculator::minute_calculator_process::stop()
{
	calculate_group.interrupt_all();
}

void minute_calculator::minute_calculator_process::quote_calculate()
{
	while(1)
	{
		boost::this_thread::sleep( boost::posix_time::milliseconds( 10 ) );
		
		common_data::quote_data quote;
		{
			boost::mutex::scoped_lock lock( mtx_quote_data );
			if ( quote_datas.empty() )
				continue;
			std::vector < common_data::quote_data >::iterator iterator_quote_data = quote_datas.begin();
			quote = *iterator_quote_data;

			quote_datas.erase(iterator_quote_data);
		}
			
		vector_minute_datafeed::iterator iterator;
		bool has_data = false;
		boost::mutex::scoped_lock lock( mtx_minute_datafeed );
		for (iterator = minute_datafeeds.begin(); iterator != minute_datafeeds.end(); ++iterator)
		{
			if (std::strcmp((*iterator).stock_name, quote.security_symbol) == 0
				&& (*iterator).minute == quote.time / 60)
			{
				(*iterator).bid += quote.bid_price;
				(*iterator).ask += quote.offer_price;
				has_data = true;
				break;
			}
		}
		if (!has_data)
		{
			common_data::minute_datafeed datafeed;
			strcpy(datafeed.stock_name, quote.security_symbol);
			datafeed.minute = quote.time / 60;
			datafeed.bid = quote.bid_price;
			datafeed.ask = quote.offer_price;
			minute_datafeeds.push_back(datafeed);
		}
	}
}

void minute_calculator::minute_calculator_process::trade_calculate()
{
	while(1)
	{
		boost::this_thread::sleep( boost::posix_time::milliseconds( 10 ) );
		
		common_data::trade_data trade;
		{
			boost::mutex::scoped_lock lock( mtx_trade_data );
			if ( trade_datas.empty() )
				continue;
			std::vector < common_data::trade_data >::iterator iterator_trade_data = trade_datas.begin();
			trade = *iterator_trade_data;

			trade_datas.erase(iterator_trade_data);
		}

		vector_minute_datafeed::iterator iterator;
		bool has_data = false;
		boost::mutex::scoped_lock lock( mtx_minute_datafeed );
		for (iterator = minute_datafeeds.begin(); iterator != minute_datafeeds.end(); ++iterator)
		{
			if (std::strcmp((*iterator).stock_name, trade.security_symbol) == 0
				&& (*iterator).minute == trade.time / 60)
			{
				if ((*iterator).open_price == 0 )
					(*iterator).open_price = trade.price;
				if ((*iterator).high_price < trade.price 
					|| (*iterator).high_price == 0 )
					(*iterator).high_price = trade.price;
				if ((*iterator).low_price > trade.price
					|| (*iterator).low_price == 0 )
					(*iterator).low_price = trade.price;
				(*iterator).close_price = trade.price;
				(*iterator).volume += trade.price;
				has_data = true;
				break;
			}
		}
		if (!has_data)
		{
			common_data::minute_datafeed datafeed;
			strcpy(datafeed.stock_name, trade.security_symbol);
			datafeed.minute = trade.time / 60;
			datafeed.open_price = trade.price;
			datafeed.close_price = trade.price;
			datafeed.high_price = trade.price;
			datafeed.low_price = trade.price;
			datafeed.volume = trade.price;
			minute_datafeeds.push_back(datafeed);
		}
	}
}

void minute_calculator::minute_calculator_process::add_quote_data(const common_data::quote_data &quote)
{
	boost::mutex::scoped_lock lock( mtx_quote_data );
	if ( quote.bid_price == 0 || quote.offer_price == 0 || std::strcmp(quote.security_symbol, "") == 0 )
	{
		throw std::logic_error("empty quote data");
		return;
	}
	quote_datas.push_back(quote);
}

void minute_calculator::minute_calculator_process::add_trade_data(const common_data::trade_data &trade)
{
	boost::mutex::scoped_lock lock( mtx_trade_data );
	if ( trade.price == 0 || std::strcmp(trade.security_symbol, "") == 0 )
	{
		throw std::logic_error("empty trade data");
		return;
	}
	trade_datas.push_back(trade);
}

common_data::minute_datafeed minute_calculator::minute_calculator_process::get_minute_datafeed_one()
{
	vector_minute_datafeed::iterator iter;
	common_data::minute_datafeed data;

	std::map <std::string, common_data::minute_datafeed> datafeed_position;

	boost::mutex::scoped_lock lock( mtx_minute_datafeed );
	for (iter = minute_datafeeds.begin(); iter != minute_datafeeds.end(); ++iter)
	{
		if (datafeed_position.find(std::string(iter->stock_name)) == datafeed_position.end())
		{
			datafeed_position.insert(std::pair<std::string, common_data::minute_datafeed>
				(std::string(iter->stock_name), (*iter)));
		} else
		{
			data = datafeed_position[iter->stock_name];	
			break;
		}
	}
	if (data.minute != 0)
		minute_datafeeds.erase(iter);
	return data;
}

minute_calculator::minute_calculator_process::vector_minute_datafeed minute_calculator::minute_calculator_process::get_minute_datafeed()
{
	boost::mutex::scoped_lock lock( mtx_minute_datafeed );
	return minute_datafeeds;
}