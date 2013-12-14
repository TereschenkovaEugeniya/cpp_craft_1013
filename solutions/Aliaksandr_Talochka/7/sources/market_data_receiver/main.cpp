#include <iostream>

#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "market_data_receiver.h"
#include "udp_listener.h"
#include "udp_sender.h"
#include "messages.h"

void run_service(boost::asio::io_service& service_)
{
	service_.run();
}

int main()
{
	try
	{
		boost::asio::io_service io_service_sender;
		boost::thread_group thread_group_sender;
		
		multicast_communication::sender senderQ1(io_service_sender, std::string("233.200.79.0"), 61000, std::string(BINARY_DIR"/233.200.79.0.udp"), 5000);
		multicast_communication::sender senderQ2(io_service_sender, std::string("233.200.79.1"), 61001, std::string(BINARY_DIR"/233.200.79.1.udp"), 5000);
		multicast_communication::sender senderT1(io_service_sender, std::string("233.200.79.128"), 62128, std::string(BINARY_DIR"/233.200.79.128.udp"), 5000);
		multicast_communication::sender senderT2(io_service_sender, std::string("233.200.79.129"), 62129, std::string(BINARY_DIR"/233.200.79.129.udp"), 5000);

		for(size_t i = 0; i<4; i++)
		{
			thread_group_sender.create_thread(boost::bind(&run_service, boost::ref(io_service_sender)));
		}


		multicast_communication::market_data_receiver Receiver(std::string(BINARY_DIR"/configs.ini"));
		Receiver.start();

		Sleep(1000);

		io_service_sender.stop();
		thread_group_sender.join_all();

		std::vector<messages::quote_message> Quotes = Receiver.get_quote_messages();
		std::vector<messages::trade_message> Trades = Receiver.get_trade_messages();

		Receiver.stop();

		/*boost::asio::io_service io_service_listener;
		boost::thread_group thread_group_listener; 

		
		multicast_communication::udp_listener mc0(io_service_listener, "233.200.79.0", 61000, 'Q');
		multicast_communication::udp_listener mc1(io_service_listener, "233.200.79.1", 61001, 'Q');

		for(size_t i = 0; i<2; i++)
		{
			thread_group_sender.create_thread(boost::bind(&run_service, boost::ref(io_service_listener)));
		}
		*/
	}
	catch (std::exception& e)
	{
		std::cout << "Exception: " << e.what() << "\n";
	}


	system("pause");
}
