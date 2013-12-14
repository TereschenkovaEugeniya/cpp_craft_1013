#pragma once

#include <string>

namespace messages
{
	struct trade_message
	{
		std::string security_symbol;
		double price;
		double volume;
		unsigned int time;

		trade_message()
			: security_symbol(""), 
			  price(0.0),
			  volume(0.0),
			  time(0)
		{
		}
	};

	struct quote_message
	{
		std::string security_symbol;
		double bid_price;
		double bid_volume;
		double offer_price;
		double offer_volume;
		unsigned int time;


		quote_message()
			: security_symbol(""), bid_price(0.0), bid_volume(0.0),
			offer_price(0.0), offer_volume(0.0), time(0)
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


	struct ShortTrade
	{
		char Msg_Header [24];
		char SEC_Symbol [3];
		char Sale_Cond;
		char Trade_Volume [4];
		char Price_Denom_IND;
		char Trade_Price [8];
		char Consolidated_IND;
		char Participant_IND;
		char Reserved;
	};

	struct LongTrade
	{
		char Msg_Header [24];
		char SEC_Symbol [11];
		char Temp_Suf;
		char Test_Msg_IND;
		char Trade_Rep_Facility_ID;
		char Prim_List_Market_Part_ID;
		char Reserved1;
		char Financial_Status;
		char Currency_Ind [3];
		char Held_Trade_IND;
		char Instr_Type;
		char Seller_Sale_Days [3];
		char Sale_Cond [4];
		char Trade_Through_Exempt_IND;
		char Short_Sale_Restrict_IND;
		char Reserved2;
		char Price_Denom_IND;
		char Trade_Price [12];
		char Trade_Vol [9]; 
		char Consolidated_Ind; 
		char Participant_Ind;
		char Reserved3;
		char Stop_Stock_Ind;
	};

	#pragma pack(pop)
}