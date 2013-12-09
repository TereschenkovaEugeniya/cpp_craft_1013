#ifndef MULTICASTCOMMUNICATION_H_
#define MULTICASTCOMMUNICATION_H_

#include "dataStructs.h"
#include "binReader.h"
#include "marketdatareceiver.h"
#include "quote_message.h"
#include "trade_message.h"

#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/noncopyable.hpp>
#include <boost/system/error_code.hpp>

typedef std::pair<std::string, unsigned short> udpEndpoint_t;
typedef std::vector<udpEndpoint_t> udpEndpoints_t;

#define buffMaxSize_ 1024

class multicast_communication:public boost::noncopyable{
public:
	explicit multicast_communication(
			size_t tradeThreadsCount,
			size_t quoteThreadsCount,
			udpEndpoints_t tradesEndpoints,
			udpEndpoints_t qoutesEndpoints,
			market_data_receiver& dataReceiver);
	virtual ~multicast_communication();

	void kill();
	void run();

private:
	size_t tradeThreadsCount_;
	size_t quoteThreadsCount_;
	udpEndpoints_t tradesEndpoints_;
	udpEndpoints_t quotesEndpoints_;
	market_data_receiver& dataReceiver_;

	std::vector<char*> quoteBuffs_;
	boost::asio::io_service quoteService_;
	std::vector<boost::asio::ip::udp::endpoint> quoteEndpoints_;
	std::vector<boost::asio::ip::udp::socket*> quoteSockets_;
	std::vector<char*> tradeBuffs_;
	boost::asio::io_service tradeService_;
	std::vector<boost::asio::ip::udp::endpoint> tradeEndpoints_;
	std::vector<boost::asio::ip::udp::socket*> tradeSockets_;

	void prepareSockets();
	void registerQuoteListeners();
	void listenQuoteHandler(const boost::system::error_code& error, std::size_t bytes_transferred, size_t bufferIndex);
	void processQuoteMessage(std::size_t messageSize, size_t bufferIndex);
	void registerTradeListeners();
	void listenTradeHandler(const boost::system::error_code& error, std::size_t bytes_transferred, size_t bufferIndex);
	void processTradeMessage(std::size_t messageSize, size_t bufferIndex);
	void startQuoteServices();
	void startTradeServices();
	std::string chars2string(const char* buffer, const size_t bufferSize)const;
	void extractQuoteMessage(const shortQuote_t& shortQuote, quote_message& quoteMessage);
	void extractQuoteMessage(const longQuote_t& longQuote, quote_message& quoteMessage);
	void extractTradeMessage(const shortTrade_t& shortTrade, trade_message& tradeMessage);
	void extractTradeMessage(const longTrade_t& longTrade, trade_message& tradeMessage);
};

#endif /* MULTICASTCOMMUNICATION_H_ */
