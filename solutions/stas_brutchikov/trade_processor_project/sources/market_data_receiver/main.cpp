#include <iostream>
#include <csignal>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>


#include "market_data_reciever.h"
#include "dummy_processor.h"



boost::mutex exit_mutex;
boost::condition_variable exit_ctrl_c;

using namespace multicast_communication;

void sigint_handler( int )
{
    exit_ctrl_c.notify_all();
}


const std::string& config_name = BINARY_DIR "/config.ini";
const std::string& result_file = BINARY_DIR "/market_data.dat";

int main()
{
    signal(SIGINT, sigint_handler);  
	
    dummy_processor processor(std::cout);
    market_data_receiver receiver(processor);

    receiver.run();

    exit_ctrl_c.wait( boost::mutex::scoped_lock( exit_mutex ) );

    receiver.stop();
}