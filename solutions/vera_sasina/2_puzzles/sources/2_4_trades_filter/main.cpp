#include <reader.h>
#include <stdint.h>
#include <fstream>

int main()
{
	std::ifstream input_file( BINARY_DIR "/input.txt", std::ios::binary|std::ios::ate );
	if ( !input_file.is_open() )
		return -1;
	std::ofstream output_file( BINARY_DIR "/output.txt", std::ios::binary );
	if ( !output_file.is_open() )
		return -1;
	size_t file_size;
	file_size = static_cast<size_t>(input_file.tellg());
	input_file.seekg (0, std::ios::beg);
	
	size_t read_size = 0;
	binary_reader::Data data;
	uint32_t maxTime = 0;
	while(read_size < file_size)
	{
		binary_reader::read_struct(input_file, data);
		read_size = static_cast<size_t>(input_file.tellg());
		if(data.type > 4)
			continue;
		if((data.time+2) <= maxTime)
			continue;
		if(data.time > maxTime)
			maxTime = data.time;

		output_file.write((char*) &data.type, sizeof(data.type));
		output_file.write((char*) &data.time, sizeof(data.time));
		output_file.write((char*) &data.len, sizeof(data.len));
		if(data.len > 0)
		{
			//if message isn't empty
			output_file.write((char*) data.msg, data.len);
			delete [] data.msg;
		}
	}
	input_file.close();
	output_file.close();
	return 0;
}
