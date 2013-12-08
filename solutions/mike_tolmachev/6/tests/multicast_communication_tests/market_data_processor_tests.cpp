#include "test_registrator.h"

#include <market_data_processor.h>
#include <market_data_processor_helper.h>
#include <market_data_receiver.h>
#include <boost/format.hpp>
#include <fstream>

namespace multicast_communication
{
	namespace tests_
	{
		class market_data_processor_test_helper : public market_data_processor
		{
		public:
			explicit market_data_processor_test_helper(){}
			virtual ~market_data_processor_test_helper(){}
		private:
			virtual void new_trade( const trade_message_ptr& );
			virtual void new_quote( const quote_message_ptr& );
		};
	}
}

void multicast_communication::tests_::market_data_processor_test_helper::new_trade( const trade_message_ptr& )
{
}
void multicast_communication::tests_::market_data_processor_test_helper::new_quote( const quote_message_ptr& )
{
}

void service_thread(boost::asio::io_service& service)
{
	service.run();
}

void send_msg(const std::pair<std::string, unsigned short>& port)
{
	std::string input_str((boost::format( SOURCE_DIR "/tests/data/%1%.udp" ) % port.first).str());
	std::ifstream input(input_str);

	boost::asio::io_service service;

	boost::asio::ip::udp::endpoint endpoint( boost::asio::ip::address::from_string(port.first), port.second );
	boost::asio::ip::udp::socket socket( service, endpoint.protocol() );
	boost::thread service_thread( boost::bind(service_thread, boost::ref(service)));

	while (input.good())
	{
		std::string msg;

		for (int i = 0; i < 1000; ++i)
		{
			char ch = input.get();
			msg.push_back(ch);
			if (ch == 0x3)
			{
				break;
			}
		}

		socket.send_to(boost::asio::buffer(msg), endpoint);
	}

	socket.close();
	service.stop();
	service_thread.join();
}

void multicast_communication::tests_::market_data_processor_tests()
{
	std::vector<std::pair<std::string, unsigned short> > trade_ports;
	std::vector<std::pair<std::string, unsigned short> > quote_ports;

	trade_ports.push_back(std::make_pair("233.200.79.128", 62128));
	trade_ports.push_back(std::make_pair("233.200.79.129", 62129));
	trade_ports.push_back(std::make_pair("233.200.79.130", 62130));
	trade_ports.push_back(std::make_pair("233.200.79.131", 62131));
	trade_ports.push_back(std::make_pair("233.200.79.132", 62132));
	trade_ports.push_back(std::make_pair("233.200.79.133", 62133));
	trade_ports.push_back(std::make_pair("233.200.79.134", 62134));
	trade_ports.push_back(std::make_pair("233.200.79.135", 62135));

	quote_ports.push_back(std::make_pair("233.200.79.0", 61000));
	quote_ports.push_back(std::make_pair("233.200.79.1", 61001));
	quote_ports.push_back(std::make_pair("233.200.79.2", 61002));
	quote_ports.push_back(std::make_pair("233.200.79.3", 61003));
	quote_ports.push_back(std::make_pair("233.200.79.4", 61004));
	quote_ports.push_back(std::make_pair("233.200.79.5", 61005));
	quote_ports.push_back(std::make_pair("233.200.79.6", 61006));
	quote_ports.push_back(std::make_pair("233.200.79.7", 61007));


	BOOST_CHECK_NO_THROW
	( 
		//market_data_processor_test_helper mdpth;
		market_data_processor_helper mdpth;
		market_data_receiver mdr(1, 1, trade_ports, quote_ports, mdpth);

		boost::thread_group senders;
		for (auto it = trade_ports.begin(); it != trade_ports.end(); ++it)
		{
			senders.create_thread(boost::bind(&send_msg, *it));
		}
		for (auto it = quote_ports.begin(); it != quote_ports.end(); ++it)
		{
			senders.create_thread(boost::bind(&send_msg, *it));
		}
		senders.join_all();

	)
}
