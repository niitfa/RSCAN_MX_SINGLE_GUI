#ifndef STEPPER_CONTROLLER_TRANSMITTER_H
#define STEPPER_CONTROLLER_TRANSMITTER_H

#include <stdint.h>
#include <string>
#include <thread>
#include <atomic>
#include <mutex>

#include <QTcpSocket>

// добавить объект в окно chamberwindow
// обработать коннект из окна mainwindow


class MessageTransmitter
{
    static const int64_t kMessageSize = 8;

    static const int64_t kBytePositionValue1 = 0;
    static const int64_t kBytePositionValue2 = 4;

	std::string ip;
    uint16_t port;

    //int sock;
    QTcpSocket* qSocket = new QTcpSocket();
	char message[kMessageSize];
    bool connected = false;

	std::mutex mtx;
public:
    MessageTransmitter();
    ~MessageTransmitter();

    bool Connect(std::string ip, uint16_t port);
    void Disconnect();
    bool IsConnected();

    int64_t startMeasurement(int32_t cycles);
    int64_t resetMeasurement();
    int64_t setNegativeVoltage();
    int64_t setPositiveVoltage();
    int64_t setVoltageValue(uint16_t volt);
    int64_t setNarrowRange();
    int64_t setBroadRange();
    bool ping();
private:
    int64_t Send(int val_1, int val_2);
};

#endif

