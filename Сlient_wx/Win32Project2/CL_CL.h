#pragma once
#include "Header.h"
#include <WinSock2.h> 
#include <iostream> 
#include <WS2tcpip.h>
#include <thread>
#include <memory>

class Client
{
private:
	SOCKET server;
	SOCKET client;
	bool _end = 0;
	char *name;
	struct sockaddr_in hints;
	char* buffer;
public:
	Client()
	{
		setlocale(LC_ALL, "russian");
		WSAData data;
		WORD version = MAKEWORD(2, 2);
		int res = WSAStartup(version, &data);
		/*if (res != 0)
			return 0;*/
		name = new char[16];
		//std::cin >> name;
		//ZeroMemory(&hints, sizeof(hints));
		hints.sin_family = AF_INET;
		hints.sin_addr.S_un.S_addr = inet_addr("192.168.14.131");
		hints.sin_port = htons(7770);
		client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		
		/*std::thread t1(GetMFServer);
		std::thread t2(SendMToServer);
		t1.join();
		t2.join();*/
	}
	void connectToServer(char *_name);
	void SendMToServer(char* _text);
	void GetMFServer();

	~Client()
	{
		delete name;
	}
};