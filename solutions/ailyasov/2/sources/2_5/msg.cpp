#include <iostream>
#include <vector>
#include <cctype>
#include <iterator>
#include <cstddef>

#include "msg.h"
#include "reader.h"
#include "writer.h"

using binary_reader::Reader;
using binary_writer::Writer;

const size_t MsgBuf::MAX_SIZE = 2048; 
const Type MsgBuf::MAX_TYPE = 100000;

std::ostream& operator<<(std::ostream& os, const Msg& msg) {
    os << "[" << msg.type_ << "," << msg.time_ << "," << msg.len_ << "," << msg.msg_ << "]";
    return os;
} 

void MsgBuf::read(Reader& in) {
    while(!in.eof()) {
        Type type = in.get_binary<uint32_t>();
        uint32_t time = in.get_binary<uint32_t>();
        uint32_t len = in.get_binary<uint32_t>();
        if(!in.eof()) {
            std::string str_msg = in.get_string(len);
            if(len == str_msg.size()) {
                const Msg msg(type, time, len, str_msg);
                add(msg);
            }
        }
    }
} 

void MsgBuf::write_type_map(Writer& out) {
    for(MsgTypeMap::const_iterator it = type_map.begin(); it != type_map.end(); ++it) {
        out.save_binary((*it).first);
        out.save_binary((*it).second.get_avg());
    }
}

void MsgBuf::write_data(const std::vector<Msg>& messages, Writer& out) {
    for(std::vector<Msg>::const_iterator it = messages.begin(); it != messages.end(); ++it) {
        out.save_binary((*it).get_type());
        out.save_binary((*it).get_time());
        out.save_binary((*it).get_len());
        out.save_string((*it).get_msg());
    }
}

std::ostream& operator<<(std::ostream& os, const MsgBufInfo& s) {
    os << s.get_avg();
    return os;
}

void MsgBuf::add(const Msg& m) {
    size_t sz = m.get_size() + this->get_size(m.get_type());
    bool msg_fit = sz < MAX_SIZE;
    if(msg_fit && m.get_type() < MAX_TYPE) {
        MsgTypeMap::iterator it = type_map.find(m.get_type());
        if(it == type_map.end()) {
            type_map.insert(std::make_pair(m.get_type(), MsgBufInfo(m.get_time(), m.get_size())));
        } else {
            (*it).second.add(m);
        }
        messages.push_back(m);
    }
}

bool MsgBuf::is_full(const uint32_t type) const {
    return this->get_size(type) < MsgBuf::MAX_SIZE;
}

void MsgBufInfo::add(const Msg& m) {
    time_set_.insert(m.get_time());
    ++msg_count_;
    size_ += m.get_size();
}

const std::vector<Msg>& MsgBuf::get_messages() const {
    return messages;
}

const MsgTypeMap& MsgBuf::get_type_map() const {
    return type_map;
} 

size_t MsgBuf::get_size(Type type) const {
    MsgTypeMap::const_iterator it = type_map.find(type);
    if(it == type_map.end()) return 0;
    return (*it).second.get_size();
}
