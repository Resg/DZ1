// Server_boost.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "_Client.h"




//typedef boost::shared_ptr<_client> client_ptr;
typedef boost::shared_ptr<_client> client_ptr;
typedef std::vector<client_ptr> array;
array connections;



int con_count;

void client_session(client_ptr client)
{
	BYTE _newdata[496];
	memset(_newdata, 0, 496);
	client->sock().read_some(buffer(_newdata));
	std::cout << _newdata << std::endl;
	Sleep(100);
	if (_newdata[0] == '1')
	{
		memset(_newdata, 0, 496);
		client->sock().read_some(buffer(_newdata, 496));
		std::cout << _newdata << std::endl;
		client->key(_newdata);
		std::cout << client->key() << std::endl;
	}
	memset(_newdata, 0, 496);
	client->sock().read_some(buffer(_newdata));
	client->username(_newdata);
	try
	{
		while (client->sock().is_open())
		{
			client->_prepData();
			memset(_newdata, 0, 496);
			size_t len = client->sock().read_some(buffer(_newdata));
			if (len > 0)
			{
				ByteBlock datablock = client->Decrypt(_newdata);
				//BYTEstrcat(client->_data(), _newdata);
				BYTEstrcat(client->_data(), datablock.byte_ptr());
				datablock = client->Encrypt(client->_data());
				for (array::iterator b = ++connections.begin(), e = connections.end(); b != e; ++b)
				{
					if (((*b)->isalive)&&(BYTEstrcmp((*b)->key(), client->key())))
					{
						size_t a = write((*b)->sock(), buffer(datablock.byte_ptr(), datablock.size()));
						std::cout << a << std::endl;
					}
				}
			}
			std::cout << client->_data() << std::endl;

			Sleep(100);
		}
	}
	catch (std::exception& e)
	{
		
		std::cerr << e.what() << std::endl;
		std::cout << "client disconnected" << std::endl;
		client->isalive = 0;
		/*
		for (b = connections.begin(); b != connections.end(); ++b)
		{

			if (client->clientIter() == (*b)->clientIter())
			{
				client.reset();
				b = connections.erase(b);
				std::cout << "11111111111";
				break;
			}
		}
		std::cout << "11111111111";*/
	}
}


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


