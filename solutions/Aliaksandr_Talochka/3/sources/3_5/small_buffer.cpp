#include <reader.h>
#include <string>

#include <boost\thread.hpp>
#include <boost\bind.hpp>
#include <boost\lexical_cast.hpp>

static const size_t BufSize = 2048;


void count(std::map<uint32_t, uint32_t> &Msg_cnt, const uint32_t Type)
{
	std::map<uint32_t, uint32_t>::iterator it;
	
	it = Msg_cnt.find(Type);
	if(it != Msg_cnt.end())
		it->second ++;
	else
		Msg_cnt.insert(std::pair<uint32_t,uint32_t>(Type, 1));
}

void len_count(std::map<uint32_t, uint32_t> &Msg_len_cnt, const uint32_t Type, const uint32_t Len)
{
	std::map<uint32_t, uint32_t>::iterator it;
	
	it = Msg_len_cnt.find(Type);
	if(it != Msg_len_cnt.end())
		it->second += Len;
	else
		Msg_len_cnt.insert(std::pair<uint32_t,uint32_t>(Type, Len));
}



int process_file(const std::string &InFileName, std::map<uint32_t, uint32_t> *Msg_cnt, uint32_t *StartTime, uint32_t *EndTime, boost::mutex *doing_with_common_variables_mutex)
{
	std::fstream InFile;
	std::map<uint32_t, uint32_t> Msg_len_cnt;
	binary_reader::stock_ex_dat Data;
	uint32_t CurTime = 0;

	std::string BinaryDir = BINARY_DIR;
	
	InFile.open((BinaryDir + '/' + InFileName).c_str(),std::ios_base::binary|std::ios_base::in);


	if(InFile)
	{
		//Read and count messages
		while((binary_reader::read_binary(InFile, Data))==0)
		{
			if(Data.TIME < *StartTime) 
			{
				boost::mutex::scoped_lock lock(*doing_with_common_variables_mutex);
				*StartTime = Data.TIME;
				CurTime = Data.TIME;
			}

			if(Data.TIME!=CurTime)
			{
				CurTime = Data.TIME;
				Msg_len_cnt.clear();
			}
			
			len_count(Msg_len_cnt, Data.TYPE, Data.LEN + 3*sizeof(uint32_t));

			if (Msg_len_cnt[Data.TYPE] <= BufSize)
			{
				boost::mutex::scoped_lock lock(*doing_with_common_variables_mutex);
				count(*Msg_cnt, Data.TYPE);	
			}

			{
				boost::mutex::scoped_lock lock(*doing_with_common_variables_mutex);
				if (*EndTime < CurTime)
					*EndTime = CurTime;
			}
		
		}
		
	}

	InFile.close();
	

	return 0;
}


int main()
{
	std::fstream OutFile;
	std::map<uint32_t, uint32_t>::iterator it;

	static const size_t NumFiles = 999;
	boost::mutex doing_with_common_variables_mutex;
	boost::thread_group ThrFilterGroup;
	std::string InFileName,  OutFileName;
	
	//common variables
	uint32_t StartTime = -1;
	uint32_t EndTime = 0;
	std::map<uint32_t, uint32_t> Msg_cnt;

	for (size_t num = 1; num <= NumFiles; ++num)
	{
		std::string FileNum = boost::lexical_cast<std::string>(num);
		InFileName = "input_" + std::string("000" + FileNum).substr(FileNum.size(), 3) + ".txt";
		
		ThrFilterGroup.create_thread( boost::bind( &process_file, InFileName, &Msg_cnt, &StartTime, &EndTime, &doing_with_common_variables_mutex));
	}

	


	ThrFilterGroup.join_all();


	OutFile.open(BINARY_DIR"/Output.txt", std::ios_base::binary|std::ios_base::out|std::ios_base::trunc);
	if(OutFile)
	{

		//Evaluate rate
		for (it = Msg_cnt.begin(); it != Msg_cnt.end(); ++it)
		{
			double rate = (double(it->second))/(double (EndTime - StartTime + 1));
			OutFile.write((const char*)(&(it->first)), sizeof(uint32_t));
			OutFile.write((const char*)(&rate), sizeof(double));
		}

		OutFile.close();
	}

}

