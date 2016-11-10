// Server_boost.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "Header.h"



//typedef boost::shared_ptr<_client> client_ptr;
std::vector <boost::shared_ptr<_client>> connections;
std::vector <boost::shared_ptr<_client>>::iterator iter;

//ip::tcp::acceptor *acc;
int con_count;
void client_session(boost::shared_ptr<_client> client)
{
	while (true)
	{
		char data[4096];
		memset(data, 0, 4096);
		size_t len = client->sock().read_some(buffer(data));
		if (len > 0)
			for (iter = connections.begin(); iter < connections.end(); iter++)
			{
				write((*iter)->sock(), buffer(data, strlen(data)));
			}
		Sleep(100);
	}
}


int main()
{
	try
	{
		con_count = 0;
		ip::tcp::endpoint ep(ip::tcp::v4(), 7770);
		ip::tcp::acceptor acc(service, ep);
		size_t len;
		//connect.bind;
		//boost::asio::ip::tcp::acceptor acceptor(io_service);
		for (;; con_count++)
		{
			std::cout << con_count;
			boost::shared_ptr<_client> client(new _client);
			acc.accept(client->sock());
			char data[4096];
			memset(data, 0, 4096);
			len= client->sock().read_some(buffer(data));
			if (len > 0)
				std::string _d = data;
			boost::thread(boost::bind(client_session, &client));
			connections.push_back(client);
			Sleep(100);
		}
		
		
	}
	catch(std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
}


