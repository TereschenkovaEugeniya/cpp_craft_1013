#pragma once

#include "market_data_processor.h"

#include <boost/noncopyable.hpp>

namespace multicast_communication
{
    class market_data_receiver : public boost::noncopyable
    {
        market_data_processor& processor_;
    public:
        explicit market_data_receiver( market_data_processor& processor ):processor_(processor)
        {
        }
        ~market_data_receiver()
        {
        }
        void run()
        {
        }
        void stop()
        {
        }
    };

}
