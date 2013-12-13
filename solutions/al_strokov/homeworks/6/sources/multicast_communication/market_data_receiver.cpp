#include "market_data_receiver.h"

market_data_receiver::market_data_receiver() {
}

market_data_receiver::~market_data_receiver() {
}

void market_data_receiver::addTrade(const trade_message& trade) {
	trades_.push_back(trade);
}

void market_data_receiver::addQuote(const quote_message& quote) {
	quotes_.push_back(quote);
}

void market_data_receiver::addTrade(const trade_message_ptr& shared_ptr) {
	trades_.push_back(*shared_ptr);
}

void market_data_receiver::addQuote(const quote_message_ptr& shared_ptr) {
	quotes_.push_back(*shared_ptr);
}

void market_data_receiver::addTrade(const shortTrade_t& shortTrade) {
	trade_message tradeMessage;
	std::string strSecuritySymbol = chars2string(shortTrade.securitySymbol, 3);
	tradeMessage.security_symbol = strSecuritySymbol;
	tradeMessage.price = atof(chars2string(shortTrade.tradePrice, 8).c_str())
			/ denominator_.getDenominatorByChar(
					shortTrade.priceDenominatorIndicator);
	tradeMessage.volume = atof(chars2string(shortTrade.tradeVolume, 4).c_str());
	trades_.push_back(tradeMessage);
}

void market_data_receiver::addTrade(const longTrade_t& longTrade) {
	trade_message tradeMessage;
	std::string strSecuritySymbol = chars2string(longTrade.securitySymbol, 11);
	tradeMessage.security_symbol = strSecuritySymbol;
	tradeMessage.price = atof(chars2string(longTrade.tradePrice, 12).c_str())
			/ denominator_.getDenominatorByChar(
					longTrade.priceDenominatorIndicator);
	tradeMessage.volume = atof(chars2string(longTrade.tradeVolume, 9).c_str());
	trades_.push_back(tradeMessage);
}

void market_data_receiver::addQuote(const shortQuote_t& shortQuote) {
	quote_message quoteMessage;
	std::string strSecuritySymbol = chars2string(shortQuote.securitySymbol, 3);
	quoteMessage.security_symbol = strSecuritySymbol;
	quoteMessage.bid_price = atof(
			chars2string(shortQuote.bidShortPrice, 8).c_str())
			/ denominator_.getDenominatorByChar(
					shortQuote.bidPriceDenominatorIndicator);
	quoteMessage.bid_volume = atof(
			chars2string(shortQuote.bidSizeUnitsOfTrade, 3).c_str());
	quoteMessage.offer_price = atof(
			chars2string(shortQuote.offerShortPrice, 8).c_str())
			/ denominator_.getDenominatorByChar(
					shortQuote.offerPriceDenominatorIndicator);
	quoteMessage.offer_volume = atof(
			chars2string(shortQuote.offerSizeInUnitsOfTrade, 3).c_str());
	quotes_.push_back(quoteMessage);
}

void market_data_receiver::addQuote(const longQuote_t& longQuote) {
	quote_message quoteMessage;
	std::string strSecuritySymbol = chars2string(longQuote.securitySymbol, 11);
	quoteMessage.security_symbol = strSecuritySymbol;
	quoteMessage.bid_price = atof(
			chars2string(longQuote.bidPrice_LowerLimitPriceBand, 12).c_str())
			/ denominator_.getDenominatorByChar(
					longQuote.bidPrice_LowerLimitPriceBandDenominatorIndicator);
	quoteMessage.bid_volume = atof(
			chars2string(longQuote.bidSizeinUnitsofTrade, 7).c_str());
	quoteMessage.offer_price =
			atof(
					chars2string(longQuote.offerPrice_UpperLimitPriceBand, 12).c_str())
					/ denominator_.getDenominatorByChar(
							longQuote.offerPrice_UpperLimitPriceBandDenominatorIndicator);
	quoteMessage.offer_volume = atof(
			chars2string(longQuote.offerSizeinUnitsofTrade, 7).c_str());
	quotes_.push_back(quoteMessage);
}

std::string market_data_receiver::chars2string(const char* buffer,
		const size_t bufferSize) const {
	std::stringstream ss;
	for (size_t i = 0; i < bufferSize; i++) {
		ss << buffer[i];
	}
	return ss.str();
}
