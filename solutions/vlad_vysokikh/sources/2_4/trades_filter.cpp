#include "../binary_reader/message.h"

int main()
{
    boost::uint32_t maxTime = 0;
    std::fstream in( SOURCE_DIR "/2.4_input.in", std::ios_base::in | std::ios_base::binary );
    std::fstream out( SOURCE_DIR "/2.4_output.out", std::ios_base::out | std::ios_base::binary );

    if ( ! (in.is_open() && out.is_open()) ) {
        return 1;
    }

    while ( true ) {
        binary_reader::Message message;
        message.read(in);

        if ( in.eof() ) {
            break;
        }

        if ( (message.getTime() + 2 > maxTime) && ( message.getType() < 5 ) )
        {
            if (message.getTime() > maxTime ) { maxTime = message.getTime();}
            message.write(out);
        }
    }
}

