#include <iostream>
#include <csignal>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>


#include "market_data_reciever.h"
#include "dummy_processor.h"

#include "config.h"


boost::mutex exit_mutex;
boost::condition_variable exit_ctrl_c;

using namespace multicast_communication;

void sigint_handler( int )
{
    exit_ctrl_c.notify_one();
}


const std::string& config_name = BINARY_DIR "/config.ini";
const std::string& result_file = BINARY_DIR "/market_data.dat";

int main()
{
    signal(SIGINT, sigint_handler);  
	
    dummy_processor processor(std::cout);
    config conf (config_name);
    market_data_receiver receiver(conf.trade_thread_size(), conf.quote_thread_size(), conf.trade_ports(), conf.quote_ports(), processor);
   
    boost::mutex::scoped_lock lock( exit_mutex ); 
    exit_ctrl_c.wait( lock );

}