#include <reader.h>
#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <fstream>



int main()
{
	std::fstream InFile;
	std::fstream OutFile;

	binary_reader::stock_ex_dat file_data;

	struct Data
	{
		uint32_t TYPE;
		uint32_t TIME; 
		uint32_t LEN;
		std::string MSG;
	} trade_data;

	trade_data.TYPE = 2;
	trade_data.TIME = 28;
	trade_data.LEN = 6;
	trade_data.MSG = "Hello!";

	
	OutFile.open(BINARY_DIR"/Input_test.txt", std::ios_base::binary|std::ios_base::out|std::ios_base::trunc);
	if(OutFile)
	{
		OutFile.write((char*) &trade_data.TYPE, sizeof(uint32_t));
		OutFile.write((char*) &trade_data.TIME, sizeof(uint32_t));
		OutFile.write((char*) &trade_data.LEN, sizeof(uint32_t));
		OutFile.write(trade_data.MSG.c_str(), trade_data.LEN);

		OutFile.close();

		InFile.open(BINARY_DIR"/Input_test.txt",std::ios_base::binary|std::ios_base::in);
		if(InFile)
		{
			binary_reader::read_binary(InFile, file_data);

			if( (file_data.TYPE == trade_data.TYPE) 
				&& (file_data.TIME == trade_data.TIME) 
				&& (file_data.LEN == trade_data.LEN)
				&& (file_data.MSG == trade_data.MSG))
				
				std::cout << "Test is passed " << std::endl;
			
			else
				std::cout << "Test is failed: reading is incorrect " << std::endl;
					
			
		}
	}


	system("pause");
	return 0;
}


