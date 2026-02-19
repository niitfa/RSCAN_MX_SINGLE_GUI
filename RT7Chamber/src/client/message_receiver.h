#ifndef MESSAGE_RECEIVER_H
#define MESSAGE_RECEIVER_H

#include <string>
#include <thread>
#include <atomic>
#include <mutex>

class MessageReceiver
{
	/* Message var */
    static const int kBufferSize = 1;
	char* message = nullptr;
	int messageSize = 1;

	/* Server var */
	std::string ip;
    uint16_t port;
    bool connected = 0;

	/* Thread var */
	std::mutex mtx;
	std::atomic<bool> started{ false };
	std::atomic<bool> initialized{ false };
	std::atomic<bool> stopped{ false };

    double frequency = 0;
public:
    MessageReceiver(int size);
    virtual ~MessageReceiver();

    int conn(std::string ip, uint16_t port);
	int disconn();
	
    double getFrequency();
    bool isConnected();
protected:
	void copyData(char* dest, int addr, int size);
private:
	void threadHandler();
};



#endif
