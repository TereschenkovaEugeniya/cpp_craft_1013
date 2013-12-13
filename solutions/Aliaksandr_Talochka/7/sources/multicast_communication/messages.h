#pragma once

#include <string>

namespace messages
{
	struct trade_message
	{
	
	};

	struct quote_message
	{
		std::string security_symbol;
		double bid_price;
		double bid_volume;
		double offer_price;
		double offer_volume;

		quote_message()
			: security_symbol(""), bid_price(0.0), bid_volume(0.0),
			offer_price(0.0), offer_volume(0.0)
		{
		}
	};

	#pragma pack(push,1)


	struct ShortQuote
	{
		char Header[24];
		char SecuritySymbol[3];
		char QuoteCondition;
		char LULD_Indicator;
		char Reserved1;
		char BPDI;		//Bid Price Denominator Indicator
		char BidShortPrice[8];
		char BidSizeUnitsOfTrade[3];
		char Reserved2;
		char OPDI;		//Offer Price Denominator Indicator
		char OfferShortPrice[8];
		char OfferSizeUnitsOfTrade[3];
		char Reserved3;
		char National_BBO_Indicator;
		char FINRA_BBO_Indicator;
	};

	struct LongQuote
	{
		char Header[24];
		char SecuritySymbol[11];
		char TemporarySuffix;
		char TestMessageIndicator;
		char PLMPI;
		char SIPGMI;
		char Reserved1;
		char FinancialStatus;
		char CurrencyIndicator[3];
		char InstrumentType;
		char CancelIndicator;
		char SettlementCondition;
		char MarketCondition;
		char QuoteCondition;
		char LULD_Indicator;
		char RIIndicator;
		char BidPrice_LLPBDI;
		char BidPrice_LLPB[12];
		char BidSize[7];
		char OfferPrice_LLPBDI;
		char OfferPrice_LLPB[12];
		char OfferSize[7];
		char FINRA_ID[4];
		char Reserved2;
		char National_BBO_LULD_Ind;
		char FINRA_BBO_LULD_Ind;
		char ShortSaleInd;
		char Reserved3;
		char National_BBO_Ind;
		char FINRA_BBO_Ind;
	};

	#pragma pack(pop)
}