#ifndef _DEAL_DATA_H
#define _DEAL_DATA_H

#include <stdint.h>
#include <iostream>

using namespace std;

struct MessageHeader{
    uint32_t type;
    uint32_t time;
    uint32_t len;

    string as_string();
};

const static uint32_t MARKET_OPEN = 1u;
const static uint32_t TRADE = 2u;
const static uint32_t QUOTE = 3u;
const static uint32_t MARKET_CLOSE = 4u;

const static unsigned short int M_HEADER_SIZEOF = sizeof(uint32_t) * 3;

istream& operator>> (istream& is, MessageHeader &m);
ostream& operator<< (ostream& os, const MessageHeader &m);

bool is_outdated_deal(MessageHeader &item);

void dump_to_stream(istream& is, ostream& os);

#endif // _DEAL_DATA_H