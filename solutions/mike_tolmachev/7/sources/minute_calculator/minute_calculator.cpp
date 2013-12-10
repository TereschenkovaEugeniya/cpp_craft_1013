#include "minute_calculator.h"

namespace minute_calculator
{
	std::ostream& operator<<(std::ostream& output, const minute_data& md)
	{		
		output.write(reinterpret_cast< const char* >(&md.minute), sizeof( md.minute));
		output.write(md.stock_name, 16);
		output.write(reinterpret_cast< const char* >(&md.open_prise), sizeof( md.open_prise));
		output.write(reinterpret_cast< const char* >(&md.high_prise), sizeof( md.high_prise));
		output.write(reinterpret_cast< const char* >(&md.low_price), sizeof( md.low_price));
		output.write(reinterpret_cast< const char* >(&md.close_prise), sizeof( md.close_prise));
		output.write(reinterpret_cast< const char* >(&md.volume), sizeof( md.volume));
		output.write(reinterpret_cast< const char* >(&md.bid), sizeof( md.bid));
		output.write(reinterpret_cast< const char* >(&md.ask), sizeof( md.ask));

		return output;
	}
}