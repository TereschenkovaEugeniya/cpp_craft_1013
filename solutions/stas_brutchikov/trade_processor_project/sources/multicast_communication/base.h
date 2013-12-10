#pragma once
namespace multicast_communication
{

const char SOH = 0x01;
const char ETX = 0x03;
const char US = 0x1f;

typedef std::pair < std::string, unsigned short > address_port;
typedef std::vector < address_port > vector_adres_port;

template<typename T>
bool parse(const string& data, T& obj);

} //namespace multicast_communication
