#include "market_data_receiver.h"
#include <iostream>

namespace multicast_communication
{
	market_data_receiver::market_data_receiver(const size_t& trade_thread_size,
												const size_t& quote_thread_size,
												const std::vector<std::pair<std::string, unsigned short> >& trade_ports,
												const std::vector<std::pair<std::string, unsigned short> >& quote_ports,
												market_data_processor& processor) : processor_(processor), working_(false)
	{
		initialize(trade_thread_size, quote_thread_size, trade_ports, quote_ports);		
	}

	market_data_receiver::market_data_receiver(const std::string& file_name,
												market_data_processor& processor) : processor_(processor), working_(false)
	{
		std::ifstream input(file_name);

		if (input.good())
		{
			size_t trade_thread_size;
			size_t quote_thread_size;
			size_t trade_ports_amount;
			size_t quote_ports_amount;
			std::vector<std::pair<std::string, unsigned short> > trade_ports;
			std::vector<std::pair<std::string, unsigned short> > quote_ports;

			input >> trade_thread_size;
			input >> quote_thread_size;
			input >> trade_ports_amount;

			while (trade_ports_amount)
			{
				std::string ip;
				unsigned short port;

				input >> ip >> port;

				trade_ports.push_back(std::make_pair(ip, port));

				--trade_ports_amount;
			}

			input >> quote_ports_amount;

			while (quote_ports_amount)
			{
				std::string ip;
				unsigned short port;

				input >> ip >> port;

				quote_ports.push_back(std::make_pair(ip, port));

				--quote_ports_amount;
			}

			initialize(trade_thread_size, quote_thread_size, trade_ports, quote_ports);
		}
	}

	market_data_receiver::~market_data_receiver()
	{
		stop();
	}

	void market_data_receiver::stop()
	{
		if (!working_)
		{
			return;
		}

		working_ = false;

		io_service_trade_.stop();
		io_service_quote_.stop();

		work_threads_.join_all();
	}

	void market_data_receiver::initialize(const size_t& trade_thread_size,
											const size_t& quote_thread_size,
											const std::vector<std::pair<std::string, unsigned short> >& trade_ports,
											const std::vector<std::pair<std::string, unsigned short> >& quote_ports)
	{
		working_ = true;

		for (auto it = trade_ports.begin(); it != trade_ports.end(); ++it)
		{
			boost::shared_ptr<market_data_connector> connector(new market_data_connector(io_service_trade_, it->first, it->second, trades_));
			connectors_.push_back(connector);
		}

		for (auto it = quote_ports.begin(); it != quote_ports.end(); ++it)
		{
			boost::shared_ptr<market_data_connector> connector(new market_data_connector(io_service_quote_, it->first, it->second, quotes_));
			connectors_.push_back(connector);
		}

		for (int i = 0; i < trade_thread_size; ++i)
		{
			work_threads_.create_thread(boost::bind(&market_data_receiver::service_thread, this, boost::ref(io_service_trade_)));
			//boost::thread trade_thread(boost::bind(&market_data_receiver::service_thread, this, boost::ref(io_service_trade_)));
		}

		for (int i = 0; i < quote_thread_size; ++i)
		{
			work_threads_.create_thread(boost::bind(&market_data_receiver::service_thread, this, boost::ref(io_service_quote_)));
			//boost::thread trade_thread(boost::bind(&market_data_receiver::service_thread, this, boost::ref(io_service_quote_)));
		}
	
		work_threads_.create_thread(boost::bind(&market_data_receiver::trades_thread, this));
		work_threads_.create_thread(boost::bind(&market_data_receiver::quotes_thread, this));
	}
	
	void market_data_receiver::service_thread(boost::asio::io_service& service)
	{
		service.run();
	}

	void market_data_receiver::trades_thread()
	{
		while (working_)
		{
			std::string block;
			if (trades_.pop(block))
			{
				char us = 0x1F;
				char soh = 0x01;
				char etx = 0x03;
				std::string msg;
				for (auto it = block.begin(); it != block.end(); ++it)
				{
					if (*it == us || *it == etx)
					{
						trade_message_ptr trade(new trade_message);
						if (trade->initialize(msg))
						{
							processor_.new_trade(trade);
						}
						msg.clear();
					}
					else if (*it != soh)
					{
						msg.push_back(*it);
					}

					if (*it == etx)
					{
						break;
					}

				}
			}			
		}
	}

	void market_data_receiver::quotes_thread()
	{
		while (working_)
		{
			std::string block;
			if (quotes_.pop(block))
			{
				char us = 0x1F;
				char soh = 0x01;
				char etx = 0x03;
				std::string msg;
				for (auto it = block.begin(); it != block.end(); ++it)
				{
					if (*it == us || *it == etx)
					{
						quote_message_ptr quote(new quote_message);
						if (quote->initialize(msg))
						{
							processor_.new_quote(quote);
						}
						msg.clear();
					}
					else if (*it != soh)
					{
						msg.push_back(*it);
					}

					if (*it == etx)
					{
						break;
					}

				}
			}			
		}
	}
}
