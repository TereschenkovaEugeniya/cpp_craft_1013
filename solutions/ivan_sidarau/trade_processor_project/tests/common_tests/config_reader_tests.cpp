#include "test_registrator.h"

#include <config_reader.h>

void common::tests_::config_reader_tests()
{
	BOOST_CHECK_THROW( config_reader( SOURCE_DIR "/tests/data/config/bad_config_1.ini" );, std::logic_error );
	BOOST_CHECK_THROW( config_reader( SOURCE_DIR "/tests/data/config/bad_config_2.ini" );, std::logic_error );
	BOOST_CHECK_THROW( config_reader( SOURCE_DIR "/tests/data/config/bad_config_3.ini" );, std::logic_error );
	BOOST_CHECK_THROW( config_reader( SOURCE_DIR "/tests/data/config/bad_config_4.ini" );, std::logic_error );

	BOOST_CHECK_NO_THROW( config_reader( SOURCE_DIR "/tests/data/config/good_config_1.ini" ); );
}
