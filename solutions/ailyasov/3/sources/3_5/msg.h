#pragma once

#include <vector>
#include <set>
#include <map>
#include <string>
#include <iostream>

#include <boost/cstdint.hpp>

#include "reader.h"
#include "writer.h"
#include "trade_msg.h"

using namespace binaryio;

class MsgBufInfo;

typedef std::map<uint32_t const, MsgBufInfo> MsgTypeMap;

class MsgBufInfo {
    public:
        MsgBufInfo(uint32_t const time, size_t const size) : size_(size), msg_count_(1) 
        {
            time_set_.insert(time);
        }

        //average number of messages of certain type per second
        long double get_avg() const 
        {
            return static_cast<long double>(msg_count_) / time_set_.size();
        }

        size_t get_size() const { return size_; }
        bool add_type(TradeMsg const&);
        friend std::ostream& operator<<(std::ostream&, MsgBufInfo const&);
        void add(TradeMsg const& m);
    private:
        MsgBufInfo();
        void operator=(MsgBufInfo const&);
        std::set<uint32_t> time_set_; //set of seconds
        size_t size_; //total length of all messages
        uint32_t msg_count_; //number of messages
};

class MsgBuf {
    public:
        MsgBuf() {}
        static size_t const MAX_SIZE;
        static uint32_t const MAX_TYPE;
        MsgTypeMap const& get_type_map() const;
        std::vector<TradeMsg> const& get_messages() const;
        size_t get_size(uint32_t const type) const;
        bool is_full(uint32_t const type) const;
        void read(Reader&);
        void write_type_map(Writer&);
        static void write_data(std::vector<TradeMsg> messages, Writer& out); 
    private:
        MsgBuf(MsgBuf const&);
        void operator=(MsgBuf const&);
        void add(TradeMsg const& m);
        MsgTypeMap type_map;
        std::vector<TradeMsg> messages;
};
