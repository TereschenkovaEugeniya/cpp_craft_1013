#ifndef TOOLS_H_
#define TOOLS_H_

#include "common.h"

bool readDataVectorFromFile(dataVector_t& vec, const std::string& filename);
bool writeDataVectorToFile(dataVector_t& vec, const std::string& filename);

#endif /* TOOLS_H_ */
