#ifndef _STOCK_DEALER_H_
#define _STOCK_DEALER_H_

#include <fstream>

static const char* input_file_path = SOURCE_DIR "/tests/2_6/2.6_example.in";
static const char* output_file_path = SOURCE_DIR "/tests/2_6/2.6_example.out";

class StockDealer
{
private :
	std::ifstream stock_info;
	std::ofstream info_output;

public :
	struct StockDeal
	{
		char* stock_name;
		unsigned date;
		double price;
		unsigned volume;
		double f2;

		StockDeal();
		~StockDeal();

		bool read_deal( std::ifstream& );
		void write_deal( std::ofstream& );

	private :
		StockDeal(const StockDeal&){}
	};

	StockDealer();
	~StockDealer();

	void process_info();
};

#endif