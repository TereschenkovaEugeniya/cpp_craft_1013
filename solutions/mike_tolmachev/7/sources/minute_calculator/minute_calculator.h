#pragma once

#include <market_data_processor.h>

namespace minute_calculator
{
	struct minute_data
	{
	    uint32_t minute; // time from 1900.01.01 00:00:00
	    char stock_name[16]; // null-terminated
	    double open_prise;
	    double high_prise;
	    double low_price;
	    double close_prise;
	    double volume;
	    double bid;
	    double ask;
	};
	
	std::ostream& operator<<(std::ostream& output, const minute_data& md);

	class minute_calculator
	{
	};
}