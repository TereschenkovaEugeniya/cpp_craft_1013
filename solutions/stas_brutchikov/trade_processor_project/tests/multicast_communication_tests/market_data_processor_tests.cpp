#include "test_registrator.h"

#include <fstream>

#include <boost/thread/recursive_mutex.hpp>
#include <boost/assign/std/vector.hpp>

#include "market_data_reciever.h"
#include "market_data_processor.h"

namespace multicast_communication
{
	namespace tests_
	{
		class market_data_processor_test_helper : public market_data_processor
		{
		public:
			explicit market_data_processor_test_helper():count_trades_(0), count_quotes_(0){}
			virtual ~market_data_processor_test_helper(){}
            size_t count_trades_;
            size_t count_quotes_;
		private:
			virtual void new_trade( const trade_message_ptr& );
			virtual void new_quote( const quote_message_ptr& );
            boost::recursive_mutex mutex_;

            typedef boost::recursive_mutex::scoped_lock locker;
		};
	}

void tests_::market_data_processor_test_helper::new_trade( const trade_message_ptr& )
{
	locker lock( mutex_ );
	++count_trades_;
}
void tests_::market_data_processor_test_helper::new_quote( const quote_message_ptr& )
{
	locker lock( mutex_ );
	++count_quotes_;
}

bool prepare_message(std::istream &input, std::string & data)
{
	char c = input.get();
	data.clear();
	if( c != SOH )
	{
        return false;
    }
	do
	{
		data.push_back( c );
		c = input.get();
	}while( input && c != ETX );

	if( c == ETX )
	{
		data.push_back( c );
		return true;
	}
    
	return false;
}



void work_thread( boost::asio::io_service& service )
{
    service.run();
}


template<typename container_message_ptr, typename message_ptr>
size_t send_messages ( const std::string& address, unsigned short port )
{
    size_t mess_count = 0;
	boost::asio::io_service service;
    std::stringstream in_file;
	in_file << TEST_DIR << "/" << address << ".udp";
	std::ifstream in( in_file.str() );
	if(in.is_open())
	{

		boost::asio::ip::udp::endpoint endpoint( boost::asio::ip::address::from_string( address ), port ); 
		boost::asio::ip::udp::socket socket( service, endpoint.protocol() );
		boost::thread sender( boost::bind( work_thread, boost::ref( service ) ) );
		
		container_message_ptr trade_list;

        std::string message;
		while( prepare_message( in, message ) )
		{
            std::string buffer( message );
			socket.send_to( boost::asio::buffer( buffer ), endpoint );
			parse<container_message_ptr, message_ptr>( message, trade_list);
		}

		mess_count = trade_list.size();
		
		service.stop();
		socket.close();
		sender.join();
        return mess_count;
	}
    return 0;
}


void tests_::market_data_processor_tests()
{
    using namespace boost::assign;
    {


        market_data_processor_test_helper test_helper;

        const size_t trade_threads = 2;
        const size_t quote_threads = 2;
            
        vector_address_port trade_ports;
        trade_ports += address_port( "233.200.79.128",62128), address_port("233.200.79.129",62129);
        vector_address_port quote_ports;
        quote_ports += address_port("233.200.79.0",61000),address_port("233.200.79.1",61001),address_port("233.200.79.2",61002);

        size_t trades_sended = 0;
        size_t quotes_sended = 0;

        {
            market_data_receiver test_reciever( trade_threads, quote_threads, trade_ports, quote_ports, test_helper); 
                
            for (vector_address_port::iterator iter = trade_ports.begin(); iter != trade_ports.end(); ++iter)
            {
                trades_sended += send_messages<trades_messages_ptr, trade_message_ptr>(iter->first, iter->second);
            }

            for (vector_address_port::iterator iter = quote_ports.begin(); iter != quote_ports.end(); ++iter)
            {
                quotes_sended += send_messages<quotes_messages_ptr, quote_message_ptr>(iter->first, iter->second);
            }
                

            boost::this_thread::sleep( boost::posix_time::milliseconds( 2000 ) );
        }

        BOOST_CHECK_EQUAL(trades_sended, test_helper.count_trades_);
        BOOST_CHECK_EQUAL(quotes_sended, test_helper.count_quotes_);


    }
}

}