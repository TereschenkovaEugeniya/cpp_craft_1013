#include "test_registrator.h"

#include <processors_test_helper.h>

void multicast_communication::tests_::market_data_processor_tests()
{
	BOOST_CHECK_NO_THROW
	( 
		market_data_processor_test_helper mdpth;
	)
}
