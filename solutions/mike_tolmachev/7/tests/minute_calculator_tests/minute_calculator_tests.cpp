#include "test_registrator.h"

#include <minute_calculator.h>

namespace minute_calculator
{
namespace tests_
{

class market_minute_processor_helper : public market_minute_processor
{
public:
	explicit market_minute_processor_helper(){}
	virtual ~market_minute_processor_helper(){}

	minute_data data_;
private:
	virtual void new_minute(const minute_data& data)
	{
		data_ = data;
	}

public:
	bool compare()
	{
		std::string security_symbol = "ACN";
		std::string stock(data_.stock_name);

		return (
			data_.close_prise == 88.9 &&
			data_.open_prise == 77.9 &&
			data_.high_prise == 88.9 &&
			data_.low_price == 77.9 &&
			security_symbol.compare(stock) == 0 &&
			data_.minute == 36225u &&
			data_.bid == 12 &&
			data_.ask == 8.0
			);
	}
};

void minute_calculator_tests()
{	
	market_minute_processor_helper mmph;

	minute_calculator calc(mmph);

	multicast_communication::trade_message_ptr tm1(new multicast_communication::trade_message);
	tm1->initialize("EIAO A  000146235T:3]008ACN@0100B00007790DD ");
	multicast_communication::trade_message_ptr tm2(new multicast_communication::trade_message);
	tm2->initialize("EIAO A  000146235T:3b008ACN@0100B00008890DD ");
	multicast_communication::trade_message_ptr tm3(new multicast_communication::trade_message);
	tm3->initialize("EIAO A  000146235T:5]008ACN@0100B00008890DD ");

	multicast_communication::quote_message_ptr qm1(new multicast_communication::quote_message);
	qm1->initialize("EDEO A  003759032T:3]073ACNR  B00004147006 B00004148004 12");
	multicast_communication::quote_message_ptr qm2(new multicast_communication::quote_message);
	qm2->initialize("EDEO A  003759032T:3]073ACNR  B00004147006 B00004148004 12");
	
	calc.new_trade( tm1 );
    BOOST_CHECK_EQUAL( mmph.compare(), false );
	calc.new_quote( qm1 );
    BOOST_CHECK_EQUAL( mmph.compare(), false );
	calc.new_quote( qm2 );
    BOOST_CHECK_EQUAL( mmph.compare(), false );
	calc.new_trade( tm2 );
    BOOST_CHECK_EQUAL( mmph.compare(), false );
	calc.new_trade( tm3 );
	BOOST_CHECK_EQUAL( mmph.compare(), true );
}

}
}