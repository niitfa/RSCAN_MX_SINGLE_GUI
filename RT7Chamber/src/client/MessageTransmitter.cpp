#include "MessageTransmitter.h"
#include <QHostAddress>

#ifdef __linux__
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <netinet/in.h>
#include <fcntl.h>
#endif

#include <thread>
#include <iostream>

typedef enum
{
    COMM_EMPTY				= 0x00,
    COMM_SET_HV				= 0x01,
    COMM_START_MEAS			= 0x02,
    COMM_RESET_MEAS			= 0x03,
    COMM_SET_MEAS_RANGE 	= 0x04,
    COMM_SWITCH_HV		 	= 0x05,
    COMM_KEEP_ALIVE			= 0x20
} command_code_t;

MessageTransmitter::MessageTransmitter()
{
	memset(this->message, 0, kMessageSize);
}

MessageTransmitter::~MessageTransmitter()
{
    this->Disconnect();
    delete this->qSocket;
}


bool MessageTransmitter::Connect(std::string ip, uint16_t port)
{
    this->Disconnect();

    this->ip = ip;
    this->port = port;

    QHostAddress addr( QString::fromStdString(this->ip) );
    addr.toIPv4Address();

    /* QTcpSocket */
    bool opened = this->qSocket->open(QIODevice::ReadWrite);
    std::cout << "MessageTransmitter::opened = " << opened << std::endl;
    this->qSocket->connectToHost(addr, this->port, QIODevice::ReadWrite);
    this->connected = this->qSocket->waitForConnected(1000);
    std::cout << "MessageTransmitter::connected = " << connected << std::endl;
    std::string err =( this->qSocket->errorString() ).toStdString();
    std::cout << "MessageTransmitter::error = " << err << std::endl;
    return connected;
}

void MessageTransmitter::Disconnect()
{
    if(this->qSocket)
    {
        /* QTcpSocket */
        this->qSocket->disconnectFromHost();
        this->qSocket->close();
    }
}

bool MessageTransmitter::IsConnected()
{
    return this->connected;
}

int64_t MessageTransmitter::startMeasurement(int32_t cycles)
{
    return this->Send(COMM_START_MEAS, cycles);
}

int64_t MessageTransmitter::resetMeasurement()
{
    return this->Send(COMM_RESET_MEAS, 0);
}

int64_t MessageTransmitter::setNegativeVoltage()
{
    return this->Send(COMM_SWITCH_HV, 1);
}

int64_t MessageTransmitter::setPositiveVoltage()
{
    return this->Send(COMM_SWITCH_HV, 0);
}

int64_t MessageTransmitter::setVoltageValue(uint16_t volt)
{
    return this->Send(COMM_SET_HV, volt);
}

int64_t MessageTransmitter::setNarrowRange()
{
    return this->Send(COMM_SET_MEAS_RANGE, 1);
}

int64_t MessageTransmitter::setBroadRange()
{
    return this->Send(COMM_SET_MEAS_RANGE, 0);
}

bool MessageTransmitter::ping()
{
    return (this->Send(COMM_EMPTY, 0) == this->kMessageSize);
}


int64_t MessageTransmitter::Send(int val_1, int val_2)
{
    int64_t send_size = 0;
    if(this->qSocket)
    {
        this->mtx.lock();
        memset(this->message, 0, this->kMessageSize);
        memcpy(this->message + this->kBytePositionValue1, &val_1, sizeof(val_1));
        memcpy(this->message + this->kBytePositionValue2, &val_2, sizeof(val_2));

        /* QTcpSocket*/
        send_size = this->qSocket->write(this->message, this->kMessageSize);

        this->mtx.unlock();
    }
	return send_size;
}
