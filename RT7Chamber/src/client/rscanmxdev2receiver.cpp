#include "rscanmxdev2receiver.h"
#include <iostream>

RSCANMXDev2Receiver::RSCANMXDev2Receiver() : MessageReceiver { kMessageSize }
{}

uint32_t RSCANMXDev2Receiver::getMessageID()
{
    uint32_t res = 0;
    int addr = this->kByteMessageID;
    MessageReceiver::copyData(reinterpret_cast<char*>(&res), addr, sizeof(res));
    return res;
}

uint16_t RSCANMXDev2Receiver::getDetectorValue(int line, int detector)
{
    uint16_t res = 0;
    int addr = this->kByteDetectorOffset + (line * this->kLineLength + detector) * this->kADCResolutionBytes;
    MessageReceiver::copyData(reinterpret_cast<char*>(&res), addr, this->kADCResolutionBytes);
    return res;
}
