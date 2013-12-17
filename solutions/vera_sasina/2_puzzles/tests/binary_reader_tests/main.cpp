#include <reader.h>
#include <iostream>
#include <fstream>

#define test_no_throw( action ) \
{ \
	try \
	{ \
		action; \
	} \
	catch( ... ) \
	{ \
		std::cout << "test_no_throw failed: " <<  __FILE__ << ":" << __LINE__ << std::endl; \
	} \
} \

void read_file(std::ifstream &f, binary_reader::Data &d)
{
	size_t file_size;
	file_size = static_cast<size_t>(f.tellg());
	f.seekg (0, std::ios::beg);
	size_t read_size = 0;
	while(read_size < file_size)
	{
		binary_reader::read_struct(f, d);
		read_size = static_cast<size_t>(f.tellg());
	}
}

int main()
{
	std::ifstream file( SOURCE_DIR "/tests/data/input1.txt",  std::ios::binary|std::ios::ate );
	if ( !file.is_open() )
		return -1;
	binary_reader::Data data;
	test_no_throw( read_file(file, data) );
	file.close();
	return 0;
}