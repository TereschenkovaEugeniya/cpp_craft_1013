#include <fstream>
#include <stdint.h>
#include <memory>
#include <map>

static const size_t buf_size = 4096;

struct Msg
{
    uint32_t type;
    uint32_t time;
    uint32_t len;
};

std::istream& operator >> (std::istream& in, Msg& msg)
{
    in.read(reinterpret_cast<char*>(&msg.type), sizeof(uint32_t));
    in.read(reinterpret_cast<char*>(&msg.time), sizeof(uint32_t));
    in.read(reinterpret_cast<char*>(&msg.len), sizeof(uint32_t));
    return in;
}

std::ostream& operator << (std::ostream& out, Msg& msg)
{
    out.write(reinterpret_cast<char*>(&msg.type), sizeof(uint32_t));
    out.write(reinterpret_cast<char*>(&msg.time), sizeof(uint32_t));
    out.write(reinterpret_cast<char*>(&msg.len), sizeof(uint32_t));
    return out;
}

int main()
{
    std::ifstream in(BINARY_DIR "/input.txt", std::ifstream::binary);
    std::ofstream out(BINARY_DIR "/output.txt", std::ofstream::binary);

    uint32_t T = 0;
    
    if (in.is_open())
    {
        Msg msg;

        while(in >> msg)
        {
            if (((msg.time + 2) > T) && (1 <= msg.type) && (msg.type <= 4))
            {
                out << msg;

				char buf[4096];
				uint32_t len = 0;
                for (len = msg.len; len > buf_size; len -= buf_size)
                {
					in.read(buf, buf_size);
					out.write(buf, buf_size);
                }
				in.read(buf, len);
				out.write(buf, len);

                if (msg.time > T)
                {
                    T = msg.time;
                }
            }
            else
            {
                in.seekg(msg.len, std::ios::cur);
            }
        }
    }

    in.close();
    out.close();
}