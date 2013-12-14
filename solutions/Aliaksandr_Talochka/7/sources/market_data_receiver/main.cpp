#include <iostream>

#include <string>

#include "market_data_receiver.h"
#include "market_data_sender.h"
#include "messages.h"


int main()
{
	try
	{

		multicast_communication::market_data_sender Sender(std::string(BINARY_DIR"/configs.ini"), std::string(BINARY_DIR));
		multicast_communication::market_data_receiver Receiver(std::string(BINARY_DIR"/configs.ini"));

		Sender.start();
		Receiver.start();

		Sleep(100);

		std::vector<messages::quote_message> Quotes = Receiver.get_quote_messages();
		std::vector<messages::trade_message> Trades = Receiver.get_trade_messages();

		Sender.stop();
		Receiver.stop();

	}
	catch (std::exception& e)
	{
		std::cout << "Exception: " << e.what() << "\n";
	}


	system("pause");
}
