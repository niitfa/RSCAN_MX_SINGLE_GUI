#ifndef MESSAGE_RECEIVER_H
#define MESSAGE_RECEIVER_H

#include <string>
#include <thread>
#include <atomic>
#include <mutex>

class MessageReceiver
{
	/* Message info */
	static const int kMessageSize = 23;
	static const int kBufferSize = 1;
	char message[kMessageSize];

	static const int kBytePos_MessageID = 0;
	static const int kBytePos_DoseADC = 4;
	static const int kBytePos_DoseADCAverage = 8;
	static const int kBytePos_HVOut = 12;
    static const int kBytePos_HVPol = 14;
    static const int kBytePos_Range = 15;
	static const int kBytePos_Pressure = 16;
	static const int kBytePos_MeasurementState = 20;
	static const int kBytePos_MeasurementTime = 21;


    /* Server var */
    std::string ip;
    uint16_t port;
    struct timeval timeout;
    bool connected = false;

    /* Thread var */
    std::mutex mtx;
    std::atomic<bool> started{ false };
    std::atomic<bool> initialized{ false };
    std::atomic<bool> stopped{ false };

    double frequency = 0;
public:
    MessageReceiver();
	~MessageReceiver() = default;
    int Connect(std::string ip, uint16_t port);
	int Disconnect();

	int GetMessageID();
	int GetADCValue();
	int GetADCAverageValue();
    int16_t GetHVOut();
    int8_t GetHVPolarity();
    int8_t GetRange();
	int GetPressurePa();
	int GetMeasurementState();
	int GetMeasurementTime();
    double GetFrequency();
    bool IsConnected();
private:
    int CreateSocket();
    int ConnectToSocket(int fd, const struct sockaddr* addr, int len);
private:
    void ThreadHandler();
};



#endif
