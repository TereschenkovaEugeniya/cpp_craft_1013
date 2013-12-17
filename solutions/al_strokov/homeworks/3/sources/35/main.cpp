#include "binFileReader.h"
#include "binFileWriter.h"
#include <iostream>
#include <boost/thread/thread.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

typedef std::map<int, int> counting_map_t;

void processor(const std::string& ifname, const std::string& ofname) {
	binFileReader bfr(ifname);

	if (!bfr.fileOk){
		return;
	}

	binFileWriter bfw(ofname);
	if (!bfw.fileOk) {
		return;
	}

	uint32_t messageSize;
	uint32_t currentTime = 0;
	uint32_t secondsCount = 0;
	static const int buffSize = 2048;
	counting_map_t buffersSizes;
	counting_map_t messagesCount;
	dataStruct1_t ds1;

	//input
	while (bfr.readDataStruct(ds1)) {
		//new second
		if (currentTime != ds1.time) {
			currentTime = ds1.time;
			secondsCount++;
			buffersSizes.clear();
		}

		messageSize = sizeof ds1.type + sizeof ds1.time + sizeof ds1.len + (sizeof(char)) * ds1.len;

		if (!buffersSizes.count(ds1.type)) {
			buffersSizes[ds1.type] = buffSize - messageSize;
			if(buffersSizes[ds1.type] > 0){
				messagesCount[ds1.type] += 1;
			}
		} else {
			buffersSizes[ds1.type] = buffersSizes[ds1.type] - messageSize;
			if (buffersSizes[ds1.type] > 0) {
				messagesCount[ds1.type] += 1;
			}
		}
	}

	//output
	double avgMsg;
	for (counting_map_t::const_iterator it = messagesCount.begin();
			it != messagesCount.end(); ++it) {
		bfw.writeRawValue(it->first);
		avgMsg = (double) it->second / secondsCount;
		bfw.writeRawValue(avgMsg);
	}
}

int main(int argc, char** argv){
    std::cout << "*Homework 35*" << std::endl;

	std::string filenamei;
	std::string filenameo;

    boost::thread_group tg;
	static const size_t maxFileIndex = 999;
	std::stringstream sstr;

	for(size_t i = 0; i < maxFileIndex; i++){
		sstr << SOURCE_DIR "/sources/35/input_" << i << ".txt";
		if(boost::filesystem::exists(sstr.str())){
			filenamei = sstr.str();
			sstr.str("");
			sstr << SOURCE_DIR "/sources/35/output_" << i << ".txt";
			filenameo = sstr.str();
			std::cout << filenamei << "--->" << filenameo << std::endl;
			tg.create_thread( boost::bind(&processor, filenamei, filenameo) );
		}
		sstr.str("");
	}

    tg.join_all();

	std::cout << "...done" << std::endl;
	return 0;
}
