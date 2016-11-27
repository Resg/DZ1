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
	BYTE _newdata[1024];
	memset(_newdata, 0, 1024);
	client->sock().read_some(buffer(_newdata));
	client->username(_newdata);
	try
	{
		while (client->sock().is_open())
		{

			client->_prepData();
			memset(_newdata, 0, 1024);
			size_t len = client->sock().read_some(buffer(_newdata));
			if (len > 0)
			{
				//std::cout << len << std::endl;
				ByteBlock datablock = Decrypt(_newdata);
				//std::cout << datablock.byte_ptr();
				BYTEstrcat(client->_data(), datablock.byte_ptr());
				datablock = Encrypt(client->_data());
				for (array::iterator b = connections.begin(), e = connections.end(); b != e; ++b)
				{
					//write((*b)->sock(), buffer(client->username(), client->username().size));
					//write((*b)->sock(), buffer(datablock.byte_ptr(), datablock.size()));
					(*b)->sock().write_some(buffer(datablock.byte_ptr(), datablock.size()));
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
		array::iterator b = find(connections.begin(), connections.end(), client);
		connections.erase(b);
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


