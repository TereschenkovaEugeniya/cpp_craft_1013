#include "test_registrator.h"

#include <time_helper.h>

void common::tests_::time_helper_tests()
{
	using namespace boost::posix_time;
	using namespace boost::gregorian;

	const ptime t1 = ptime( date( 2013, 12, 13 ), time_duration( 14, 15, 16 ) );
	const boost::uint64_t time = convert( t1 );

	BOOST_CHECK_EQUAL( t1.time_of_day().hours(), 14 );
	BOOST_CHECK_EQUAL( t1.time_of_day().minutes(), 15 );

	const ptime t2 = convert( time );
	const ptime t3 = convert( time );
	const boost::uint64_t time2 = convert( t2 );

	BOOST_CHECK_EQUAL( t1 == t3, true );

	BOOST_CHECK_EQUAL( t1.time_of_day().hours(), t2.time_of_day().hours() );
	BOOST_CHECK_EQUAL( t1.time_of_day().minutes(), t2.time_of_day().minutes() );

	BOOST_CHECK_EQUAL( t1.date().year(), t2.date().year() );
	BOOST_CHECK_EQUAL( t1.date().month(), t2.date().month() );

	BOOST_CHECK_EQUAL( convert( create_uint_time( 1, 2, 3 ) ) == create_ptime( 1, 2, 3 ), true );

	BOOST_CHECK_EQUAL( create_uint_time( 1, 2, 3, 3 ) < convert( create_ptime( 1, 2, 3, 4 ) ), true );
	BOOST_CHECK_EQUAL( create_uint_time( 1, 2, 3, 5 ) > convert( create_ptime( 1, 2, 3, 4 ) ), true );
	BOOST_CHECK_EQUAL( create_uint_time( 1, 2, 3, 44 ), convert( create_ptime( 1, 2, 3, 44 ) ) );

}
