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

#include <trade_message.h>
#include <quote_message.h>

namespace multicast_communication
{
    class market_data_receiver : public boost::noncopyable
    {
        market_data_processor& processor_;
        
        
        typedef std::shared_ptr< boost::asio::io_service > io_ptr;
		typedef std::shared_ptr< async_udp::udp_listener > udp_listner_ptr;


        typedef std::vector< io_ptr > vector_io_ptr;
        typedef std::vector< udp_listner_ptr > vector_udp_listner_ptr;

        struct market_group
        {
            vector_io_ptr  io_group;
            boost::thread_group	threads;
            vector_udp_listner_ptr udp_listners_group;
        };
		

        market_group    trade_group;
        market_group    quote_group;

            
        template<typename container_message_ptr, typename message_ptr>
        void initialize_market_group( market_group& group, const size_t thread_size,  const vector_address_port& ports)
        {
            group.io_group.resize(thread_size);
            for( vector_io_ptr::iterator iter = group.io_group.begin(); iter != group.io_group.end(); ++iter )
	        {
                iter->reset(new boost::asio::io_service());
	        }

            group.udp_listners_group.resize(ports.size());
            
            for( auto iter = 0; iter != ports.size(); ++iter )
	        {
                group.udp_listners_group[ iter ].reset ( new async_udp::udp_listener(  (*group.io_group[ iter % thread_size ]), 
                    ports[iter].first, ports[iter].second,			
                            [&](const std::string& data)
		                    {
			                    container_message_ptr messages;
			                    if(! parse<container_message_ptr, message_ptr>(data,  messages) )
			                    {
                                    return; 
                                }

				                std::for_each(messages.begin(), messages.end(),
                                    [&](const message_ptr& msg)
					                {
                                        processor_.process(msg);
					                }
				                    );
		                    }));
            }


		    for( size_t i = 0; i < thread_size; i++ )
		    {
                group.threads.create_thread(
				    boost::bind( &multicast_communication::market_data_receiver::activate_reciev, this, group.io_group[i % group.io_group.size()]));
		    }
    

        }
        void uninitialize_market_group( market_group& group )
        {
            for( vector_io_ptr::iterator iter = group.io_group.begin(); iter != group.io_group.end(); ++iter )
            {
		        ( *iter )->stop();	
            }
	            
            group.threads.join_all();
        }

        void activate_reciev ( io_ptr service )
        {
            service->run();
        }


    public:
        explicit market_data_receiver( const size_t trade_thread_size,
                                       const size_t quote_thread_size,
                                       const vector_address_port& trade_ports,
                                       const vector_address_port& quote_ports,
                                       market_data_processor& processor )
            :processor_( processor )
        {

            initialize_market_group<trades_messages_ptr, trade_message_ptr>(trade_group, trade_thread_size, trade_ports);
            initialize_market_group<quotes_messages_ptr, quote_message_ptr>(quote_group, quote_thread_size, quote_ports);     
        }
        ~market_data_receiver()
        {
            try
            {
                uninitialize_market_group( trade_group );
                uninitialize_market_group( quote_group );

            }
            catch(...)
            {
            }
        }
    };

}
