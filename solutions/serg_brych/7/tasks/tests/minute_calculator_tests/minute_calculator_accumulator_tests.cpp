#include "test_registrator.h"
#include "minute_calculator_accumulator.h"


void minute_calculator::tests_::minute_calculator_accumulator_tests()
{
	minute_calculator::minute_calculator_accumulator ac;
	ac.run();
	{
		std::stringstream ss("EIAO A  000140208P:3\\806BGS@0100D00330400DF ");
		std::stringstream ss1("EIAO A  000140208P:3\\806BGS@0100D00330400DF ");

		multicast_communication::trade_message_ptr trade(new multicast_communication::trade_message);
		multicast_communication::trade_message_ptr trade1(new multicast_communication::trade_message);
		trade->parse_trade(ss);
		trade1->parse_trade(ss1);

		ac.push_trade(trade);
		ac.push_trade(trade1);
	}


	boost::this_thread::sleep(boost::posix_time::microseconds(2000));
	ac.stop();
}
