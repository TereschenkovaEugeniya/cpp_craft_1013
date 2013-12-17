#ifndef COMMON_H_
#define COMMON_H_

#include <stdint.h>
#include <string>
#include <vector>

struct dataStruct_t{
	uint32_t type;
	uint32_t time;
	std::string msg;
};

typedef std::vector<dataStruct_t> dataVector_t;

#endif /* COMMON_H_ */
