#ifndef DATASTRUCTS_INCLUDE
#define DATASTRUCTS_INCLUDE

#include <string>
#include <stdint.h>
#include <stdlib.h>

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

	void render();
};

struct shortQuote_t{
	std::string securitySymbol;
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

	void render();
};

struct longQuote_t{
	std::string securitySymbol;
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
	std::string securitySymbol;
	char saleCondition;
	char tradeVolume[4];
	char priceDenominatorIndicator;
	char tradePrice[8];
	char consolidatedHighLowLastIndicator;
	char participantOpenHighLowLastIndicator;
	char reserved;
};

struct longTrade_t{
	std::string securitySymbol;
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

#endif
