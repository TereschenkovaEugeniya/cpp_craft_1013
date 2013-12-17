#include "test_registrator.h"
#include "multicast_communication.h"
#include "market_data_receiver.h"
#include "binReader.h"

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
		const char* buff = "\x01""EDEO A  003759032T:J_073ADMR  B00004147006 B00004148004 12""\x03";
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

	{
		const char* buff = "\x01""EBEO A  003759496N:J_699ADM             0    AAAR AD0000004147000000001D0000004149000000008     A   02""\x03";
		binReader br(buff, 104);
		tqHeader_t header;
		br.readMessageHeader(header);
		BOOST_CHECK_EQUAL(header.isLongQuote(), true);
		BOOST_CHECK_EQUAL(header.isShortQuote(), false);
		longQuote_t lq;
		br.readLongQuote(lq);
		BOOST_CHECK_EQUAL(renderArrStr(lq.securitySymbol, 11), "ADM        ");
	}

	{
		const char* buff = "\x01" "EBAO A  000146234N:3]004ACN             0     000 F  1  D000000779000000000100DD 0" "\x03";
		binReader br(buff, 84);
		tqHeader_t header;
		br.readMessageHeader(header);
		BOOST_CHECK_EQUAL(header.isLongTrade(), true);
		BOOST_CHECK_EQUAL(header.isShortTrade(), false);
		longTrade_t lt;
		br.readLongTrade(lt);
		BOOST_CHECK_EQUAL(renderArrStr(lt.securitySymbol, 11), "ACN        ");
	}

	{
		const char* buff = "\x01" "EIAO A  000146237T:3]010ACN@0100B00007790DD " "\x03";
		binReader br(buff, 46);
		tqHeader_t header;
		br.readMessageHeader(header);
		BOOST_CHECK_EQUAL(header.isLongTrade(), false);
		BOOST_CHECK_EQUAL(header.isShortTrade(), true);
		shortTrade_t st;
		br.readShortTrade(st);
		BOOST_CHECK_EQUAL(renderArrStr(st.securitySymbol, 3), "ACN        ");
	}

}

