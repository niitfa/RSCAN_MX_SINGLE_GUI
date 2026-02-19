#ifdef _WIN32
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment (lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <WS2tcpip.h>
#endif

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

#include "message_receiver.h"
#include <cstring>
#include <vector>
#include <thread>
#include <iostream>

MessageReceiver::MessageReceiver(int size)
{
	this->messageSize = std::max(1, size);
	this->message = new char[this->messageSize];
    memset(this->message, 0, this->messageSize);
}

MessageReceiver::~MessageReceiver()
{
    delete[] this->message;
}

int MessageReceiver::conn(std::string ip, uint16_t port)
{
	if (!this->started.load())
	{
        this->connected = false;
        this->ip = ip;
        this->port = port;

		this->stopped.store(false);
		this->initialized.store(false);
		this->started.store(true);
		std::thread thrd(&MessageReceiver::threadHandler, this);
		thrd.detach();
		while (!this->initialized.load());
	}
	return 0;
}

int MessageReceiver::disconn()
{
	this->stopped.store(true);
	while (this->started.load());
	return 0;
}

void MessageReceiver::copyData(char* dest, int addr, int size)
{
	if( size > 0 && addr >= 0 && (addr + size) < this->messageSize )
	{	
		memcpy(dest, this->message + addr, size);
	}
}

double MessageReceiver::getFrequency()
{
    this->mtx.lock();
    double freq = this->frequency;
    this->mtx.unlock();
    return freq;
}

bool MessageReceiver::isConnected()
{
    this->mtx.lock();
    bool conn = this->connected;
    this->mtx.unlock();
    return conn;
}

void MessageReceiver::threadHandler()
{
#ifdef __linux__
	sockaddr_in hint;
	memset(&hint, 0, sizeof(hint));
	inet_pton(AF_INET, this->ip.c_str(), &hint.sin_addr);
	hint.sin_port = htons(this->port);
	hint.sin_family = AF_INET;

    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	std::vector<char> message_vec;
	int bytes_received = 0;
	char buff[kBufferSize];
	memset(buff, 0, kBufferSize);

	this->initialized.store(true);

	auto start = std::chrono::system_clock::now();
	while (!this->stopped.load())
	{
		auto end = std::chrono::system_clock::now();
        ssize_t recv_size = recv(sock, buff, kBufferSize, 0);
        if (recv_size > 0)
		{
			bytes_received += recv_size;
			for (int i = 0; i < recv_size; ++i)
			{
				message_vec.push_back(buff[i]);
			}

			if (bytes_received == this->messageSize)
			{
				end = std::chrono::system_clock::now();
                std::chrono::duration<double> elapsed_seconds = end - start;
                //std::cout << "receiving period(ms) : " << elapsed_seconds.count() * 1000
                //    << "\trecv: " << recv_size << std::endl;
				start = std::chrono::system_clock::now();

				this->mtx.lock();
                this->frequency = 1. / elapsed_seconds.count();
                uint64_t vec_size = message_vec.size();
                for (uint64_t i = 0; i < vec_size; ++i)
				{
					this->message[i] = message_vec.at(i);
				}

                //int id = *reinterpret_cast<int32_t*>(message + 0);
                //std::cout << "id: " << id << std::endl;

				this->mtx.unlock();

				memset(buff, 0, kBufferSize);
				message_vec.clear();
				bytes_received = 0;
			}
		}
		else
		{
			memset(this->message, 0, this->messageSize);
			shutdown(sock, SHUT_RDWR);
			close(sock);
            sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            
			int conn = connect(sock, reinterpret_cast<const sockaddr*>(&hint), sizeof(hint));
            if (conn < 0)
            {
			}

            this->mtx.lock();
            this->connected = (conn == 0);
            this->mtx.unlock();

            std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
	}
	memset(this->message, 0, this->messageSize);
	shutdown(sock, SHUT_RDWR);
	close(sock);
	this->started.store(false);
#endif
}
