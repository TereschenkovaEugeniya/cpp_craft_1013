#include "test_registrator.h"
#include <market_data_receiver.h>
#include <data_processor.h>
#include <msg_utility.h>

#include <sstream>
#include <fstream>
#include <numeric>


namespace multicast_communication
{
	namespace tests_
	{
		namespace detail
		{
			void service_thread( boost::asio::io_service& service );
		}
		void send_quote_data(const address_and_port, size_t &);
		void send_trade_data(const address_and_port, size_t &);
		bool get_data(std::istream &, std::string &);
		
			
	};

	class market_data_processor_test_hlpr : public market_data_processor
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
		virtual void new_trade( const trade_message_ptr& );
		virtual void new_quote( const quote_message_ptr& );
		};
};

void multicast_communication::market_data_processor_test_hlpr::new_trade( const trade_message_ptr& trade)
{
	boost::lock_guard<boost::mutex> l_(mutex_);
	++count_processed_trades_;
}

void multicast_communication::market_data_processor_test_hlpr::new_quote( const quote_message_ptr& quote)
{
	boost::lock_guard<boost::mutex> l_(mutex_);
	++count_processed_quotes_;
}

size_t	multicast_communication::market_data_processor_test_hlpr::get_count_msg()
{
	return count_processed_trades_ + count_processed_quotes_;
}



void multicast_communication::tests_::detail::service_thread( boost::asio::io_service& service )
{
	service.run();
}


bool multicast_communication::tests_::get_data(std::istream &input, std::string & data)
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
void multicast_communication::tests_::send_quote_data(const address_and_port addr, size_t &count_sended_msg)
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
		
		quote_message_list_ptr quote_list;

		while(get_data(intup_data, data))
		{
			std::string buffer( data );
			
			socket.send_to( boost::asio::buffer( buffer ), endpoint );
			quote_message::parse_block(data, quote_list);
			data.clear();
						sended++;
		}

		count_sended_msg = quote_list.size();
		service.stop();
		receive_messages.join();
	}
}

void multicast_communication::tests_::send_trade_data(const address_and_port addr, size_t &count_sended_msg)
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
		
		trade_message_list_ptr trade_list;

		while(get_data(intup_data, data))
		{
			std::string buffer( data );
			socket.send_to( boost::asio::buffer( buffer ), endpoint );

			//boost::this_thread::sleep( boost::posix_time::milliseconds( 20 ));
			trade_message::parse_block(data, trade_list);
			data.clear();

		}

		count_sended_msg = trade_list.size();
		
		service.stop();
		socket.close();
		receive_messages.join();
	}
}

void multicast_communication::tests_::market_data_receiver_tests()
{
	/*const size_t trade_thread_size = 2;
	const size_t quote_thread_size = 2;
	addresses_and_ports trades_adr, quotes_adr;
	trades_adr.push_back(address_and_port("233.200.79.128",62128));
	trades_adr.push_back(address_and_port("233.200.79.129",62129));
	trades_adr.push_back(address_and_port("233.200.79.130",62130));
	trades_adr.push_back(address_and_port("233.200.79.131",62131));

	trades_adr.push_back(address_and_port("233.200.79.132",62132));
	trades_adr.push_back(address_and_port("233.200.79.133",62133));
	trades_adr.push_back(address_and_port("233.200.79.134",62134));
	trades_adr.push_back(address_and_port("233.200.79.135",62135));/*
		
	
	quotes_adr.push_back(address_and_port("233.200.79.0",61000));
	quotes_adr.push_back(address_and_port("233.200.79.1",61001));
	//quotes_adr.push_back(address_and_port("233.200.79.2",61002));
	//quotes_adr.push_back(address_and_port("233.200.79.3",61003));
	
	/*quotes_adr.push_back(address_and_port("233.200.79.4",61004));
	quotes_adr.push_back(address_and_port("233.200.79.5",61005));
	quotes_adr.push_back(address_and_port("233.200.79.6",61006));
	quotes_adr.push_back(address_and_port("233.200.79.7",61007));*/
		
	// test run/stop
	{
		const size_t trade_thread_size = 2;
		const size_t quote_thread_size = 2;
		addresses_and_ports trades_adr, quotes_adr;
		trades_adr.push_back(address_and_port("233.200.79.128",62128));
		quotes_adr.push_back(address_and_port("233.200.79.1",61001));
		config_reader* pcr = new config_reader(trade_thread_size, quote_thread_size, trades_adr, quotes_adr);

		multicast_communication::market_data_processor_test_hlpr processor;
		multicast_communication::market_data_receiver receiver(processor, pcr);
		
		BOOST_CHECK_NO_THROW(receiver.run());
		BOOST_CHECK_NO_THROW(receiver.stop());
	}

	// Send quote
	{
		const size_t trade_thread_size = 0;
		const size_t quote_thread_size = 2;
		addresses_and_ports trades_adr, quotes_adr;
		quotes_adr.push_back(address_and_port("233.200.79.0",61000));

		config_reader* pcr = new config_reader(trade_thread_size, quote_thread_size, trades_adr, quotes_adr);
		
		multicast_communication::market_data_processor_test_hlpr processor;
		multicast_communication::market_data_receiver receiver(processor, pcr);
		boost::thread_group sender_threads;
		size_t pkg_sended;
			
		receiver.run();
		send_quote_data(quotes_adr.front(), pkg_sended);
		boost::this_thread::sleep(boost::posix_time::milliseconds( 1000 ));

		receiver.stop();

		BOOST_CHECK_EQUAL(pkg_sended, processor.get_count_msg());
	}

	// Send trade
	{
		const size_t trade_thread_size = 2;
		const size_t quote_thread_size = 0;
		addresses_and_ports trades_adr, quotes_adr;
		trades_adr.push_back(address_and_port("233.200.79.128",62128));
	
		config_reader* pcr = new config_reader(trade_thread_size, quote_thread_size, trades_adr, quotes_adr);
		
		multicast_communication::market_data_processor_test_hlpr processor;
		multicast_communication::market_data_receiver receiver(processor, pcr);
		boost::thread_group sender_threads;
		size_t pkg_sended;
			
		receiver.run();
		send_trade_data(trades_adr.front(), pkg_sended);
		boost::this_thread::sleep(boost::posix_time::milliseconds( 1000 ));

		receiver.stop();

		BOOST_CHECK_EQUAL(pkg_sended, processor.get_count_msg());
	}

	{   // Test not passed lost udp pakages!!!!
		const size_t trade_thread_size = 16;
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


		config_reader* pcr = new config_reader(trade_thread_size, quote_thread_size, trades_adr, quotes_adr);

		multicast_communication::market_data_processor_test_hlpr processor;
		multicast_communication::market_data_receiver receiver(processor, pcr);
		boost::thread_group sender_threads;
		receiver.run();
		std::vector<size_t> all(trades_adr.size() + quotes_adr.size(), 0);
		size_t index = 0;
		
		for(auto i: trades_adr)
		{
			sender_threads.create_thread(boost::bind(send_trade_data, i, boost::ref(all[index])));
			index++;
		}
		for(auto i: quotes_adr)
		{
			sender_threads.create_thread(boost::bind(send_quote_data, i, boost::ref(all[index])));
			index++;
		}
		sender_threads.join_all();
		boost::this_thread::sleep( boost::posix_time::milliseconds( 2000 ));
		receiver.stop();
		
		BOOST_CHECK_EQUAL(std::accumulate(all.begin(), all.end(), 0), processor.get_count_msg());
	}
}