#ifndef _COMMON_DATA_H_
#define _COMMON_DATA_H_

#include <boost/cstdint.hpp>

namespace common_data
{
	struct quote_data
	{
		char security_symbol[24];
		boost::uint32_t time; // (значение времени в секундах
		double bid_price;
		double offer_price;

		quote_data()
			: time(0), bid_price(0), offer_price(0)
		{}
	};

	struct trade_data
	{
		char security_symbol[24];
		double price;
		boost::uint32_t time;

		trade_data()
			: price(0), time(0)
		{}
	};

	struct minute_datafeed
	{
		boost::uint32_t minute; //значение времени в минутах;
		char stock_name[24]; //
		double open_price;
		double high_price;
		double low_price;
		double close_price;
		double volume;
		double bid;
		double ask;

		minute_datafeed()
			: minute(0), open_price(0), high_price(0), low_price(0),
			close_price(0), volume(0), bid(0), ask(0)
		{}
	};
}

#endif