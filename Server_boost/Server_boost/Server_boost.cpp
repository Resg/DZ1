// Server_boost.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "client.h"



//typedef boost::shared_ptr<_client> client_ptr;
typedef boost::shared_ptr<_client> client_ptr;
typedef std::vector<client_ptr> array;
array connections;
boost::recursive_mutex cs;
//ip::tcp::acceptor *acc;
int con_count;

void client_session(client_ptr client)
{
	char _newdata[4096];
	memset(_newdata, 0, 4096);
	client->sock().read_some(buffer(_newdata));
	client->username(_newdata);
	while (true)
	{
		client->_prepData();
		memset(_newdata, 0, 4096);
		size_t len = client->sock().receive(buffer(_newdata));
		if (len > 0)
		{
			strcat(client->_data(), _newdata);
			for (array::iterator b = connections.begin(), e = connections.end(); b != e; ++b)
			{
				//write((*b)->sock(), buffer(client->username(), client->username().size));
				write((*b)->sock(), buffer(client->_data(), strlen(client->_data())));
			}
		}
		std::cout << client->_data() << std::endl;
		
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
		//connect.bind;
		//boost::asio::ip::tcp::acceptor acceptor(io_service);
		for (;; con_count++)
		{
			std::cout << con_count;
			client_ptr client(new _client);
			acc.accept(client->sock());
			//boost::recursive_mutex::scoped_lock lk(cs);
			boost::thread(boost::bind(client_session, client));
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


