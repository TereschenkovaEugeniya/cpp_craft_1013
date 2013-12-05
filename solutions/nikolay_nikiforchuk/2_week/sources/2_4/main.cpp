#include <iostream>
#include <fstream>
#include <string>
#include <stdint.h>
#include <vector>
#include <algorithm>
#include <market_message.h>

std::string in_file = BINARY_DIR "/2.4_example.in";
std::string out_file = BINARY_DIR "/2.4_example.out";
std::vector<binary_reader::market_message*> messages;

bool isAllowedType(binary_reader::market_message &message){
	return message.type() <= 4u;
}

struct comparator {
	bool operator() (binary_reader::market_message *i, binary_reader::market_message *j){
		return (i->time() < j->time());
	}
} message_comparator;

int main()
{	
	std::ifstream is(in_file, std::ifstream::binary);
	std::ofstream os(out_file, std::ofstream::binary);
	uint32_t max_time = 0;

	if (is){
		while (!is.eof()){
			binary_reader::market_message *message = new binary_reader::market_message(is);
			if (!is.eof() && isAllowedType(*message)){
				if ((message->time() + 2) <= max_time){
					continue;
				}
				if (message->time() > max_time){
					max_time = message->time();
				}
				messages.push_back(message);
			}
		}
		std::sort(messages.begin(), messages.end(), message_comparator);
		std::vector<binary_reader::market_message*>::iterator it = messages.begin();
		while (it != messages.end()){
			binary_reader::market_message* tmp_msg = *it;
			tmp_msg->write(os);
			//std::cout << "Write -> msg:" << tmp_msg->msg() << "::type:"
			//	<< tmp_msg->type() << "::time:" << tmp_msg->time() << std::ends;
			it++;
		}

		is.close();
		os.close();
	}
	else {
		std::cout << "No input file found " << in_file << std::endl;
	}
}

