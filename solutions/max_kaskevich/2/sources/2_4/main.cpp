#include "message.h"
#include <fstream>
#include <stdint.h>

enum msg_type: uint32_t
{
    MARKET_OPEN = 1u,
    TRADE = 2u,
    QUOTE = 3u,
    MARKET_CLOSE = 4u    
};

int main( int argc, char* argv[] )
{
    std::ifstream input(BINARY_DIR "/input.txt", std::ios::binary);
    std::ofstream output(BINARY_DIR "/output.txt", std::ios::binary);
    if(!input || !output)
    {
        return 1;
    }

    uint32_t T = 0;
    message m;
    while(input >> m)
    {
        if(m.type >= MARKET_OPEN && m.type <= MARKET_CLOSE && m.time + 2 > T )
        {
            if( !(output << m ) )
            {
                return 1;
            }
            // redirect msg to output
            if (input && m.len)
            {
				uint32_t len = m.len;
				static const size_t buf_size = 4096;
				char buf[buf_size];
				while (len > buf_size)
				{
					input.read(buf, buf_size);
					output.write(buf, buf_size);
					len -= buf_size;
				}
				input.read(buf, len);
				output.write(buf, len);
            }

            T = std::max(T, m.time);
        }
        else
        {
            // miss msg
            if (input)
            {
                input.seekg(m.len, std::ios::cur);
            }
        }
    }
}
