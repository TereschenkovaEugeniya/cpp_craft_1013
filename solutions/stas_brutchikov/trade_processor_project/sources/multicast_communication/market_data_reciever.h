#pragma once

#include "udp_listener.h"

#include "base.h"
#include "market_data_processor.h"

#include <boost/noncopyable.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include <vector>

namespace multicast_communication
{
    class market_data_receiver : public boost::noncopyable
    {
        market_data_processor& processor_;
        
        
        typedef std::shared_ptr< boost::asio::io_service > io_ptr;
		typedef std::shared_ptr< async_udp::udp_listener > udp_listner_ptr;
		
        typedef std::vector< io_ptr > vector_service_ptr;
        typedef std::vector< udp_listner_ptr > vector_udp_listner_ptr;
        vector_service_ptr io_trades_;
		vector_service_ptr io_quotes_;
		vector_udp_listner_ptr udp_listners_trades;
		vector_udp_listner_ptr udp_listners_quotes;



    public:
        explicit market_data_receiver( const size_t trade_thread_size,
                                       const size_t quote_thread_size,
                                       const vector_adres_port& trade_ports,
                                       const vector_adres_port& quote_ports,
                                       market_data_processor& processor )
            :processor_(processor)
        {
            // init trades
	        io_trades_.resize(trade_thread_size);

            for(vector_service_ptr::iterator iter = io_trades_.begin(); iter != io_trades_.end(); ++iter)
	        {
                iter->reset(new boost::asio::io_service());
	        }

            udp_listners_trades.resize(trade_ports.size());
            for(auto iter = 0; iter != trade_ports.size(); ++iter)
	        {
                udp_listners_trades[ iter ].reset ( new async_udp::udp_listener( ( io_trades_[ iter % trade_thread_size ], 
                    trade_ports[iter].first, trade_ports[iter].second,			
                            [&](const std::string& data)
		                    {
			                    quote_messages_ptr messages;
			                    if( parse(data,  messages) )
			                    {
				                    std::for_each(messages.begin(), messages.end(),
                                        [&](quote_messages_ptr &msg)
					                    {
						                    processor_.new_quote(msg);
					                    }
				                    );
			                    }
		                    } );
	        }

            


            // ini quotes
            io_quotes_.resize(trade_thread_size);
	        for(auto i = io_quotes_.begin(); i != io_quotes_.end(); i++)
	        {
		        i->reset(new boost::asio::io_service());
	        }



        }
        ~market_data_receiver()
        {
            try
            {



            }
            catch(...)
            {
            }
        }
    };

}
