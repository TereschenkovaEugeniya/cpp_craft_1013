#ifndef MARKETDATARECEIVER_H_
#define MARKETDATARECEIVER_H_

#include "dataStructs.h"
#include "denominator.h"
#include "market_data_processor.h"

#include <boost/noncopyable.hpp>
#include <string>
#include <cstdlib>
#include <sstream>

class market_data_receiver: public market_data_processor, public boost::noncopyable {
public:
	market_data_receiver();
	virtual ~market_data_receiver();

	virtual void addTrade(const trade_message_ptr&);
	virtual void addQuote(const quote_message_ptr&);

	virtual void addTrade(const trade_message& trade_ptr);
	virtual void addQuote(const quote_message& quote_ptr);
	virtual void addTrade(const shortTrade_t& shortTrade);
	virtual void addTrade(const longTrade_t& longTrade);
	virtual void addQuote(const shortQuote_t& shortQuote);
	virtual void addQuote(const longQuote_t& longQuote);

	std::vector<trade_message> trades_;
	std::vector<quote_message> quotes_;

private:
	denominator denominator_;

	std::string chars2string(const char* buffer, const size_t bufferSize) const;
};

#endif /* MARKETDATARECEIVER_H_ */
