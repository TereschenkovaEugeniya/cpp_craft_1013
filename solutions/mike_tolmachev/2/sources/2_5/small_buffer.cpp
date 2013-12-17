#include <fstream>
#include <string>
#include <stdint.h>
#include <map>


static const uint32_t MSG_SIZE = 2048;
static const uint32_t DATA_SIZE = 3*sizeof(uint32_t);

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

int main()
{
    std::ifstream in(BINARY_DIR "/input.txt", std::ifstream::binary);
    std::ofstream out(BINARY_DIR "/output.txt", std::ofstream::binary);

    if (in.is_open())
    {
		Msg msg;
		    
        std::map<uint32_t, std::map<uint32_t, std::pair<uint32_t, double>>> msgs;//<TYPE,<TIME<LEN, count>>>

		while(in >> msg)
        {
			in.seekg(msg.len, std::ios::cur);

			uint32_t msg_size = DATA_SIZE + msg.len;

            auto it = msgs[msg.type].find(msg.time);
            if (it == msgs[msg.type].end())
            {
                auto new_elem = msgs[msg.type].insert(std::make_pair(msg.time, std::make_pair(0, 0)));
                it = new_elem.first;
            }

			if ((it->second.first + msg_size) <= MSG_SIZE)
            {
				it->second.first += msg_size;
				++it->second.second;
            }
        }

        for (auto it = msgs.begin(); it != msgs.end(); ++it)
        {
            uint32_t type = it->first;
            out.write(reinterpret_cast<char*>(&type), sizeof(uint32_t));
			double count = 0;
			for (auto it_t = it->second.begin(); it_t != it->second.end(); ++it_t)
			{
				count += it_t->second.second;
			}
			count /= it->second.size();
			out.write(reinterpret_cast<char*>(&count), sizeof(double));
        }
    }

    in.close();
    out.close();
}