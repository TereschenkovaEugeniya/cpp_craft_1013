
#include "../binary_reader/message.h"
#include <vector>

int main()
{
    static const boost::uint32_t maxType = 1000000;
    static const boost::uint32_t buffer_size = 2048;

    std::fstream in( SOURCE_DIR "/2.5_input.in", std::ios_base::in | std::ios_base::binary );
    std::fstream out( SOURCE_DIR "/2.5_output.out", std::ios_base::out | std::ios_base::binary );
    if ( ! (in.is_open() && out.is_open()) ) {
        return 1;
    }

    std::vector<boost::uint32_t> counts( maxType, 0 ), buffers( maxType, 0 );
    boost::uint32_t currentTime = 1;

    while ( true ) {
        binary_reader::Message message;
        message.read(in);

        if ( in.eof() ) {
            break;
        }

        if (currentTime != message.getTime()) {
            currentTime = message.getTime();
            std::fill(buffers.begin(), buffers.end(), 0);
        }

        if ( message.getType() < maxType ) {
            buffers.at( message.getType() )  += message.size();
            if ( buffers.at( message.getType() ) < buffer_size ) {
                counts.at( message.getType() ) += 1;
            }
        }
    }
    //std::cout << currentTime << std::endl;
    for ( boost::uint32_t i = 1; i <= buffer_size; i++ ) {
        if ( counts.at(i) != 0) {
            //std::cout << i << " " << (double) counts.at(i) / currentTime << std::endl;
            double value = (double) counts.at(i) / currentTime;
            out.write( reinterpret_cast<char*>(&i), sizeof(i) );
            out.write( reinterpret_cast<char*>(&value), sizeof(value));
        }
    }
}

