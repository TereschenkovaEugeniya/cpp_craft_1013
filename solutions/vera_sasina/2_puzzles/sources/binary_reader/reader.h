#ifndef _BINARY_READER_READER_H_
#define _BINARY_READER_READER_H_

// this #ifndef + #define + #endif called header (include) guards: http://ru.wikipedia.org/wiki/Include_guard
#include <stdint.h>
#include <fstream>
#include <limits>

namespace binary_reader
{
	struct Data
	{
		uint32_t type;
		uint32_t time;
		uint32_t len;
		char *msg;
	};
	void read_struct(std::ifstream&, Data&, uint32_t type_max = std::numeric_limits<uint32_t>::max());
}

#endif // _BINARY_READER_READER_H_
