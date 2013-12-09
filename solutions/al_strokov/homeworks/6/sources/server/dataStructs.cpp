#include "dataStructs.h"

#include <iostream>
#include <string>
#include <sstream>

std::string renderArr(const char * arr, size_t size){
	std::stringstream ss;
	for( size_t i = 0; i < size; i++){
		ss << std::hex << (uint)arr[i] << " ";
	}
	return ss.str();

}

void tqHeader_t::render() {
	std::cout << "quote header renderer" << std::endl;

	std::cout << "messageCategory;          :" << messageCategory << std::endl;
	std::cout << "messageType;              :" << messageType << std::endl;
	std::cout << "messageNetwork;           :" << messageNetwork << std::endl;
	std::cout << "retransmissionRequest[2]; :" << renderArr(retransmissionRequest, 2) << std::endl;
	std::cout << "headerIdentifier;         :" << headerIdentifier << std::endl;
	std::cout << "reserved;                 :" << renderArr(reserved, 2) << std::endl;
	std::cout << "messageSequenceNumber[9]; :" << renderArr(messageSequenceNumber, 9) << std::endl;
	std::cout << "participantID;            :" << participantID << std::endl;
	std::cout << "timeStamp[6];             :" << renderArr(timeStamp, 6) << std::endl;
}

void shortQuote_t::render() {
	std::cout << "securitySymbol[3];              :" << securitySymbol << std::endl;
	std::cout << "quoteCondition;                 :" << quoteCondition << std::endl;
	std::cout << "limitUpLimitDownIndicator;      :" << limitUpLimitDownIndicator << std::endl;
	std::cout << "reserved1;                      :" << reserved1 << std::endl;
	std::cout << "bidPriceDenominatorIndicator;   :" << bidPriceDenominatorIndicator << std::endl;
	std::cout << "bidShortPrice[8];               :" << renderArr(bidShortPrice, 8) << std::endl;
	std::cout << "bidSizeUnitsOfTrade[3];         :" << renderArr(bidSizeUnitsOfTrade, 3) << std::endl;
	std::cout << "reserved2;                      :" << reserved2 << std::endl;
	std::cout << "offerPriceDenominatorIndicator; :" << offerPriceDenominatorIndicator << std::endl;
	std::cout << "offerShortPrice[8];             :" << renderArr(offerShortPrice, 8) << std::endl;
	std::cout << "offerSizeInUnitsOfTrade[3];     :" << renderArr(offerSizeInUnitsOfTrade, 3) << std::endl;
	std::cout << "reserved3;                      :" << reserved3 << std::endl;
	std::cout << "nationalBBOIndicator;           :" << nationalBBOIndicator << std::endl;
	std::cout << "fINRABBOIndicator;              :" << fINRABBOIndicator << std::endl;


}
