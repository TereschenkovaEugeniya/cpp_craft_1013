#ifndef MARKETDATARECEIVER_H_
#define MARKETDATARECEIVER_H_

#include "market_data_processor.h"
#include <boost/noncopyable.hpp>

class market_data_receiver: public market_data_processor, public boost::noncopyable {
public:
	market_data_receiver();
	virtual ~market_data_receiver();

	virtual void new_trade(const trade_message_ptr& trade_ptr);
	virtual void new_quote(const quote_message_ptr& quote_ptr);

	std::vector<trade_message> trades_;
	std::vector<quote_message> quotes_;
};

#endif /* MARKETDATARECEIVER_H_ */
