#ifndef DATASTRUCTS_INCLUDE
#define DATASTRUCTS_INCLUDE

#include <stdint.h>
#include <stdlib.h>
#include <string>

struct tqHeader_t{
	char messageCategory;
	char messageType;
	char messageNetwork;
	char retransmissionRequest[2];
	char headerIdentifier;
	char reserved[2];
	char messageSequenceNumber[9];
	char participantID;
	char timeStamp[6];

	bool isShortQuote();
	bool isLongQuote();
	bool isShortTrade();
	bool isLongTrade();
};

struct shortQuote_t{
	char securitySymbol[3];
	char quoteCondition;
	char limitUpLimitDownIndicator;
	char reserved1;
	char bidPriceDenominatorIndicator;
	char bidShortPrice[8];
	char bidSizeUnitsOfTrade[3];
	char reserved2;
	char offerPriceDenominatorIndicator;
	char offerShortPrice[8];
	char offerSizeInUnitsOfTrade[3];
	char reserved3;
	char nationalBBOIndicator;
	char fINRABBOIndicator;

};

struct longQuote_t{
	char securitySymbol[11];
	char temporarySuffix;
	char testMessageIndicator;
	char primaryListingMarketParticipantIdentifier;
	char sIPGeneratedMessageIdentifier;
	char reserved;
	char financialStatus;
	char currencyIndicator[3];
	char instrumentType;
	char cancel_CorrectionIndicator;
	char settlementCondition;
	char marketCondition;
	char quoteCondition;
	char limitUp_LimitDown_LULD_Indicator;
	char retailInterestIndicator;
	char bidPrice_LowerLimitPriceBandDenominatorIndicator;
	char bidPrice_LowerLimitPriceBand[12];
	char bidSizeinUnitsofTrade[7];
	char offerPrice_UpperLimitPriceBandDenominatorIndicator;
	char offerPrice_UpperLimitPriceBand[12];
	char offerSizeinUnitsofTrade[7];
	char fINRAMarketMakerID[4];
	char reserved1;
	char nationalBBOLULDIndicator;
	char fINRABBOLULDIndicator;
	char shortSaleRestrictionIndicator;
	char reserved2;
	char nationalBBOIndicator;
	char fINRABBOIndicator;
};

struct shortTrade_t{
	char securitySymbol[3];
	char saleCondition;
	char tradeVolume[4];
	char priceDenominatorIndicator;
	char tradePrice[8];
	char consolidatedHighLowLastIndicator;
	char participantOpenHighLowLastIndicator;
	char reserved;
};

struct longTrade_t{
	char securitySymbol[11];
	char temporarySuffix;
	char testMessageIndicator;
	char tradeReportingFacilityIdentifier;
	char primaryListingMarketParticipantIdentifier;
	char reserved;
	char financialStatus;
	char currencyIndicator[3];
	char heldTradeIndicator;
	char instrumentType;
	char sellersSaleDays[3];
	char saleCondition[4];
	char tradeThroughExemptIndicator;
	char shortSaleRestrictionIndicator;
	char reserved1;
	char priceDenominatorIndicator;
	char tradePrice[12];
	char tradeVolume[9];
	char consolidatedHighLowLastIndicator;
	char participantOpenHighLowLastIndicator;
	char reserved2;
	char stopStockIndicator;

};

std::string renderArrHex(const char * arr, const size_t size);
std::string renderArrStr(const char * arr, const size_t size);

#endif
