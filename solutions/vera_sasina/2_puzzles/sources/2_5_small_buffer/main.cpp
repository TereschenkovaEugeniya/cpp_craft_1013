#include <reader.h>
#include <map>

typedef std::map<uint32_t, uint32_t> uint32_map;
const static uint32_t max_type = 100000;
const static size_t size_buffer = 2048;

int main()
{
	std::ifstream input_file( BINARY_DIR "/input.txt",  std::ios::binary|std::ios::ate );
	if ( !input_file.is_open() )
		return -1;
	std::ofstream output_file( BINARY_DIR "/output.txt",  std::ios::binary );
	if ( !output_file.is_open() )
		return -1;
	size_t file_size;
	file_size = static_cast<size_t>(input_file.tellg());
	input_file.seekg (0, std::ios::beg);
	
	binary_reader::Data net_data;
	uint32_map results;
	uint32_map buffer;
	size_t read_size = 0;
	uint32_t current_time = 0;

	while(read_size < file_size)
	{
		binary_reader::read_struct(input_file, net_data, max_type);
		read_size = static_cast<size_t>(input_file.tellg());
		if(net_data.time+1 != current_time)	//time+1 for time == 0
		{
			buffer.clear();
			current_time = net_data.time+1;
		}
		if((buffer[net_data.type] + net_data.len + sizeof(uint32_t)*3) <= size_buffer)
		{
			buffer[net_data.type] += net_data.len + sizeof(uint32_t)*3;
			results[net_data.type]++;
		}

		if(net_data.len > 0)
			delete [] net_data.msg;
	}

	uint32_t full_time = net_data.time;
	double average;
	for(uint32_map::iterator it = results.begin(); it != results.end(); ++it)
	{
		if(it->second > 0)
		{
			average = static_cast<double>(it->second) / static_cast<double>(full_time);
			output_file.write((char*) &it->first, sizeof(it->first));
			output_file.write((char*) &average, sizeof(average));
		}
	}
	input_file.close();
	output_file.close();
	return 0;
}