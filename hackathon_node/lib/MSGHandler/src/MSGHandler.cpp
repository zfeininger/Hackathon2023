#include <MSGHandler.h>

std::string MSGHandler::getMessage(std::unordered_map<uint8_t, DataSource*> &d_l){
    std::string msg;
    char buf[32];
    for(auto dev_it = d_l.begin(); dev_it != d_l.end(); ++dev_it){
        if((*dev_it).second->update_needed == true){
            sprintf(buf, "%02x", (*dev_it).second->address);
            msg.append(buf);
            for(auto data_it = (*dev_it).second->data.begin(); data_it != (*dev_it).second->data.end(); ++data_it){
                sprintf(buf, "%02x%08x", (*data_it).first, (*data_it).second.data.i);
                msg.append(buf);
            }
            msg += char(END_BYTE);
            (*dev_it).second->update_needed = false;
        }
    }
    return msg;
}

std::queue<device_data> MSGHandler::parseMsg(std::string msg){ // IF THE SIZE OF NUM ADDRESS DIGITS CHANGES THIS FUNCTION NEEDS TO BE FIXED
    std::queue<device_data> message_data;
    uint8_t dev_address;
    sscanf(msg.c_str(), "%02x", &dev_address);
    msg.erase(0, 2);
    while(!msg.empty()){
        device_data dev;
        dev.device_address = dev_address;
        dev.op_code = normal;
        sscanf(msg.c_str(), "%02x%08x", &dev.data_address, &dev.data.i);
        msg.erase(0, NUM_ADDRESS_DIGITS + NUM_VALUE_DIGITS);
        message_data.push(dev);
    }
    return message_data;
}

std::queue<device_data> MSGHandler::parseQueue(std::queue<char> &message_fifo){
    std::queue<device_data> message_data_queue;
    if(!message_fifo.empty()){
        std::queue<char> msg_fifo_cpy(message_fifo);
        uint8_t last_end_index;
        uint8_t i;
        std::string msg;
        for(i = 0; !msg_fifo_cpy.empty(); i++){
            msg += msg_fifo_cpy.front();
            if(msg_fifo_cpy.front() == char(END_BYTE)){
                auto parsed_queue = parseMsg(msg);
                while(!parsed_queue.empty()) {
                    message_data_queue.push(parsed_queue.front());
                    parsed_queue.pop();
                }
                msg.clear();
                last_end_index = i;
            }
            msg_fifo_cpy.pop();
        }
        for(i = 0; i <= last_end_index; i++) message_fifo.pop();
    }
    return message_data_queue;
}