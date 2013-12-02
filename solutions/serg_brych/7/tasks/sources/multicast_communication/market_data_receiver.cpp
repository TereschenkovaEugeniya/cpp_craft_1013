#include "market_data_receiver.h"
#include "udp_listener.h"

multicast_communication::market_data_receiver::market_data_receiver(market_data_processor& processor):
	config_(new config_reader(config_file)), processor_(processor)
{
	if(config_->get_quote_adr_and_ports().size() > config_->get_quote_thread_size())
	{
		throw std::logic_error("Need more threads to process quote messages");
	}

	if(config_->get_trade_adr_and_ports().size() > config_->get_trade_thread_size())
	{
		throw std::logic_error("Need more threads to process trade messages");
	}
}

multicast_communication::market_data_receiver::market_data_receiver( market_data_processor& processor, multicast_communication::config_reader* config )
	: processor_(processor)
{
	config_.reset(config);
	
	if(config_->get_quote_adr_and_ports().size() > config_->get_quote_thread_size())
	{
		throw std::logic_error("Need more threads to process quote messages");
	}

	if(config_->get_trade_adr_and_ports().size() > config_->get_trade_thread_size())
	{
		throw std::logic_error("Need more threads to process trade messages");
	}
}

multicast_communication::market_data_receiver::~market_data_receiver()
{
	stop();
}

void multicast_communication::market_data_receiver::run()
{
	services_quotes_.resize(config_->get_quote_adr_and_ports().size());
	for(auto i = services_quotes_.begin(); i != services_quotes_.end(); i++)
	{
		i->reset(new boost::asio::io_service());
	}

	size_t count_of_multicast_addresses = config_->get_quote_adr_and_ports().size();
	
	udp_listners_quotes.resize(config_->get_quote_adr_and_ports().size());
	for(size_t i = 0; i < count_of_multicast_addresses; i++)
	{
		udp_listners_quotes[i].reset(new async_udp::udp_listener(
			(*(services_quotes_[i])), 
			config_->get_quote_adr_and_ports()[i % count_of_multicast_addresses].first,
			config_->get_quote_adr_and_ports()[i % count_of_multicast_addresses].second,
			[&](const std::string& data)
			{
				quote_message_list_ptr processed_messages;
				if(quote_message::parse_block(data,  processed_messages))
				{
					std::for_each(processed_messages.begin(), processed_messages.end(),[&](quote_message_ptr &msg)
						{
							processor_.new_quote(msg);
						}
					);
				}
			}
		));
	}


	
	if(count_of_multicast_addresses)
	{
		for(size_t i = 0; i < config_->get_quote_thread_size(); i++)
		{
			const size_t id = i % count_of_multicast_addresses;
			process_threads_.create_thread(
				boost::bind( &multicast_communication::market_data_receiver::process_thread, this, services_quotes_[id]));
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	services_trades_.resize(config_->get_trade_adr_and_ports().size());
	for(auto i = services_trades_.begin(); i != services_trades_.end(); i++)
	{
		i->reset(new boost::asio::io_service());
	}

	count_of_multicast_addresses = config_->get_trade_adr_and_ports().size();
	
	udp_listners_trades.resize(config_->get_trade_adr_and_ports().size());
	for(size_t i = 0; i < count_of_multicast_addresses; i++)
	{
		udp_listners_trades[i].reset(new async_udp::udp_listener(
			(*(services_trades_[i])), 
			config_->get_trade_adr_and_ports()[i % count_of_multicast_addresses].first,
			config_->get_trade_adr_and_ports()[i % count_of_multicast_addresses].second,
			[&](const std::string& data)
			{
				trade_message_list_ptr processed_messages;
				if(trade_message::parse_block(data,  processed_messages))
				{
					std::for_each(processed_messages.begin(), processed_messages.end(),[&](trade_message_ptr &msg)
						{
							processor_.new_trade(msg);
						}
					);
				}
			}
		));
	}


	
	if(count_of_multicast_addresses)
	{
		for(size_t i = 0; i < config_->get_trade_thread_size(); i++)
		{
			const size_t id = i % count_of_multicast_addresses;
			process_threads_.create_thread(
				boost::bind( &multicast_communication::market_data_receiver::process_thread, this, services_trades_[id]));
		}
	}
}

void multicast_communication::market_data_receiver::stop()
{
	for(auto service: services_quotes_)
	{
		service->stop();	
	}

	for(auto service: services_trades_)
	{
		service->stop();	
	}
	
	process_threads_.join_all();
}

void multicast_communication::market_data_receiver::process_thread(service_ptr service)
{
	service->run();
}
