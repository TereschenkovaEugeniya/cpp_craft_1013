#include <time_helper.h>

#include <boost/date_time/posix_time/posix_time_duration.hpp>

boost::uint64_t common::create_uint_time( long hours, long minutes, long seconds, long microsec )
{
	boost::posix_time::ptime t( today, boost::posix_time::time_duration( hours, minutes, seconds, microsec ) );
	return (t - start_epoch_time).total_microseconds();
}
boost::posix_time::ptime common::create_ptime( long hours, long minutes, long seconds, long microsec )
{
	return boost::posix_time::ptime( today, boost::posix_time::time_duration( hours, minutes, seconds, microsec ) );
}

boost::uint64_t common::convert( const boost::posix_time::ptime& ptime )
{
	return (ptime - start_epoch_time).total_microseconds();
}
boost::posix_time::ptime common::convert( const boost::uint64_t time )
{
	return (start_epoch_time + boost::posix_time::time_duration( boost::posix_time::microseconds( time ) ));
}
