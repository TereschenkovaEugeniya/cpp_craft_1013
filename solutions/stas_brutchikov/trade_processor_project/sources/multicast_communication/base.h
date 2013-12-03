#pragma once


const char SOH = 0x01;
const char ETX = 0x03;
const char US = 0x1f;

const size_t transmission_block_max_size = 1000;
struct transmission_block
{
    char[transmission_block_max_size];
};


char message_category [1]
char message_type [1]
char message_network [1]
char retransmission_requester [2]
char header_identifier [1]
char reserved [2]
char message_sequence_number [9]
char participant_id [1]
char time_stamp [6]