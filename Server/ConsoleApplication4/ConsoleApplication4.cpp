#pragma comment(lib,"Ws2_32.lib") 


#include "stdafx.h" 
#include <WinSock2.h> 
#include <iostream> 
#include <WS2tcpip.h> 

SOCKET Connect;
SOCKET *Connections;
SOCKET Listen;
using namespace std;
int ClientCount = 0;

void SendMassageToClient(int ID)
{
	char *buffer = new char[4096];
	for (;; Sleep(100))
	{
		memset(buffer, 0, strlen(buffer));
		if (recv(Connections[ID], buffer, 4096, NULL))
		{
			cout << ID << ":" << buffer << endl;
			for (int i = 0; i < ClientCount; i++)
			{
				if (i != ID)
				{
					//send(Connections[i], , strlen(buffer), NULL)
					send(Connections[i], buffer, strlen(buffer), NULL);
				}
			}

		}
	}
	delete buffer;
}

int main()
{
	setlocale(LC_ALL, "russian");
	WSAData data;
	WORD version = MAKEWORD(2, 2);
	int res = WSAStartup(version, &data);
	if (res != 0)
		return 0;
	struct addrinfo hints;
	struct addrinfo *result;
	Connections = (SOCKET*)calloc(64, sizeof(SOCKET));
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	getaddrinfo(NULL, "7770", &hints, &result);
	Listen = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	bind(Listen, result->ai_addr, result->ai_addrlen);
	listen(Listen, SOMAXCONN);
	freeaddrinfo(result);
	printf("START SERVER...");
	char m_connect[] = "Connect...;;;5";
	for (;; Sleep(100))
	{
		if (Connect = accept(Listen, NULL, NULL))
		{
			printf("Client conect....\n");
			Connections[ClientCount] = Connect;
			send(Connections[ClientCount], m_connect, strlen(m_connect), NULL);
			ClientCount++;
			cout << ClientCount << endl;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)SendMassageToClient, (LPVOID)(ClientCount - 1), NULL, NULL);
		}

	}
	return 1;
}