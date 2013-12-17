
#include <string>
#include <signal.h>
#include <vector>

#include <boost/thread.hpp>

#include <fstream>
#include <sstream>

#include "common_data.h"
#include "messages.h"

#include "minute_calculator.h"
#include "market_data_receiver.h"

typedef std::vector < common_data::minute_datafeed > vector_minute_datafeed;
boost::thread_group thread_gr;

std::map<std::string, std::ofstream *> output_files;
minute_calculator::minute_calculator_process calculator;
multicast_communication::market_data_receiver communication(std::string(SOURCE_DIR"/sources/etc/configs.ini"));

void save_datafeed(common_data::minute_datafeed& datafeed )
{
	if (output_files.find(datafeed.stock_name) == output_files.end())
	{
		std::string fileName =  BINARY_DIR + std::string( "/" ) + std::string( datafeed.stock_name ) + ".dat";
		std::ofstream *output_file = new std::ofstream(fileName.c_str(), std::ios_base::binary|std::ios_base::out|std::ios_base::trunc  );
		output_files.insert(std::pair<std::string, std::ofstream *>(std::string(datafeed.stock_name), output_file));
	}
	
	if (output_files[std::string(datafeed.stock_name)]->is_open())
		output_files[std::string(datafeed.stock_name)]->write(reinterpret_cast<char*>(&datafeed), sizeof(datafeed));
}

void process()
{
	while(1)
	{
		std::vector<messages::quote_message> quotes = communication.get_quote_messages();
		while (!quotes.empty())
		{
			messages::quote_message quote_m = quotes.back();
			quotes.pop_back();

			common_data::quote_data quote_d;

			quote_d.bid_price = quote_m.bid_price;
			quote_d.offer_price = quote_m.offer_price;
			strcpy(quote_d.security_symbol, quote_m.security_symbol.c_str());
			quote_d.time = quote_m.time;

			try
			{
				calculator.add_quote_data(quote_d);
			} 
			catch ( std::logic_error& )
			{
			}
		}

		std::vector<messages::trade_message> trades = communication.get_trade_messages();
		while (!trades.empty())
		{
			messages::trade_message trade_m = trades.back();
			trades.pop_back();

			common_data::trade_data trade_d;

			trade_d.price = trade_m.price;
			trade_d.time = trade_m.time;
			strcpy(trade_d.security_symbol, trade_m.security_symbol.c_str());

			try
			{
				calculator.add_trade_data(trade_d);
			} 
			catch ( std::logic_error& )
			{
			}
		}
		common_data::minute_datafeed data = calculator.get_minute_datafeed_one();
		if ( data.minute != 0 )
			save_datafeed(data);

		boost::this_thread::sleep( boost::posix_time::milliseconds( 10 ) );
	}
}		

static void button_handler( int )
{
	vector_minute_datafeed minute_datafeeds = calculator.get_minute_datafeed();
	calculator.stop();
	communication.stop();
	thread_gr.interrupt_all();

	while (!minute_datafeeds.empty())
	{
		save_datafeed(minute_datafeeds.back());
		minute_datafeeds.pop_back();
	}
	
	for(std::map < std::string, std::ofstream *>::iterator it = output_files.begin();
			it != output_files.end(); )
	{
		it->second->close();
		delete it->second;
		it = output_files.erase(it);
	}

	exit( 0 );
}

int main()
{
	signal( SIGINT, button_handler );

	try
	{
		communication.start();
	}
	catch (const boost::system::system_error& e )
	{
		std::cout << e.what() << std::endl;
		exit(0);
	}
	calculator.start();

	thread_gr.create_thread( boost::bind( &process) );
	thread_gr.join_all();
}
