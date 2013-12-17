#include "test_registrator.h"

#include <mediator.h>

#include <fstream>

#include <boost/filesystem.hpp>

void minute_calculator::tests_::mediator_tests()
{
	boost::filesystem::remove( BINARY_DIR "/configs.ini" );
	BOOST_CHECK_THROW( mediator m( BINARY_DIR );, std::logic_error );
	boost::filesystem::copy( SOURCE_DIR "/tests/data/config/good_config_1.ini", BINARY_DIR "/configs.ini" );

	BOOST_CHECK_NO_THROW( mediator m( BINARY_DIR ); );
	boost::filesystem::remove( BINARY_DIR "/configs.ini" );
}
