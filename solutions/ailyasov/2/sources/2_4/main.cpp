#include "trades_filter.h"

int main() {
    TradesFilter::filter_trades_and_save(
            SOURCE_DIR "/tests/input_241.txt",
            SOURCE_DIR "/tests/output_241.txt");
    return 0;
} 
