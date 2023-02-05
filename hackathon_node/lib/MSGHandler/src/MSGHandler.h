#ifndef MSGHANDLER_H
#define MSGHANDLER_H

#include <queue>
#include <string>
#include <DataSource.h>

#define NUM_ADDRESS_DIGITS 2
#define NUM_VALUE_DIGITS 8
#define END_BYTE 4
#define HEX 16

enum OP_CODES{
    normal,
    halt
};

struct device_data{
    uint8_t op_code;
    uint8_t device_address;
    uint8_t data_address;
    value data;
};

class MSGHandler{
public:
    static std::string getMessage(std::unordered_map<uint8_t, DataSource*> &d_l);
    static std::queue<device_data> parseQueue(std::queue<char> &message_fifo);
private:
    static std::queue<device_data> parseMsg(std::string msg);
};
#endif