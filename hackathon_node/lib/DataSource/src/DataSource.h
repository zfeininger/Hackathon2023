#ifndef DATA_SOURCE_H
#define DATA_SOURCE_H
#include <cstdint>
#include <unordered_map>

#define MAX_NUM_DEVICES 0xff
#define MAX_NUM_DATA 0xff

union value {
    uint32_t i;
    float f;
};

struct data_struct {
    value data;
};

class DataSource {
public:
    DataSource(bool high_frequency);
    static uint8_t address_index;
    uint8_t address;
    bool update_needed = 0;
    bool h_f;
    unsigned long prevMillis;
    uint8_t getDeviceAddress();
    void addData(const char* identifier);
    value getData(const char* identifier);
    int8_t setData(const char* identifier, value set_data);
    int8_t setData(uint8_t address, value set_data);
    uint8_t data_index = 0;
    std::unordered_map<uint8_t, data_struct> data;
    std::unordered_map<const char* , uint8_t> identifier_address_map;
};

#endif