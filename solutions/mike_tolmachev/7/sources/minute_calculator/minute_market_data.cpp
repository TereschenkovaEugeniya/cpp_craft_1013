#include "minute_market_data.h"


namespace minute_calculator
{

minute_market_data::minute_market_data(const size_t trade_thread_size,
								const size_t quote_thread_size,
								const ports_vector& trade_ports,
								const ports_vector& quote_ports,
                                const std::string& output_dir) : calculator_(*this),
    receiver_(trade_thread_size, quote_thread_size, trade_ports, quote_ports, *this), output_dir_(output_dir), working_(true), writer_(boost::bind(&minute_market_data::process_minutes, this))
{
}

minute_market_data::minute_market_data(const std::string& config_file, const std::string& output_dir) : calculator_(*this),
    receiver_(config_file, *this), output_dir_(output_dir), working_(true), writer_(boost::bind(&minute_market_data::process_minutes, this))
{
}

minute_market_data::~minute_market_data()
{
    stop();
}

void minute_market_data::stop()
{
    receiver_.stop();

    working_ = false;

    writer_.join();
}

void minute_market_data::new_trade( const trade_message_ptr& trade)
{
	calculator_.new_trade(trade);
}

void minute_market_data::new_quote( const quote_message_ptr& quote)
{
	calculator_.new_quote(quote);
}

void minute_market_data::new_minute(const minute_data& minute)
{
    minutes_.push(minute);
}

void minute_market_data::process_minutes()
{
    while (working_ || !minutes_.empty())
    {
        minute_data minute;
        if (minutes_.pop(minute))
        {
            std::ofstream& output = outputs_[minute.stock_name];

            if(!output.is_open())
            {
                std::string path = output_dir_ + "/";
		        path.append(minute.stock_name);
		        path.append(".dat");
                output.open(path);
            }            

	        output << minute;
        }
    }
}

}