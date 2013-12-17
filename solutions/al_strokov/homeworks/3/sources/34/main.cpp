#include "binFileReader.h"
#include "binFileWriter.h"
#include <iostream>
#include <fstream>
#include <set>
#include <boost/thread/thread.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <strstream>

uint32_t tmp[] = { 1u, 2u, 3u, 4u };
static const std::set<uint32_t> acceptableOperations(&tmp[0], &tmp[0] + 4);

void processor(const std::string& ifname, const std::string& ofname) {
	binFileReader bfr(ifname);

	if (!bfr.fileOk){
		return;
	}

	binFileWriter bfw(ofname);
	if (!bfw.fileOk) {
		return;
	}

	uint32_t maxTime = 0;
	bool isRecordValid;
	dataStruct1_t ds1;

	while (bfr.readDataStruct(ds1)) {
		isRecordValid = true;

		if ((!acceptableOperations.count(ds1.type)) || (ds1.time + 2 <= maxTime)) {
			isRecordValid = false;
		}

		if (maxTime < ds1.time) {
			maxTime = ds1.time;
		}

		if (isRecordValid) {
			bfw.writeDataStruct(ds1);
		}
	}
}


int main(int argc, char** argv) {
	std::cout << "*Homework34*" << std::endl;

	std::string filenamei;
	std::string filenameo;


    boost::thread_group tg;
	static const size_t maxFileIndex = 999;
	std::stringstream sstr;

	for(size_t i = 0; i < maxFileIndex; i++){
		sstr << SOURCE_DIR "/sources/34/input_" << i << ".txt";
		if(boost::filesystem::exists(sstr.str())){
			filenamei = sstr.str();
			sstr.str("");
			sstr << SOURCE_DIR "/sources/34/output_" << i << ".txt";
			filenameo = sstr.str();
			std::cout << filenamei << "--->" << filenameo << std::endl;
			tg.create_thread( boost::bind(&processor, filenamei, filenameo) );
		}
		sstr.str("");
	}

    tg.join_all();

	std::cout << "---===over===---" << std::endl;
	return 0;
}
