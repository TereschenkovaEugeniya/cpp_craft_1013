#include "market_data_receiver.h"
#include "market_data_processor_helper.h"
#include <boost/thread.hpp>
#include <csignal>

void signal_handler(boost::condition_variable& cond, int)
{
	cond.notify_all();
}

int main()
{
    using namespace multicast_communication;
    
	std::string config_path(SOURCE_DIR "/tests/data/config.ini");

	market_data_processor_helper processor;
	market_data_receiver receiver(config_path, processor);

	boost::mutex mtx;
	boost::condition_variable cond;

	std::signal(SIGINT, boost::bind(&signal_handler, boost::ref(cond)));
	boost::mutex::scoped_lock lock(mtx);
	cond.wait(lock);
}
