#ifndef _COMMON_BUFFER_H_
#define _COMMON_BUFFER_H_

#include <boost/shared_ptr.hpp>

namespace common
{
	struct buffer
	{
		static const unsigned int size = 1024ul;
		char data[ size ];
		static unsigned int buffer_size();
	};

	typedef boost::shared_ptr< buffer > buffer_ptr;
}

#endif // _COMMON_BUFFER_H_

