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

#include "MessageReceiver.h"
#include <cstring>
#include <vector>
#include <thread>
#include <iostream>


MessageReceiver::MessageReceiver()
{
	memset(this->message, 0, this->kMessageSize);
}

int MessageReceiver::Connect(std::string ip, uint16_t port)
{
	if (!this->started.load())
	{
        this->connected = false;
        this->ip = ip;
        this->port = port;

		this->stopped.store(false);
		this->initialized.store(false);
		this->started.store(true);
		std::thread thrd(&MessageReceiver::ThreadHandler, this);
		thrd.detach();
		while (!this->initialized.load());
	}
	return 0;
}

int MessageReceiver::Disconnect()
{
	this->stopped.store(true);
	while (this->started.load());
	return 0;
}

int MessageReceiver::GetMessageID()
{
	this->mtx.lock();
    int32_t val = *reinterpret_cast<int32_t*>(this->message + this->kBytePos_MessageID);
	this->mtx.unlock();
	return val;
}

int MessageReceiver::GetADCValue()
{
	this->mtx.lock();
	int32_t val = *reinterpret_cast<int32_t*>(this->message + this->kBytePos_DoseADC);
	this->mtx.unlock();
	return val;
}
int MessageReceiver::GetADCAverageValue()
{
	this->mtx.lock();
	int32_t val = *reinterpret_cast<int32_t*>(this->message + this->kBytePos_DoseADCAverage);
	this->mtx.unlock();
	return val;
}
int16_t MessageReceiver::GetHVOut()
{
	this->mtx.lock();
    int16_t val = *reinterpret_cast<int16_t*>(this->message + this->kBytePos_HVOut);
	this->mtx.unlock();
	return val;
}

int8_t MessageReceiver::GetHVPolarity()
{
    this->mtx.lock();
    int8_t val = *reinterpret_cast<int8_t*>(this->message + this->kBytePos_HVPol);
    this->mtx.unlock();
    return val;
}

int8_t MessageReceiver::GetRange()
{
    this->mtx.lock();
    int8_t val = *reinterpret_cast<int8_t*>(this->message + this->kBytePos_Range);
    this->mtx.unlock();
    return val;
}

int MessageReceiver::GetPressurePa()
{
	this->mtx.lock();
	int32_t val = *reinterpret_cast<int32_t*>(this->message + this->kBytePos_Pressure);
	this->mtx.unlock();
	return val;
}

int MessageReceiver::GetMeasurementState()
{
	this->mtx.lock();
	uint8_t val = *reinterpret_cast<uint8_t*>(this->message + this->kBytePos_MeasurementState);
	this->mtx.unlock();
	return val;
}

int MessageReceiver::GetMeasurementTime()
{
	this->mtx.lock();
	uint16_t val = *reinterpret_cast<uint16_t*>(this->message + this->kBytePos_MeasurementTime);
	this->mtx.unlock();
	return val;
} 

double MessageReceiver::GetFrequency()
{
    this->mtx.lock();
    double freq = this->frequency;
    this->mtx.unlock();
    return freq;
}

bool MessageReceiver::IsConnected()
{
    this->mtx.lock();
    bool conn = this->connected;
    this->mtx.unlock();
    return conn;
}

int MessageReceiver::CreateSocket()
{
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    return sock;
}

int MessageReceiver::ConnectToSocket(int fd, const struct sockaddr* addr, int len)
{
    return connect(fd, reinterpret_cast<const sockaddr*>(addr), static_cast<socklen_t>(len));
}

void MessageReceiver::ThreadHandler()
{
#ifdef __linux__
    sockaddr_in hint;
    memset(&hint, 0, sizeof(hint));
    inet_pton(AF_INET, this->ip.c_str(), &hint.sin_addr);
    hint.sin_port = htons(this->port);
    hint.sin_family = AF_INET;

    int sock = this->CreateSocket();

    // create vector
    std::vector<char> message_vec;
    int bytes_received = 0;
    char buff[kBufferSize];
    memset(buff, 0, kBufferSize);

    this->initialized.store(true);

    auto start = std::chrono::system_clock::now(); // TIME MEASUREMENT
    while (!this->stopped.load())
    {
        auto end = std::chrono::system_clock::now(); // TIME MEASUREMENT
        ssize_t recv_size = recv(sock, buff, kBufferSize, 0);
        //std::cout << "recv_size = " << recv_size << std::endl;
        if (recv_size > 0)
        {
            bytes_received += recv_size;
            for (int i = 0; i < recv_size; ++i)
            {
                message_vec.push_back(buff[i]); // !!!
            }

            if (bytes_received == kMessageSize)
            {
                end = std::chrono::system_clock::now();
                std::chrono::duration<double> elapsed_seconds = end - start;
                //std::cout << "receiving period(ms) : " << elapsed_seconds.count() * 1000
                //	<< "\trecv: " << recv_size << std::endl;
                start = std::chrono::system_clock::now();

                this->mtx.lock();
                this->frequency = 1. / elapsed_seconds.count();
                uint64_t vec_size = message_vec.size();
                for (uint64_t i = 0; i < vec_size; ++i) // !!!
                {
                    this->message[i] = message_vec.at(i); // !!!
                }
                this->mtx.unlock();

                /* Clean buffer and vector */
                memset(buff, 0, kBufferSize);
                message_vec.clear(); // !!!
                bytes_received = 0;
            }
        }
        else
        {
            memset(this->message, 0, kMessageSize);
            shutdown(sock, SHUT_RDWR);
            close(sock);

            sock = this->CreateSocket();
            int conn = ConnectToSocket(sock, reinterpret_cast<const sockaddr*>(&hint), sizeof(hint));
            if (conn < 0)
            {
                // Conn error
            }

            this->mtx.lock();
            this->connected = (conn == 0);
            this->mtx.unlock();

            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }
    memset(this->message, 0, kMessageSize);
    shutdown(sock, SHUT_RDWR);
    close(sock);
    this->started.store(false);
#endif
}
