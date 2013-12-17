#pragma once

#include <iostream>

#include <boost/thread/recursive_mutex.hpp>
#include <boost/noncopyable.hpp>

#include "market_data_processor.h"

namespace multicast_communication
{
    class dummy_processor: public market_data_processor,
        public boost::noncopyable
    {    
        typedef boost::recursive_mutex::scoped_lock locker;
    public:
        dummy_processor(std::ostream& out):out_(out){};
        ~dummy_processor(){};
    private:
        virtual void new_trade( const trade_message_ptr& msg)
        {
            locker lock(mutex_);
            out_ << msg;
        }
        virtual void new_quote( const quote_message_ptr& msg)
        {
            locker lock(mutex_);
            out_ << msg;
        }      
        std::ostream &out_;
        boost::recursive_mutex mutex_;        
    };

}
