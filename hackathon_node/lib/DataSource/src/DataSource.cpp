#include <DataSource.h>

uint8_t DataSource::address_index = 0;

DataSource::DataSource(bool high_frequency)
    :address(address_index++),
    h_f(high_frequency)
{
}

void DataSource::addData(const char* identifier){
    if(data_index < MAX_NUM_DATA){
        data_struct new_device;
        new_device.data.i = 0;
        data.insert({data_index, new_device});
        identifier_address_map.insert({identifier, data_index++});
    }
}

uint8_t DataSource::getDeviceAddress(){
    return address;
}

value DataSource::getData(const char* identifier){
    if(identifier_address_map.find(identifier) == identifier_address_map.end()){
        value val;
        return val;
    } else return data.at(identifier_address_map.at(identifier)).data;
}

int8_t DataSource::setData(const char* identifier, value set_data){
    if(identifier_address_map.find(identifier) == identifier_address_map.end())
        return 0;
    else{
        data.at(identifier_address_map.at(identifier)).data = set_data;
        return 1;
    }

}

int8_t DataSource::setData(uint8_t address, value set_data){
    if(data.find(address) == data.end()){
        return 0;
    } else {
        data.at(address).data = set_data;
        return 1;
    } 
}