#include <reader.h>
#include <string>

#include <boost\thread.hpp>
#include <boost\bind.hpp>
#include <boost\lexical_cast.hpp>



int filter(const std::string &InFileName, const std::string &OutFileName)
{
	std::fstream InFile;
	std::fstream OutFile;
	binary_reader::stock_ex_dat Data;
	uint32_t CurTime = 0;

	std::string BinaryDir = BINARY_DIR;

	InFile.open((BinaryDir + '/' + InFileName).c_str(),std::ios_base::binary|std::ios_base::in);


	if(InFile)
	{
		OutFile.open((BinaryDir + '/' + OutFileName).c_str(), std::ios_base::binary|std::ios_base::out|std::ios_base::trunc);

		while((binary_reader::read_binary(InFile, Data))==0)
		{
			if( (Data.TYPE >= 1)&&(Data.TYPE <= 4) )
			{
				if ((Data.TIME > CurTime-2)||(CurTime < 2))
				{
					OutFile.write((char*)(&Data.TYPE), 3*(sizeof(uint32_t)));
					OutFile.write(Data.MSG.c_str(), Data.LEN);

					if(Data.TIME > CurTime)
						CurTime = Data.TIME;
				}
			}
		}	
	}
//	else 
//	{
//		std::cout << "Could not read file: " << InFileName.c_str() << std::endl;
//	}

	InFile.close();

	if(OutFile)
		OutFile.close();

	return 0;
}



int main()
{
	static const size_t NumFiles = 999;
	boost::thread_group ThrFilterGroup;
	std::string InFileName,  OutFileName;

	for (size_t num = 1; num <= NumFiles; ++num)
	{
		std::string FileNum = boost::lexical_cast<std::string>(num);
		InFileName = "input_" + std::string("000" + FileNum).substr(FileNum.size(), 3) + ".txt";
		OutFileName = "output_" + std::string("000" + FileNum).substr(FileNum.size(), 3) + ".txt";
		
		ThrFilterGroup.create_thread( boost::bind( &filter, InFileName, OutFileName));
	}


	ThrFilterGroup.join_all();

	//system("pause");

}

