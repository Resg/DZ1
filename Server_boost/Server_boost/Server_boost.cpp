// Server_boost.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "_Client.h"




//typedef boost::shared_ptr<_client> client_ptr;

int con_count;

int main()
{
	try
	{
		con_count = 0;
		int _serv=10000;
		while ((_serv > 9999) || (_serv < 1000))
		{
			std::cout << "Enter service" << std::endl;
			std::cin >> _serv;
			
			if ((_serv > 9999) || (_serv < 1000))
			{
				std::cout << "wrong service" << std::endl;
			}
		}
		ip::tcp::endpoint ep(ip::tcp::v4(), _serv);
		ip::tcp::acceptor acc(service, ep);
		client_ptr client0(new _client);
		connections.push_back(client0);
		std::cout << "Server start" << std::endl;
		for (;; con_count++)
		{
			client_ptr client(new _client);
			acc.accept(client->sock());
			boost::thread(boost::bind(client_session, client));
			std::cout << "client connected" << std::endl;
			connections.push_back(client);
			Sleep(100);
		}
		
		
	}
	catch(std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	system("pause");
	return 0;
}


