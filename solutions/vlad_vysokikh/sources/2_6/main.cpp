#include  "../binary_reader/market_message.h"
#include <iostream>

int main()
{
    std::fstream in( SOURCE_DIR "/2.6_input.in", std::ios_base::in | std::ios_base::binary );
    std::fstream out( SOURCE_DIR "/2.6_output.out", std::ios_base::out | std::ios_base::binary );
    if ( ! (in.is_open() && out.is_open()) ) {
        return 1;
    }

    while (true) {
        binary_reader::MarketMessage message;
        message.read( in );

        if ( in.eof() ) {
            break;
        }
        message.write( out );
    }

    return 0;
}
