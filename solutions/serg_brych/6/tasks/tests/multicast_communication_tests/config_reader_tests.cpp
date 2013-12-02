#include "test_registrator.h"


#include <boost/filesystem.hpp>
#include <config_reader.h>

void multicast_communication::tests_::config_reader_tests()
{

	const size_t trade_thread_size = 8;
	const size_t quote_thread_size = 16;
	addresses_and_ports trades_adr, quotes_adr;
	trades_adr.push_back(address_and_port("233.200.79.128",62128));
	trades_adr.push_back(address_and_port("233.200.79.129",62129));
	trades_adr.push_back(address_and_port("233.200.79.130",62130));
	trades_adr.push_back(address_and_port("233.200.79.131",62131));
	trades_adr.push_back(address_and_port("233.200.79.132",62132));
	trades_adr.push_back(address_and_port("233.200.79.133",62133));
	trades_adr.push_back(address_and_port("233.200.79.134",62134));
	trades_adr.push_back(address_and_port("233.200.79.135",62135));

	quotes_adr.push_back(address_and_port("233.200.79.0",61000));
	quotes_adr.push_back(address_and_port("233.200.79.1",61001));
	quotes_adr.push_back(address_and_port("233.200.79.2",61002));
	quotes_adr.push_back(address_and_port("233.200.79.3",61003));
	quotes_adr.push_back(address_and_port("233.200.79.4",61004));
	quotes_adr.push_back(address_and_port("233.200.79.5",61005));
	quotes_adr.push_back(address_and_port("233.200.79.6",61006));
	quotes_adr.push_back(address_and_port("233.200.79.7",61007));



	{
		BOOST_CHECK_NO_THROW
		(
			config_reader cr(trade_thread_size, quote_thread_size, trades_adr, quotes_adr);

			BOOST_CHECK_EQUAL( cr.get_trade_thread_size(), trade_thread_size );
			BOOST_CHECK_EQUAL( cr.get_quote_thread_size(), quote_thread_size );

			BOOST_CHECK_EQUAL( cr.get_trade_adr_and_ports().size(), trades_adr.size());
			BOOST_CHECK_EQUAL( cr.get_quote_adr_and_ports().size(), quotes_adr.size());
			
		)
	}
	
	{
		std::string test_file_path = BINARY_DIR "/config.ini";
		std::ofstream config_;
		config_.open(test_file_path, std::ios::out);
		if(config_.is_open())
		{
			config_ << trade_thread_size << std::endl;
			config_ << quote_thread_size << std::endl;

			config_ << trades_adr.size() << std::endl;
			for(auto el: trades_adr)
			{
				config_ << el.first << " " << el.second << std::endl;
			}

			config_ << quotes_adr.size() << std::endl;
			for(auto el: quotes_adr)
			{
				config_ << el.first << " " << el.second << std::endl;
			}
			config_.close();
		}
		

		BOOST_CHECK_NO_THROW
		(
			config_reader cr(test_file_path);

			BOOST_CHECK_EQUAL( cr.get_trade_thread_size(), trade_thread_size );
			BOOST_CHECK_EQUAL( cr.get_quote_thread_size(), quote_thread_size );

			BOOST_CHECK_EQUAL( cr.get_trade_adr_and_ports().size(), trades_adr.size());
			BOOST_CHECK_EQUAL( cr.get_quote_adr_and_ports().size(), quotes_adr.size());
			
		)
		boost::filesystem::remove( test_file_path );
	}
	
	
}