#include <vector>
#include <string>

#include <boost/thread.hpp>
#include <boost/regex.hpp>
#include <boost/cstdint.hpp>
#include "binary_datafeed.h"


int main() {

    BinaryDatafeed::process( 
            SOURCE_DIR "/tests/input_361.txt", 
            SOURCE_DIR "/tests/output_361.txt");

    return 0;
}

