#include "test_registrator.h"
#include "multicast_communication.h"
#include "marketdatareceiver.h"
#include "binReader.h"

#include "cstdlib"

#include <vector>


void task6::tests_::multicast_communication_tests()
{
	market_data_receiver mdr;
	std::vector<std::pair<std::string, unsigned short> > v1;
	std::vector<std::pair<std::string, unsigned short> > v2;

	{
		BOOST_CHECK_NO_THROW( multicast_communication  mc(1, 1, v1, v2, mdr ) );
    }
}

void task6::tests_::binReader_tests(){
	{
		BOOST_CHECK_NO_THROW(binReader br());
	}

	{
		const char* buff = "asid";
		binReader br(buff, 4);
		BOOST_CHECK_EQUAL(br.fileOk, true);

		tqHeader_t header;
		bool rez = br.readMessageHeader(header);
		BOOST_CHECK_EQUAL(br.fileOk, false);
		BOOST_CHECK_EQUAL(rez, false);
	}

	{
		const char* buff = "EDEO A  003759032T:J_073ADMR  B00004147006 B00004148004 12";
		binReader br(buff, 60);
		BOOST_CHECK_EQUAL(br.fileOk, true);

		tqHeader_t header;
		bool rez = br.readMessageHeader(header);
		BOOST_CHECK_EQUAL(br.fileOk, true);
		BOOST_CHECK_EQUAL(rez, true);
		BOOST_CHECK_EQUAL(header.messageCategory, 'E');
		BOOST_CHECK_EQUAL(header.messageType, 'D');
		BOOST_CHECK_EQUAL(header.isLongQuote(), false);
		BOOST_CHECK_EQUAL(header.isShortQuote(), true);
		shortQuote_t sq;
		br.readShortQuote(sq);
		BOOST_CHECK_EQUAL(br.fileOk, true);
		BOOST_CHECK_EQUAL(renderArrStr(sq.securitySymbol, 3), "ADM");
	}
}

