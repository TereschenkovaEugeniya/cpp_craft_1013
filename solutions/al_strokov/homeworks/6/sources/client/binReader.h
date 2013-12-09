#ifndef BINREADER_INCLUDED
#define BINREADER_INCLUDED

#include <iostream>
#include <string>
#include <fstream>
#include <stdint.h>
#include "dataStructs.h"

class binReader{
private:
    char* binBuffer_;
    size_t buffSize_;
    size_t readPos_;

    binReader(const binReader& that);
    binReader operator=(const binReader& that);

public:
    binReader(char* buffer, const size_t bufferSize);
    ~binReader();

    bool fileOk;

    template<class T>
    bool readRawValue(T& value);
    bool readRawValue(std::string& value, const size_t length);

    bool readMessageHeader(tqHeader_t& quoteHeader);
    bool readShortQuote(shortQuote_t& shortQuote);
    bool readLongQuote(longQuote_t& longQuote);
    bool readShortTrade(shortTrade_t& shortTrade);
    bool readLongTrade(longTrade_t& longTrade);
};

template<class T>
bool binReader::readRawValue(T& value){
   if (readPos_ + (sizeof value) > buffSize_){
	   fileOk = false;
       return false;
   } else {
	   for(size_t i = 0; i < sizeof value; i++ ){
		   ((char*)&value)[i] = binBuffer_[readPos_++];
	   }
       return true;
   }
}

#endif
