#include <boost/filesystem.hpp>
#include <fstream>


#include "test_registrator.h"
#include <minute_market_data_processor.h>


static const std::string test_file_path = BINARY_DIR "/config.ini";



bool create_config_file(const size_t trade_thread_size, const size_t quote_thread_size, 
						const multicast_communication::addresses_and_ports &trades_adr, 
						const multicast_communication::addresses_and_ports &quotes_adr)
{
	
	
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
		return true;
	}
	return false;
}

void minute_market_data::tests_::minute_market_data_processor_tests()
{
	using namespace multicast_communication;
	const size_t trade_thread_size = 16;
	const size_t quote_thread_size = 16;
	multicast_communication::addresses_and_ports trades_adr, quotes_adr;
	
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

	bool result = create_config_file(trade_thread_size, quote_thread_size, trades_adr, quotes_adr);

	BOOST_CHECK_EQUAL(result, true);

	BOOST_CHECK_NO_THROW
	(
		minute_market_data::minute_market_data_processor processor;
		processor.run();
		boost::this_thread::sleep(boost::posix_time::milliseconds( 1000 ));
	);

	
	boost::filesystem::remove( test_file_path );
}
