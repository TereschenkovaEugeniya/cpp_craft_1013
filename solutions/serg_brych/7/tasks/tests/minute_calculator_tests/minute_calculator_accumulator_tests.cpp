#include "test_registrator.h"
#include "minute_calculator_accumulator.h"


void minute_calculator::tests_::minute_calculator_accumulator_tests()
{
	minute_calculator::container_minute_data_ptr datafeed;
	
	minute_calculator::minute_calculator_accumulator ac([&](minute_calculator::container_minute_data_ptr data_ptr)
	{
		datafeed = data_ptr;
	});
	ac.run();
	{
		std::stringstream ss("EIAO A  000140208P:3\\806BGS@0100D00330400DF ");
		std::stringstream ss1("EIAO A  000140208P:3\\806BGS@0100D00600000DF ");
		std::stringstream ss2("EIAO A  000140208P:5\\806BGS@0100D00330400DF ");

		multicast_communication::trade_message_ptr trade(new multicast_communication::trade_message);
		multicast_communication::trade_message_ptr trade1(new multicast_communication::trade_message);
		multicast_communication::trade_message_ptr trade2(new multicast_communication::trade_message);
		trade->parse_trade(ss);
		trade1->parse_trade(ss1);
		trade2->parse_trade(ss2);

		ac.push_trade(trade);
		ac.push_trade(trade1);
		ac.push_trade(trade2);
	}
	boost::this_thread::sleep(boost::posix_time::microseconds(2000));
	
	BOOST_CHECK_EQUAL(datafeed->at("BGS").stock_name, "BGS");
	BOOST_CHECK_EQUAL(datafeed->at("BGS").low_price, 33.04);
	BOOST_CHECK_EQUAL(datafeed->at("BGS").high_price, 60.0);
	BOOST_CHECK_EQUAL(datafeed->at("BGS").open_price, 33.04);
	BOOST_CHECK_EQUAL(datafeed->at("BGS").close_price, 60.0);
	BOOST_CHECK_EQUAL(datafeed->at("BGS").volume, 60.0 + 33.04);
	BOOST_CHECK_EQUAL(datafeed->at("BGS").ask, 0.0);
	BOOST_CHECK_EQUAL(datafeed->at("BGS").bid, 0.0);
	ac.stop();
}
