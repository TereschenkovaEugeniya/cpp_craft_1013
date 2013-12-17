#include "test_registrator.h"

#include <minute_calculator.h>

void minute_calculator::tests_::minute_calculator_tests()
{
	{
		minute_calculator::minute_calculator_process calc;
		BOOST_CHECK_THROW( calc.add_quote_data(common_data::quote_data()), std::logic_error );
		BOOST_CHECK_THROW( calc.add_trade_data(common_data::trade_data()), std::logic_error );

	}

	minute_calculator::minute_calculator_process calc;
	calc.start();

	BOOST_CHECK_NO_THROW
	(
		{
			common_data::quote_data quote;
			std::strcpy(quote.security_symbol, "aaaa");
			quote.time = 1 * 60;
			quote.bid_price = 5;
			quote.offer_price = 20;

			BOOST_CHECK_EQUAL( calc.get_minute_datafeed().size(), 0 );
			calc.add_quote_data(quote);
		}

		boost::this_thread::sleep( boost::posix_time::milliseconds( 100 ) );

		BOOST_CHECK_EQUAL( calc.get_minute_datafeed().size(), 1 );
		BOOST_CHECK_EQUAL( std::string(calc.get_minute_datafeed().front().stock_name), "aaaa" );
		BOOST_CHECK_EQUAL( calc.get_minute_datafeed().front().minute, 1 );
		BOOST_CHECK_EQUAL( calc.get_minute_datafeed().front().bid, 5 );
		BOOST_CHECK_EQUAL( calc.get_minute_datafeed().front().ask, 20 );
	)
	BOOST_CHECK_NO_THROW
	(
		{
			common_data::quote_data quote;
			std::strcpy(quote.security_symbol, "aaaa");
			quote.time = 1 * 60;
			quote.bid_price = 10;
			quote.offer_price = 200;

			calc.add_quote_data(quote);
		}

		boost::this_thread::sleep( boost::posix_time::milliseconds( 100 ) );

		BOOST_CHECK_EQUAL( calc.get_minute_datafeed().size(), 1 );
		BOOST_CHECK_EQUAL( std::string(calc.get_minute_datafeed().front().stock_name), "aaaa" );
		BOOST_CHECK_EQUAL( calc.get_minute_datafeed().front().minute, 1 );
		BOOST_CHECK_EQUAL( calc.get_minute_datafeed().front().bid, 15 );
		BOOST_CHECK_EQUAL( calc.get_minute_datafeed().front().ask, 220 );
	)
	BOOST_CHECK_NO_THROW
	(
		{
			common_data::quote_data quote;
			std::strcpy(quote.security_symbol, "aaaa");
			quote.time = 2 * 60;
			quote.bid_price = 10;
			quote.offer_price = 200;

			calc.add_quote_data(quote);
		}
		{
			common_data::trade_data trade;
			std::strcpy(trade.security_symbol, "aaaa");
			trade.time = 2 * 60;
			trade.price = 10;
			
			calc.add_trade_data(trade);
		}

		boost::this_thread::sleep( boost::posix_time::milliseconds( 100 ) );

		BOOST_CHECK_EQUAL( calc.get_minute_datafeed().size(), 2 );
		BOOST_CHECK_EQUAL( std::string((*(calc.get_minute_datafeed().begin() + 1)).stock_name), "aaaa" );
		BOOST_CHECK_EQUAL( (*(calc.get_minute_datafeed().begin() + 1)).minute, 2 );
		BOOST_CHECK_EQUAL( (*(calc.get_minute_datafeed().begin() + 1)).bid, 10 );
		BOOST_CHECK_EQUAL( (*(calc.get_minute_datafeed().begin() + 1)).ask, 200 );
		BOOST_CHECK_EQUAL( (*(calc.get_minute_datafeed().begin() + 1)).open_price, 10 );
		BOOST_CHECK_EQUAL( (*(calc.get_minute_datafeed().begin() + 1)).high_price, 10 );
		BOOST_CHECK_EQUAL( (*(calc.get_minute_datafeed().begin() + 1)).low_price, 10 );
		BOOST_CHECK_EQUAL( (*(calc.get_minute_datafeed().begin() + 1)).close_price, 10 );
		BOOST_CHECK_EQUAL( (*(calc.get_minute_datafeed().begin() + 1)).volume, 10 );
	)
	BOOST_CHECK_NO_THROW
	(
		{
			common_data::trade_data trade;
			std::strcpy(trade.security_symbol, "aaaa");
			trade.time = 2 * 60;
			trade.price = 100;
			
			calc.add_trade_data(trade);
		}
		{
			common_data::trade_data trade;
			std::strcpy(trade.security_symbol, "aaaa");
			trade.time = 2 * 60;
			trade.price = 90;
			
			calc.add_trade_data(trade);
		}

		boost::this_thread::sleep( boost::posix_time::milliseconds( 100 ) );

		BOOST_CHECK_EQUAL( calc.get_minute_datafeed().size(), 2 );
		BOOST_CHECK_EQUAL( std::string((*(calc.get_minute_datafeed().begin() + 1)).stock_name), "aaaa" );
		BOOST_CHECK_EQUAL( (*(calc.get_minute_datafeed().begin() + 1)).minute, 2 );
		BOOST_CHECK_EQUAL( (*(calc.get_minute_datafeed().begin() + 1)).bid, 10 );
		BOOST_CHECK_EQUAL( (*(calc.get_minute_datafeed().begin() + 1)).ask, 200 );
		BOOST_CHECK_EQUAL( (*(calc.get_minute_datafeed().begin() + 1)).open_price, 10 );
		BOOST_CHECK_EQUAL( (*(calc.get_minute_datafeed().begin() + 1)).high_price, 100 );
		BOOST_CHECK_EQUAL( (*(calc.get_minute_datafeed().begin() + 1)).low_price, 10 );
		BOOST_CHECK_EQUAL( (*(calc.get_minute_datafeed().begin() + 1)).close_price, 90 );
		BOOST_CHECK_EQUAL( (*(calc.get_minute_datafeed().begin() + 1)).volume, 200 );

		calc.stop();
	)
}
