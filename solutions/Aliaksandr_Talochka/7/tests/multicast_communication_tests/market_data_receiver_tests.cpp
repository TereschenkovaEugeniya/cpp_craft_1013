#include "test_registrator.h"

#include <market_data_receiver.h>
#include <market_data_sender.h>
#include <vector>

void multicast_communication::tests_::market_data_receiver_tests()
{
	{
		BOOST_CHECK_NO_THROW( multicast_communication::market_data_receiver Receiver(std::string(SOURCE_DIR"/sources/etc/configs.ini")));
	}
	{
		multicast_communication::market_data_receiver Receiver(std::string(SOURCE_DIR"/sources/etc/configs.ini"));
		BOOST_CHECK_EQUAL(Receiver.get_quote_messages().size(), 0);
		BOOST_CHECK_EQUAL(Receiver.get_trade_messages().size(), 0);
	}
	{
		multicast_communication::market_data_receiver Receiver(std::string(SOURCE_DIR"/sources/etc/configs.ini"));
		BOOST_CHECK_NO_THROW(Receiver.start());
		Sleep(1000);
		BOOST_CHECK_NO_THROW(Receiver.stop());
	}
	{
		BOOST_CHECK_NO_THROW( multicast_communication::market_data_sender Sender(std::string(SOURCE_DIR"/sources/etc/configs.ini"), std::string(SOURCE_DIR"/tests/data")));
	}
	{
		multicast_communication::market_data_sender Sender(std::string(SOURCE_DIR"/sources/etc/configs.ini"), std::string(SOURCE_DIR"/tests/data"));
		multicast_communication::market_data_receiver Receiver(std::string(SOURCE_DIR"/sources/etc/configs.ini"));
		BOOST_CHECK_NO_THROW( Sender.start());
		Receiver.start();
		Sleep(5000);
		std::vector <messages::quote_message> Quotes = Receiver.get_quote_messages();
		std::vector <messages::trade_message> Trades = Receiver.get_trade_messages();
		BOOST_CHECK_EQUAL((Quotes.size() == 0), false);
		BOOST_CHECK_EQUAL((Trades.size() == 0), false);
		BOOST_CHECK_NO_THROW(Sender.stop());
		Receiver.stop();
	}

}
