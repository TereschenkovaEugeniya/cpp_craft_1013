#include "multicast_communication.h"

const size_t multicast_communication::buffMaxSize_ = 1024;


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
	bReader.readMessageHeader(messageHeader);
	if (messageHeader.isShortQuote()) {
		shortQuote_t shortQuouteMsg;
		bReader.readShortQuote(shortQuouteMsg);
		boost::lock_guard<boost::mutex> lock(mtx_);
		dataReceiver_.addQuote(shortQuouteMsg);
	} else if (messageHeader.isLongQuote()) {
		longQuote_t longQuoteMsg;
		bReader.readLongQuote(longQuoteMsg);
		boost::lock_guard<boost::mutex> lock(mtx_);
		dataReceiver_.addQuote(longQuoteMsg);
	}
}

void multicast_communication::processTradeMessage(std::size_t messageSize,
		size_t bufferIndex) {
	binReader bReader(tradeBuffs_[bufferIndex], messageSize);
	tqHeader_t messageHeader;
	bReader.readMessageHeader(messageHeader);
	if (messageHeader.isShortTrade()) {
		shortTrade_t shortTradeMsg;
		bReader.readShortTrade(shortTradeMsg);
		boost::lock_guard<boost::mutex> lock(mtx_);
		dataReceiver_.addTrade(shortTradeMsg);
	} else if (messageHeader.isLongTrade()) {
		longTrade_t longTradeMsg;
		bReader.readLongTrade(longTradeMsg);
		boost::lock_guard<boost::mutex> lock(mtx_);
		dataReceiver_.addTrade(longTradeMsg);
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
