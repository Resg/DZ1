// Server_boost.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <boost\asio.hpp>
#include <iostream>
using boost::asio::ip::tcp;


int main()
{
	try
	{
		boost::asio::io_service io_service;
		tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 7770));
		for (;;)
		{
			tcp::socket socket(io_service);
			acceptor.accept(socket);
		}
	}
	catch(std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
}

