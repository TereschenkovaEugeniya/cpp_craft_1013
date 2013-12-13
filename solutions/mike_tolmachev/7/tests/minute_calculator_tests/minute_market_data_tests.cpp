#include "test_registrator.h"

#include <minute_market_data.h>
#include <boost/format.hpp>

namespace minute_calculator
{
namespace tests_
{

boost::mutex test_output_mtx;

void write_quote(const std::string& block, std::ofstream& test_output)
{
	boost::mutex::scoped_lock lock( test_output_mtx );

	const char us = 0x1F;
	const char soh = 0x01;
	const char etx = 0x03;
	std::string msg;
	for (auto it = block.begin(); it != block.end(); ++it)
	{
		if (*it == us || *it == etx)
		{
			multicast_communication::quote_message qm;
			qm.initialize(msg);

			test_output << std::fixed << "Q" << " "
				<< qm.security_symbol() << " "
				<< std::setprecision(2) << qm.bid_price() << " "
				<< std::setprecision(1) << qm.bid_volume() << " "
				<< std::setprecision(2) << qm.offer_price() << " "
				<< std::setprecision(1) << qm.offer_volume() << std::endl;
		
			msg.clear();
		}
		else if (*it != soh)
		{
			msg.push_back(*it);
		}

		if (*it == etx)
		{
			break;
		}
	}
	
}

void write_trade(const std::string& block, std::ofstream& test_output)
{
    boost::mutex::scoped_lock lock( test_output_mtx );

    const char us = 0x1F;
    const char soh = 0x01;
    const char etx = 0x03;
    std::string msg;
    for (auto it = block.begin(); it != block.end(); ++it)
    {
	    if (*it == us || *it == etx)
	    {
		    multicast_communication::trade_message tm;
		    tm.initialize(msg);

		    test_output << std::fixed << "T" << " "
			    << tm.security_symbol() << " "
			    << std::setprecision(2) << tm.price() << " "
			    << std::setprecision(1) << tm.volume() << std::endl;
	
		    msg.clear();
	    }
	    else if (*it != soh)
	    {
		    msg.push_back(*it);
	    }

	    if (*it == etx)
	    {
		    break;
	    }
    }

}

void service_thread(boost::asio::io_service& service)
{
	service.run();
}

void send_msg(const std::pair<std::string, unsigned short>& port, std::ofstream& test_output, bool is_trade)
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
        Sleep(100);
		if (is_trade)
		{
			write_trade(msg, test_output);
		}
		else
		{
			write_quote(msg, test_output);
		}
	}

	socket.close();
	service.stop();
	service_thread.join();
	
}

void minute_market_data_tests()
{	
	std::string output_dir = BINARY_DIR;
	std::string test_output_path = BINARY_DIR "/test.dat";

	std::ofstream test_output(test_output_path);

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
	
	
    minute_market_data mmd(4, 4, trade_ports, quote_ports, output_dir);
    
	boost::thread_group senders;
	for (auto it = trade_ports.begin(); it != trade_ports.end(); ++it)
	{
		senders.create_thread(boost::bind(&send_msg, *it, boost::ref(test_output), true));
	}
	for (auto it = quote_ports.begin(); it != quote_ports.end(); ++it)
	{
		senders.create_thread(boost::bind(&send_msg, *it, boost::ref(test_output), false));
	}
	senders.join_all();

    BOOST_CHECK_NO_THROW
    (
        mmd.stop();
        test_output.close();
    )
}

}
}