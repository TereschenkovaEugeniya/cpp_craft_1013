#include "binary_datafeed.h"

int main() {

    BinaryDatafeed::process( 
            SOURCE_DIR "/tests/input_261.txt", 
            SOURCE_DIR "/tests/output_261.txt");

    return 0;
}

