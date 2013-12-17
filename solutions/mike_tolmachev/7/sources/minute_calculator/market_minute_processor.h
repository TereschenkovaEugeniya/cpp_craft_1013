#pragma once


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
		
		bool init_by_trade;
	};

	class market_minute_processor
	{
		friend class minute_calculator; // please insert here your processing class 
	public:
		explicit market_minute_processor(){}
		virtual ~market_minute_processor(){}
	private:
		virtual void new_minute(const minute_data& ) = 0;
	};
}

