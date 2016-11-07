// Server_boost.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <boost\asio.hpp>
#include <iostream>
//#include <memory>
#include <boost\thread.hpp>
//#include <vector>

using namespace boost::asio;
//typedef boost::shared_ptr<ip::tcp::socket> socket_ptr;
//std::vector <ip::tcp::socket> connections;
//std::vector <ip::tcp::socket>::iterator iter;
ip::tcp::socket *connections;
ip::tcp::acceptor *acc;
int con_count;
void client_session(int _ID)
{
	while (true)
	{
		char data[4096];
		memset(data, 0, 4096);
		size_t len = connections[_ID].read_some(buffer(data));
		if (len > 0)
			for (int itID=0;itID<con_count;itID++)
			{
				write(connections[itID], buffer(data, strlen(data)));

			}
	}
}


int main()
{
	try
	{
		con_count = 0;
		io_service service;
		ip::tcp::endpoint ep(ip::tcp::v4(), 7770);
		ip::tcp::acceptor acc(service, ep);
		boost::asio::ip::tcp::acceptor acceptor(io_service);
		acc.open(ep.protocol());
		acc.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
		acc.bind(ep);
		acc.listen();
		for (;; con_count++)
		{
			std::cout << con_count;
			connections = new ip::tcp::socket(service);
			//acc.accept(connections[con_count]);
			boost::thread(boost::bind(client_session, con_count));
			Sleep(100);
		}
		
		
	}
	catch(std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
}


