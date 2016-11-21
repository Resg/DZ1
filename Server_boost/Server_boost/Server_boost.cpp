// Server_boost.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "_Client.h"




//typedef boost::shared_ptr<_client> client_ptr;
typedef boost::shared_ptr<_client> client_ptr;
typedef std::vector<client_ptr> array;
array connections;

ByteBlock Encrypt(BYTE *_buffer)
{
	ByteBlock _bytebstr1(_buffer, 4096);
	ByteBlock _bytebstr2;
	hex_representation(_bytebstr1);
	std::vector<ByteBlock> bytevect = split_blocks(_bytebstr1, 16);
	std::vector<ByteBlock>::iterator _iter = bytevect.begin();
	//ByteBlock _bytebstr2(_buffer, sizeof(_buffer));
	BYTE key[] = "8899aabbccddeeff0011223344556677fedcba98765432100123456789abcdef";
	ByteBlock _key(key, 32);
	Kuznyechik kuz(_key);
	for (_iter = bytevect.begin(); _iter < bytevect.end(); _iter++)
	{
		kuz.encrypt(*_iter, *_iter);
	}
	_bytebstr2 = join_blocks(bytevect);
	//_buffer = _bytebstr2.byte_ptr();
	return  _bytebstr2;
	//_buffer = _bytebstr1.byte_ptr();
}

ByteBlock Decrypt(BYTE *_buffer)
{
	ByteBlock _bytebstr1(_buffer, 4096);
	ByteBlock _bytebstr2;
	//std::string a=_buffer;
	//hex_to_bytes(_bytebstr1.byte_ptr())
	std::vector<ByteBlock> bytevect = split_blocks(_bytebstr1, 16);
	std::vector<ByteBlock>::iterator _iter = bytevect.begin();
	//ByteBlock _bytebstr2(_buffer, sizeof(_buffer));
	BYTE key[] = "8899aabbccddeeff0011223344556677fedcba98765432100123456789abcdef";
	ByteBlock _key(key, 32);
	Kuznyechik kuz(_key);
	for (_iter = bytevect.begin(); _iter < bytevect.end(); _iter++)
	{
		kuz.decrypt(*_iter, *_iter);
	}
	_bytebstr2 = join_blocks(bytevect);
	//_buffer = _bytebstr2.byte_ptr();
	return  _bytebstr2;
}

int con_count;

void client_session(client_ptr client)
{
	BYTE _newdata[4096];
	memset(_newdata, 0, 4096);
	client->sock().read_some(buffer(_newdata));
	client->username(_newdata);
	try
	{
		while (client->sock().is_open())
		{

			client->_prepData();
			memset(_newdata, 0, 4096);
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
					write((*b)->sock(), buffer(datablock.byte_ptr(), datablock.size()));
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


