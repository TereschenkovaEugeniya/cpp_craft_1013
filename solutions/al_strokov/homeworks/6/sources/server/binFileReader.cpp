#include "binFileReader.h"

binFileReader::binFileReader(const std::string& filename) :
		fName(filename), fileOk(false) {
	ifs.open(filename.c_str(), std::ios::binary | std::ios::in);
	fileOk = ifs.is_open();
}

bool binFileReader::readRawValue(std::string& value, const size_t length) {
	if (ifs.eof()) {
		fileOk = false;
		return false;
	} else {
		char* buff = new char;
		ifs.read(buff, length);
		value = buff;
		value = value.substr(0, length);
		delete buff;
		return true;
	}
}

binFileReader::~binFileReader() {
	ifs.close();
	fileOk = false;
}

bool binFileReader::readQuoteHeader(tqHeader_t& quoteHeader) {
	if (ifs.eof()) {
		fileOk = false;
		return false;
	} else {
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

bool binFileReader::readShortQuote(shortQuote_t& shortQuote) {
	if (ifs.eof()) {
		fileOk = false;
		return false;
	} else {
		readRawValue(shortQuote.securitySymbol, 3);
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

bool binFileReader::readLongQuote(longQuote_t& longQuote) {
	if (ifs.eof()) {
		fileOk = false;
		return false;
	} else {
		readRawValue(longQuote.securitySymbol, 11);
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

bool binFileReader::readShortTrade(shortTrade_t& shortTrade) {
	if (ifs.eof()) {
		fileOk = false;
		return false;
	} else {
		readRawValue(shortTrade.securitySymbol, 3);
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

bool binFileReader::readLongTrade(longTrade_t& longTrade) {
	if (ifs.eof()) {
		fileOk = false;
		return false;
	} else {
		readRawValue(longTrade.securitySymbol, 11);
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
