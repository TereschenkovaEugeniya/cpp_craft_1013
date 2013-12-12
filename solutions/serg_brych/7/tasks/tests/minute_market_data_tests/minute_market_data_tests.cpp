#include <boost/filesystem.hpp>
#include <fstream>

#include <market_data_receiver.h>
#include "test_registrator.h"
#include <minute_market_data_processor.h>


static const std::string test_file_path = BINARY_DIR "/config.ini";

namespace minute_market_data
{
	namespace tests_
	{
		namespace detail
		{
			void service_thread( boost::asio::io_service& service );
		}
		void send_quote_data(const multicast_communication::address_and_port, size_t &);
		void send_trade_data(const multicast_communication::address_and_port, size_t &);
		bool get_data(std::istream &, std::string &);
		
			
	};

	class market_data_processor_test_hlpr : public multicast_communication::market_data_processor
	{
		friend class market_data_receiver;
		boost::mutex mutex_;
		size_t count_processed_trades_;
		size_t count_processed_quotes_;
	public:
		explicit market_data_processor_test_hlpr():
			count_processed_trades_(0), count_processed_quotes_(0)
		{
			
		}
		virtual ~market_data_processor_test_hlpr(){}
		size_t	get_count_msg();

	private:
		virtual void new_trade( const multicast_communication::trade_message_ptr& );
		virtual void new_quote( const multicast_communication::quote_message_ptr& );
		};
};

void minute_market_data::market_data_processor_test_hlpr::new_trade( const multicast_communication::trade_message_ptr& trade)
{
	boost::lock_guard<boost::mutex> l_(mutex_);
	++count_processed_trades_;
}

void minute_market_data::market_data_processor_test_hlpr::new_quote( const multicast_communication::quote_message_ptr& quote)
{
	boost::lock_guard<boost::mutex> l_(mutex_);
	++count_processed_quotes_;
}

size_t minute_market_data::market_data_processor_test_hlpr::get_count_msg()
{
	return count_processed_trades_ + count_processed_quotes_;
}



void minute_market_data::tests_::detail::service_thread( boost::asio::io_service& service )
{
	service.run();
}


bool minute_market_data::tests_::get_data(std::istream &input, std::string & data)
{
	char ch = input.get();
	data.clear();
	if( ch == Signatures::StartMessage)
	{
		do
		{
			data.push_back(ch);
			ch = input.get();
			if(ch == Signatures::EndMessage)
			{
				data.push_back(ch);
				return true;
			}
		}while(input);
	}
	return false;
}
static size_t sended = 0;

void minute_market_data::tests_::send_quote_data(const multicast_communication::address_and_port addr, size_t &count_sended_msg)
{
	count_sended_msg = 0;
	boost::asio::io_service service;
	std::string data;
	std::stringstream input_file_path;
	input_file_path << TEST_DATA_DIR << "/" <<addr.first << ".udp";
	std::ifstream intup_data(input_file_path.str());
	if(intup_data.is_open())
	{
		boost::asio::ip::udp::endpoint endpoint( boost::asio::ip::address::from_string( addr.first ), addr.second ); 
		boost::asio::ip::udp::socket socket( service, endpoint.protocol() );
		boost::thread receive_messages( boost::bind( detail::service_thread, boost::ref( service ) ) );
		
		multicast_communication::quote_message_list_ptr quote_list;

		while(get_data(intup_data, data))
		{
			std::string buffer( data );
			
			socket.send_to( boost::asio::buffer( buffer ), endpoint );
			multicast_communication::quote_message::parse_block(data, quote_list);
			data.clear();
						sended++;
		}

		count_sended_msg = quote_list.size();
		service.stop();
		receive_messages.join();
	}
}

void minute_market_data::tests_::send_trade_data(const multicast_communication::address_and_port addr, size_t &count_sended_msg)
{
	count_sended_msg = 0;
	boost::asio::io_service service;
	std::string data;
	std::stringstream input_file_path;
	input_file_path << TEST_DATA_DIR << "/" <<addr.first << ".udp";
	std::ifstream intup_data(input_file_path.str());
	if(intup_data.is_open())
	{
		boost::asio::ip::udp::endpoint endpoint( boost::asio::ip::address::from_string( addr.first ), addr.second ); 
		boost::asio::ip::udp::socket socket( service, endpoint.protocol() );
		boost::thread receive_messages( boost::bind( detail::service_thread, boost::ref( service ) ) );
		
		multicast_communication::trade_message_list_ptr trade_list;

		while(get_data(intup_data, data))
		{
			std::string buffer( data );
			socket.send_to( boost::asio::buffer( buffer ), endpoint );

			//boost::this_thread::sleep( boost::posix_time::milliseconds( 20 ));
			multicast_communication::trade_message::parse_block(data, trade_list);
			data.clear();

		}

		count_sended_msg = trade_list.size();
		
		service.stop();
		socket.close();
		receive_messages.join();
	}
}



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
	}

	{
		using namespace multicast_communication;
		const size_t trade_thread_size = 1;
		const size_t quote_thread_size = 1;
		multicast_communication::addresses_and_ports trades_adr, quotes_adr;
	
		trades_adr.push_back(address_and_port("233.200.79.128",62128));
		quotes_adr.push_back(address_and_port("233.200.79.0",61000));
		
		bool result = create_config_file(trade_thread_size, quote_thread_size, trades_adr, quotes_adr);

		BOOST_CHECK_EQUAL(result, true);

		BOOST_CHECK_NO_THROW
		(
			minute_market_data::minute_market_data_processor processor;
			size_t pkg_sended;
			processor.run();	
		
			send_trade_data(trades_adr.front(), pkg_sended);
			boost::this_thread::sleep(boost::posix_time::milliseconds( 3000 ));
			processor.stop();
		);
	
	}

	
	boost::filesystem::remove( test_file_path );
}
