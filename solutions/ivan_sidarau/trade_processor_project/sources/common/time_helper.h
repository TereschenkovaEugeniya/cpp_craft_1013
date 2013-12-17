#ifndef _COMMON_TIME_HELPER_H_
#define _COMMON_TIME_HELPER_H_

#include <boost/cstdint.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/date_time/time_clock.hpp>

namespace common
{
	static const boost::posix_time::ptime start_epoch_time( boost::gregorian::date( 1900, 1, 1 ) );
	static const boost::gregorian::date today( 
		boost::date_time::second_clock< boost::posix_time::ptime >::universal_time().date() );

	boost::uint64_t create_uint_time( long hours, long minutes, long seconds, long microsec = 0 );
	boost::posix_time::ptime create_ptime( long hours, long minutes, long seconds, long microsec = 0 );
	boost::uint64_t convert( const boost::posix_time::ptime& ptime );
	boost::posix_time::ptime convert( const boost::uint64_t time );
}

#endif // _COMMON_TIME_HELPER_H_

