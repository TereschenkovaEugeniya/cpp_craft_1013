#ifndef _STOCK_DEALER_H_
#define _STOCK_DEALER_H_

#include <fstream>
#include <map>
#include <string>
#include <vector>

static const char* input_file_path = SOURCE_DIR "/tests/3_6/3.6_example.in";

class StockDealer
{
public:
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

		StockDeal(const StockDeal& deal)
		{
			this->date = deal.date;
			this->price = deal.price;
			this->volume = deal.volume;
			this->f2 = deal.f2;

			this->stock_name = new char[ strlen(deal.stock_name) + 1 ];
			strcpy( this->stock_name, deal.stock_name ); 
		}
	};
	
	StockDealer();
	~StockDealer();

	void process_info();
	void sort_deal( const StockDeal& );
	void output_catalogue();
	void write_stock( std::string );

private :
	std::ifstream stock_info;
	std::map< std::string, std::vector< StockDealer::StockDeal > > deals_catalogue;

};

#endif