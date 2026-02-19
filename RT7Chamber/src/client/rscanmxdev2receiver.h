#ifndef RSCANMXDEV2RECEIVER_H
#define RSCANMXDEV2RECEIVER_H

#include "message_receiver.h"

/*
to do:
2) fix matrix size = 4 bug
*/

class RSCANMXDev2Receiver : public MessageReceiver
{
public:
    static const int kMessageSize = 3456;
    static const int kByteMessageID = 0;
    static const int kByteDetectorOffset = 64;
    static const int kLineLength = 41;
    static const int kNumberOfLines = 41;   
    static const int kADCResolutionBytes = 2;
public:
    RSCANMXDev2Receiver();
    uint32_t getMessageID();
    uint16_t getDetectorValue(int line, int detector);
};

#endif // RSCANMXDEV2RECEIVER_H
