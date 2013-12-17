#include <reader.h>
#include <map>
#include <list>

#include <boost/thread.hpp>

std::map <unsigned __int32, unsigned __int32 > resultMessages;
std::map <unsigned __int32, unsigned __int32 >::iterator iteratorResultMessages;
static boost::mutex mtxResultMessages;
static boost::mutex mtxCommonCountTime;
unsigned __int32 commonCountTime = 1;

void startWorkDataThread(const std::string &input)
{
	binary_reader::Reader reader(std::string(SOURCE_DIR) + "/input_" + input + ".txt");
	
	unsigned __int32 countCurrentTime = 0;
	unsigned __int32 currentTime = 0;
	unsigned __int32 countTime = 1;
	std::map <unsigned __int32, std::list <unsigned __int32> > countOfType;
	std::map <unsigned __int32, std::list <unsigned __int32> >::iterator it;

	if (reader.isOpenedFile())
	{
		MessageData messageData;
		bool isFirst = true;
		while (reader.readMessageData(&messageData))
		{
			if (isFirst)
			{
				currentTime = messageData.time;
				isFirst = false;
			}
			
			{
				boost::mutex::scoped_lock lock( mtxCommonCountTime );
				commonCountTime = commonCountTime > messageData.time ? commonCountTime : messageData.time;
			}

			if (messageData.time != currentTime)
			{
				countCurrentTime = 1;
				++countTime;
				currentTime = messageData.time;
			} else
			{
				++countCurrentTime;
			}

			unsigned __int32 commonLenghtMessage = messageData.len;

			if ((it = countOfType.find(messageData.type)) == countOfType.end())
			{
				if (commonLenghtMessage < 2048)
                            	{
					std::list <unsigned __int32> listLenght;
					listLenght.push_back(messageData.len);
					countOfType.insert(std::pair<unsigned __int32, std::list <unsigned __int32> >(messageData.type, listLenght));
                            	}
			}
			else
			{
				
				for (std::list <unsigned __int32>::reverse_iterator i = it->second.rbegin(); 
					i != it->second.rend() && countCurrentTime > 0;
					++i, --countCurrentTime)
				{
					commonLenghtMessage += *i;
				}

				if (commonLenghtMessage < 2048)
				{
					it->second.push_back(messageData.len);
				}
			}
		}

		for (it = countOfType.begin(); it != countOfType.end(); ++it)
		{
			boost::mutex::scoped_lock lock( mtxResultMessages );
			if ((iteratorResultMessages = resultMessages.find(it->first)) == resultMessages.end())
				resultMessages.insert(std::pair< unsigned __int32, unsigned __int32 >(it->first, it->second.size()));
			else
				iteratorResultMessages->second += it->second.size();
		}
	}
}

int main()
{
	static const size_t threadsCount = 1000;
	boost::thread_group groupSmallBuffer;
	for( size_t i = 1; i < threadsCount; ++i )
	{
		std::stringstream ss;
		ss << i;
		std::string number = ss.str();

		groupSmallBuffer.create_thread( boost::bind( startWorkDataThread, std::string("000" + number).substr(number.size(), 3) ) );
	}
	groupSmallBuffer.join_all();
	
	std::ofstream outputFile(SOURCE_DIR "/output.txt", std::ios_base::binary|std::ios_base::out|std::ios_base::trunc );
	
	for (iteratorResultMessages = resultMessages.begin(); iteratorResultMessages != resultMessages.end(); ++iteratorResultMessages)
	{
		unsigned __int32 type = iteratorResultMessages->first;
		outputFile.write(reinterpret_cast<char*>(&type), sizeof(unsigned __int32));
		double mean = (double)iteratorResultMessages->second / commonCountTime;
		outputFile.write(reinterpret_cast<char*>(&mean), sizeof(double));
		//outputFile << type << " " << mean << std::endl;
	}
	outputFile.close();
}

