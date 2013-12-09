#include "binReader.h"

binReader::binReader(char* buffer, const size_t bufferSize):
	binBuffer_(buffer),
	buffSize_(bufferSize),
	readPos_(0),
	fileOk(false) {

}

binReader::~binReader() {
	fileOk = false;
}

bool binReader::readMessageHeader(tqHeader_t& quoteHeader) {
	if ( (readPos_ + sizeof quoteHeader) > buffSize_) {
		fileOk = false;
		return false;
	} else {
		char buff;
		readRawValue(buff);
		readRawValue(quoteHeader.messageCategory);
		readRawValue(quoteHeader.messageType);
		readRawValue(quoteHeader.messageNetwork);
		readRawValue(quoteHeader.retransmissionRequest);
		readRawValue(quoteHeader.headerIdentifier);
		readRawValue(quoteHeader.reserved);
		readRawValue(quoteHeader.messageSequenceNumber);
		readRawValue(quoteHeader.participantID);
		readRawValue(quoteHeader.timeStamp);
		return true;
	}
}

bool binReader::readShortQuote(shortQuote_t& shortQuote) {
	if ( (readPos_ + sizeof shortQuote) > buffSize_) {
		fileOk = false;
		return false;
	} else {
		readRawValue(shortQuote.securitySymbol);
		readRawValue(shortQuote.quoteCondition);
		readRawValue(shortQuote.limitUpLimitDownIndicator);
		readRawValue(shortQuote.reserved1);
		readRawValue(shortQuote.bidPriceDenominatorIndicator);
		readRawValue(shortQuote.bidShortPrice);
		readRawValue(shortQuote.bidSizeUnitsOfTrade);
		readRawValue(shortQuote.reserved2);
		readRawValue(shortQuote.offerPriceDenominatorIndicator);
		readRawValue(shortQuote.offerShortPrice);
		readRawValue(shortQuote.offerSizeInUnitsOfTrade);
		readRawValue(shortQuote.reserved3);
		readRawValue(shortQuote.nationalBBOIndicator);
		readRawValue(shortQuote.fINRABBOIndicator);
		return true;
	}
}

bool binReader::readLongQuote(longQuote_t& longQuote) {
	if ( (readPos_ + sizeof longQuote) > buffSize_) {
		fileOk = false;
		return false;
	} else {
		readRawValue(longQuote.securitySymbol);
		readRawValue(longQuote.temporarySuffix);
		readRawValue(longQuote.testMessageIndicator);
		readRawValue(longQuote.primaryListingMarketParticipantIdentifier);
		readRawValue(longQuote.sIPGeneratedMessageIdentifier);
		readRawValue(longQuote.reserved);
		readRawValue(longQuote.financialStatus);
		readRawValue(longQuote.currencyIndicator);
		readRawValue(longQuote.instrumentType);
		readRawValue(longQuote.cancel_CorrectionIndicator);
		readRawValue(longQuote.settlementCondition);
		readRawValue(longQuote.marketCondition);
		readRawValue(longQuote.quoteCondition);
		readRawValue(longQuote.limitUp_LimitDown_LULD_Indicator);
		readRawValue(longQuote.retailInterestIndicator);
		readRawValue(longQuote.bidPrice_LowerLimitPriceBandDenominatorIndicator);
		readRawValue(longQuote.bidPrice_LowerLimitPriceBand);
		readRawValue(longQuote.bidSizeinUnitsofTrade);
		readRawValue(longQuote.offerPrice_UpperLimitPriceBandDenominatorIndicator);
		readRawValue(longQuote.offerPrice_UpperLimitPriceBand);
		readRawValue(longQuote.offerSizeinUnitsofTrade);
		readRawValue(longQuote.fINRAMarketMakerID);
		readRawValue(longQuote.reserved1);
		readRawValue(longQuote.nationalBBOLULDIndicator);
		readRawValue(longQuote.fINRABBOLULDIndicator);
		readRawValue(longQuote.shortSaleRestrictionIndicator);
		readRawValue(longQuote.reserved2);
		readRawValue(longQuote.nationalBBOIndicator);
		readRawValue(longQuote.fINRABBOIndicator);
		 return true;
	}
}

bool binReader::readShortTrade(shortTrade_t& shortTrade) {
	if ( (readPos_ + sizeof shortTrade) > buffSize_) {
		fileOk = false;
		return false;
	} else {
		readRawValue(shortTrade.securitySymbol);
		readRawValue(shortTrade.saleCondition);
		readRawValue(shortTrade.tradeVolume);
		readRawValue(shortTrade.priceDenominatorIndicator);
		readRawValue(shortTrade.tradePrice);
		readRawValue(shortTrade.consolidatedHighLowLastIndicator);
		readRawValue(shortTrade.participantOpenHighLowLastIndicator);
		readRawValue(shortTrade.reserved);
		return true;
	}
}

bool binReader::readLongTrade(longTrade_t& longTrade) {
	if ( (readPos_ + sizeof longTrade) > buffSize_) {
		fileOk = false;
		return false;
	} else {
		readRawValue(longTrade.securitySymbol);
		readRawValue(longTrade.temporarySuffix);
		readRawValue(longTrade.testMessageIndicator);
		readRawValue(longTrade.tradeReportingFacilityIdentifier);
		readRawValue(longTrade.primaryListingMarketParticipantIdentifier);
		readRawValue(longTrade.reserved);
		readRawValue(longTrade.financialStatus);
		readRawValue(longTrade.currencyIndicator);
		readRawValue(longTrade.heldTradeIndicator);
		readRawValue(longTrade.instrumentType);
		readRawValue(longTrade.sellersSaleDays);
		readRawValue(longTrade.saleCondition);
		readRawValue(longTrade.tradeThroughExemptIndicator);
		readRawValue(longTrade.shortSaleRestrictionIndicator);
		readRawValue(longTrade.reserved1);
		readRawValue(longTrade.priceDenominatorIndicator);
		readRawValue(longTrade.tradePrice);
		readRawValue(longTrade.tradeVolume);
		readRawValue(longTrade.consolidatedHighLowLastIndicator);
		readRawValue(longTrade.participantOpenHighLowLastIndicator);
		readRawValue(longTrade.reserved2);
		readRawValue(longTrade.stopStockIndicator);

		 return true;
	}
}
