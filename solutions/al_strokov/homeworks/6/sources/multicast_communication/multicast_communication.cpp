#include "multicast_communication.h"
#include <sstream>

const size_t multicast_communication::buffMaxSize_ = 1024;

const std::map<const char, double> multicast_communication::denominators = { {
		'A', 10.0 }, { 'B', 100.0 }, { 'C', 1000.0 }, { 'D', 10000.0 }, { 'E',
		100000.0 }, { 'F', 1000000.0 }, { 'G', 10000000.0 },
		{ 'H', 100000000.0 }, { '3', 8.0 }, { '4', 16.0 }, { '5', 32.0 }, { '6',
				64.0 }, { '7', 128.0 }, { '8', 256.0 }, { 'I', 1.0 } };

multicast_communication::multicast_communication(size_t tradeThreadsCount,
		size_t quoteThreadsCount, udpEndpoints_t tradesEndpoints,
		udpEndpoints_t quotesEndpoints, market_data_receiver& dataReceiver) :
		tradeThreadsCount_(tradeThreadsCount), quoteThreadsCount_(
				quoteThreadsCount), tradesEndpoints_(tradesEndpoints), quotesEndpoints_(
				quotesEndpoints), dataReceiver_(dataReceiver) {

	for (size_t i = 0; i < quotesEndpoints_.size(); i++) {
		quoteEndpoints_.push_back(
				boost::asio::ip::udp::endpoint(
						boost::asio::ip::address::from_string("0.0.0.0"),
						quotesEndpoints[i].second));
		quoteSockets_.push_back(
				new boost::asio::ip::udp::socket(quoteService_));
		char *b = new char[buffMaxSize_];
		quoteBuffs_.push_back(b);
	}

	for (size_t i = 0; i < tradesEndpoints_.size(); i++) {
		tradeEndpoints_.push_back(
				boost::asio::ip::udp::endpoint(
						boost::asio::ip::address::from_string("0.0.0.0"),
						tradesEndpoints[i].second));
		tradeSockets_.push_back(
				new boost::asio::ip::udp::socket(tradeService_));
		char *b = new char[buffMaxSize_];
		tradeBuffs_.push_back(b);
	}
}

multicast_communication::~multicast_communication() {
	for (size_t i = 0; i < quoteBuffs_.size(); i++) {
		delete[] quoteBuffs_[i];
	}
	for (size_t i = 0; i < tradeBuffs_.size(); i++) {
		delete[] tradeBuffs_[i];
	}
}

void multicast_communication::prepareSockets() {
	using boost::asio::ip::udp;
	using boost::asio::ip::address;
	using boost::asio::ip::multicast::join_group;

	for (size_t i = 0; i < quotesEndpoints_.size(); i++) {
		quoteSockets_[i]->open(quoteEndpoints_[i].protocol());
		quoteSockets_[i]->set_option(udp::socket::reuse_address(true));
		quoteSockets_[i]->bind(quoteEndpoints_[i]);
		quoteSockets_[i]->set_option(
				join_group(address::from_string(quotesEndpoints_[i].first)));
	}

	for (size_t i = 0; i < tradesEndpoints_.size(); i++) {
		tradeSockets_[i]->open(tradeEndpoints_[i].protocol());
		tradeSockets_[i]->set_option(udp::socket::reuse_address(true));
		tradeSockets_[i]->bind(tradeEndpoints_[i]);
		tradeSockets_[i]->set_option(
				join_group(address::from_string(tradesEndpoints_[i].first)));
	}
}

void multicast_communication::registerQuoteListeners() {
	for (size_t i = 0; i < quotesEndpoints_.size(); i++) {
		quoteSockets_[i]->async_receive(
				boost::asio::buffer(quoteBuffs_[i], buffMaxSize_),
				boost::bind(&multicast_communication::listenQuoteHandler, this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred, i));
	}
}

void multicast_communication::registerTradeListeners() {
	for (size_t i = 0; i < tradesEndpoints_.size(); i++) {
		tradeSockets_[i]->async_receive(
				boost::asio::buffer(tradeBuffs_[i], buffMaxSize_),
				boost::bind(&multicast_communication::listenTradeHandler, this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred, i));
	}
}

void multicast_communication::listenQuoteHandler(
		const boost::system::error_code& error, std::size_t bytes_transferred,
		size_t bufferIndex) {
	if (error) {
		std::cout << "listener got error [" << error.value() << "]"
				<< error.message() << std::endl;
	} else {
		processQuoteMessage(bytes_transferred, bufferIndex);
		registerQuoteListeners();
	}
}

void multicast_communication::listenTradeHandler(
		const boost::system::error_code& error, std::size_t bytes_transferred,
		size_t bufferIndex) {
	if (error) {
		std::cout << "listener got error [" << error.value() << "]"
				<< error.message() << std::endl;
	} else {
		processTradeMessage(bytes_transferred, bufferIndex);
		registerTradeListeners();
	}
}

void multicast_communication::processQuoteMessage(std::size_t messageSize,
		size_t bufferIndex) {
	binReader bReader(quoteBuffs_[bufferIndex], messageSize);
	tqHeader_t messageHeader;
	quote_message qm;
	bReader.readMessageHeader(messageHeader);
	if (messageHeader.isShortQuote()) {
		shortQuote_t shortQuouteMsg;
		bReader.readShortQuote(shortQuouteMsg);
		extractQuoteMessage(shortQuouteMsg, qm);
		boost::lock_guard<boost::mutex> lock(mtx_);
		dataReceiver_.quotes_.push_back(qm);
	} else if (messageHeader.isLongQuote()) {
		longQuote_t longQuoteMsg;
		bReader.readLongQuote(longQuoteMsg);
		extractQuoteMessage(longQuoteMsg, qm);
		boost::lock_guard<boost::mutex> lock(mtx_);
		dataReceiver_.quotes_.push_back(qm);
	}
}

void multicast_communication::processTradeMessage(std::size_t messageSize,
		size_t bufferIndex) {
	binReader bReader(tradeBuffs_[bufferIndex], messageSize);
	tqHeader_t messageHeader;
	trade_message tm;
	bReader.readMessageHeader(messageHeader);
	if (messageHeader.isShortTrade()) {
		shortTrade_t shortTradeMsg;
		bReader.readShortTrade(shortTradeMsg);
		extractTradeMessage(shortTradeMsg, tm);
		boost::lock_guard<boost::mutex> lock(mtx_);
		dataReceiver_.trades_.push_back(tm);
	} else if (messageHeader.isLongTrade()) {
		longTrade_t longTradeMsg;
		bReader.readLongTrade(longTradeMsg);
		extractTradeMessage(longTradeMsg, tm);
		boost::lock_guard<boost::mutex> lock(mtx_);
		dataReceiver_.trades_.push_back(tm);
	}
}

void multicast_communication::kill() {
	for (size_t i = 0; i < quoteSockets_.size(); i++) {
		quoteSockets_[i]->cancel();
	}
	quoteService_.stop();

	for (size_t i = 0; i < tradeSockets_.size(); i++) {
		tradeSockets_[i]->cancel();
	}
	tradeService_.stop();
}

void multicast_communication::run() {
	prepareSockets();
	registerQuoteListeners();
	registerTradeListeners();

	boost::thread_group quotesPool;
	for (size_t i = 0; i < quoteThreadsCount_; i++) {
		quotesPool.create_thread(
				boost::bind(&multicast_communication::startQuoteServices,
						this));
	}

	boost::thread_group tradesPool;
	for (size_t i = 0; i < tradeThreadsCount_; i++) {
		tradesPool.create_thread(
				boost::bind(&multicast_communication::startTradeServices,
						this));
	}

	quotesPool.join_all();
	tradesPool.join_all();
}

void multicast_communication::startQuoteServices() {
	quoteService_.run();
}

void multicast_communication::startTradeServices() {
	tradeService_.run();
}

std::string multicast_communication::chars2string(const char* buffer,
		const size_t bufferSize) const {
	std::stringstream ss;
	for (size_t i = 0; i < bufferSize; i++) {
		ss << buffer[i];
	}
	return ss.str();
}

void multicast_communication::extractQuoteMessage(
		const shortQuote_t& shortQuote, quote_message& quoteMessage) {
	std::string strSecuritySymbol = chars2string(shortQuote.securitySymbol, 3);
	quoteMessage.security_symbol = strSecuritySymbol;
	quoteMessage.bid_price = atof(
			chars2string(shortQuote.bidShortPrice, 8).c_str())
			/ getDenominatorByChar(shortQuote.bidPriceDenominatorIndicator);
	quoteMessage.bid_volume = atof(
			chars2string(shortQuote.bidSizeUnitsOfTrade, 3).c_str());
	quoteMessage.offer_price = atof(
			chars2string(shortQuote.offerShortPrice, 8).c_str())
			/ getDenominatorByChar(shortQuote.offerPriceDenominatorIndicator);
	quoteMessage.offer_volume = atof(
			chars2string(shortQuote.offerSizeInUnitsOfTrade, 3).c_str());
}

void multicast_communication::extractQuoteMessage(const longQuote_t& longQuote,
		quote_message& quoteMessage) {
	std::string strSecuritySymbol = chars2string(longQuote.securitySymbol, 11);
	quoteMessage.security_symbol = strSecuritySymbol;
	quoteMessage.bid_price = atof(
			chars2string(longQuote.bidPrice_LowerLimitPriceBand, 12).c_str())
			/ getDenominatorByChar(
					longQuote.bidPrice_LowerLimitPriceBandDenominatorIndicator);
	quoteMessage.bid_volume = atof(
			chars2string(longQuote.bidSizeinUnitsofTrade, 7).c_str());
	quoteMessage.offer_price =
			atof(
					chars2string(longQuote.offerPrice_UpperLimitPriceBand, 12).c_str())
					/ getDenominatorByChar(
							longQuote.offerPrice_UpperLimitPriceBandDenominatorIndicator);
	quoteMessage.offer_volume = atof(
			chars2string(longQuote.offerSizeinUnitsofTrade, 7).c_str());
}

void multicast_communication::extractTradeMessage(
		const shortTrade_t& shortTrade, trade_message& tradeMessage) {
	std::string strSecuritySymbol = chars2string(shortTrade.securitySymbol, 3);
	tradeMessage.security_symbol = strSecuritySymbol;
	tradeMessage.price = atof(chars2string(shortTrade.tradePrice, 8).c_str())
			/ getDenominatorByChar(shortTrade.priceDenominatorIndicator);
	tradeMessage.volume = atof(chars2string(shortTrade.tradeVolume, 4).c_str());
}

void multicast_communication::extractTradeMessage(const longTrade_t& longTrade,
		trade_message& tradeMessage) {
	std::string strSecuritySymbol = chars2string(longTrade.securitySymbol, 11);
	tradeMessage.security_symbol = strSecuritySymbol;
	tradeMessage.price = atof(chars2string(longTrade.tradePrice, 12).c_str())
			/ getDenominatorByChar(longTrade.priceDenominatorIndicator);
	tradeMessage.volume = atof(chars2string(longTrade.tradeVolume, 9).c_str());
}

double multicast_communication::getDenominatorByChar(
		const char denominatorChar) {
	if (denominators.count(denominatorChar)) {
		return denominators.at(denominatorChar);
	} else {
		return 1.0;
	}
}
