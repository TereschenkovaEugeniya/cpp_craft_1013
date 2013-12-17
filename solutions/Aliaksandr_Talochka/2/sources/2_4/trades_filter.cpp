#include "reader.h"
#include <stdint.h>

int main()
{
	std::fstream InFile;
	std::fstream OutFile;
	binary_reader::stock_ex_dat Data;
	uint32_t CurTime = 0;


	InFile.open(BINARY_DIR"/Input.txt",std::ios_base::binary|std::ios_base::in);
	OutFile.open(BINARY_DIR"/Output.txt", std::ios_base::binary|std::ios_base::out|std::ios_base::trunc);

	if(InFile)
	{
		while((binary_reader::read_binary(InFile, Data))==0)
		{
			if( (Data.TYPE >= 1)&&(Data.TYPE <= 4) )
			{
				if ((Data.TIME >= CurTime-2)||(CurTime < 2))
				{
					OutFile.write((char*)(&Data.TYPE), 3*(sizeof(uint32_t)));
					OutFile.write(Data.MSG.c_str(), Data.LEN);

					if(Data.TIME > CurTime)
						CurTime = Data.TIME;
				}
			}
		}	
	}
	else 
	{
		std::cout << "Could not read Input.txt file" << std::endl;
	}

	InFile.close();
	OutFile.close();

	//system("pause");
	return 0;
}

