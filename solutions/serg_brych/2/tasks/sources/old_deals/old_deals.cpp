#include <iostream> 
#include <fstream> 
#include <algorithm>
#include <stdint.h>
#include <memory>

static const uint32_t max_type = 4u;
static const int BufferSize = 2048;

struct deal
{
	uint32_t type;
	uint32_t time;
	uint32_t len;
};

int main() 
{ 
	uint32_t max_time = 0;
	char buffer[BufferSize] = {0};
	deal current_deal ={0};
	bool store_deal = false;

	std::ifstream input_file(BINARY_DIR"\\input.txt", std::ifstream::binary);
	std::ofstream output_file(BINARY_DIR"\\output.txt", std::ifstream::binary);

	if(input_file.is_open() && output_file.is_open())
	{
		while(input_file.read(reinterpret_cast<char*>(&current_deal), sizeof(deal)))
		{
			store_deal = false;

			if((current_deal.type > 0) && ( current_deal.type <= max_type))
			{
				if(max_time < current_deal.time)
				{
					max_time = current_deal.time;
					store_deal = true;
				}
				else
				{
					if((current_deal.time + 2) > max_time)
					{
						store_deal = true;
					}
				}
			}

			if(store_deal)
			{
				output_file.write(reinterpret_cast<char*>(&current_deal), sizeof(deal));
				uint32_t read_bytes_msg = 0;
				uint32_t bytes_to_read = 0;
				while(read_bytes_msg < current_deal.len)
				{
					bytes_to_read = BufferSize < current_deal.len? BufferSize: current_deal.len;

					input_file.read(buffer, bytes_to_read);
					output_file.write(buffer, bytes_to_read);
					read_bytes_msg += bytes_to_read;
				}
				
			}
			else
			{
				std::streampos pos = input_file.tellg();
				input_file.seekg(pos + static_cast<std::streamoff>(current_deal.len));
			}
		}
	}
	
	return 0;
}