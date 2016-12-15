

#include "stdafx.h"
#include "_Client.h"



int BYTEstrlen(BYTE *str)
{
	int i = 0;
	while (str[i] != 0)
	{
		i++;
	}
	return i;
}

void BYTEstrcat(BYTE *str1, BYTE *str2)
{
	for (int i = BYTEstrlen(str1), k = 0; ((i <= (BYTEstrlen(str1) + BYTEstrlen(str2))) && (i<1024)); i++, k++)
		str1[i] = str2[k];
}
void BYTEstrcat(BYTE *str1, char *str2)
{
	for (int i = BYTEstrlen(str1), k = 0; ((i <= (BYTEstrlen(str1) + strlen(str2))) && (i<1024)); i++, k++)
		str1[i] = str2[k];
}

_client::_client() : _sock(service), _clientIter(_intIter)
{
	_intIter++;
	_isalive = 1;
	//std::cout << "iter: " << _clientIter << "1111111" << std::endl;
	_name = new BYTE(1024);
	data = new BYTE(1024);
	_key = new BYTE(32);
}

int &_client::clientIter() { return _clientIter; }
BYTE *_client::room() { return _room; }
BYTE *_client::username() { return _name; }
BYTE *_client::username(BYTE *_newname)
{

	for (int i = 0; i <= BYTEstrlen(_newname); i++)
	{
		_name[i] = _newname[i];
	}
	return _name;
}

BYTE *_client::key() { return _key; }

BYTE *_client::key(BYTE *_newkey)
{

	for (int i = 0; i < 32; i++)
	{
		_key[i] = _newkey[i];
	}
	return _key;
}

ip::tcp::socket & _client::sock() { return _sock; }

BYTE * _client::_prepData()
{
	for (int i = 0; i <= BYTEstrlen(_name); i++)
	{
		data[i] = _name[i];
	}
	BYTEstrcat(data, ": ");
	return data;
}
BYTE *_client::_data() { return data; }

_client::~_client()
{
	delete[] _name;
	delete[] data;
	delete[] _key;
	delete[] _room;
}
ByteBlock _client::Encrypt(BYTE *_buffer)
{
	ByteBlock _bytebstr1(_buffer, 1024);
	ByteBlock _bytebstr2;
	std::vector<ByteBlock> bytevect = split_blocks(_bytebstr1, 16);
	auto _iter = bytevect.begin();
	ByteBlock _keyb(_key, 32);
	Kuznyechik kuz(_keyb);
	for (_iter = bytevect.begin(); _iter < bytevect.end(); _iter++)
	{
		kuz.encrypt(*_iter, *_iter);
	}
	_bytebstr2 = join_blocks(bytevect);
	return  _bytebstr2;
}

ByteBlock _client::Decrypt(BYTE *_buffer)
{
	
	ByteBlock _bytebstr1(_buffer, 1024);
	ByteBlock _bytebstr2;
	std::vector<ByteBlock> bytevect = split_blocks(_bytebstr1, 16);
	std::vector<ByteBlock>::iterator _iter = bytevect.begin();
	ByteBlock _keyb(_key, 32);
	Kuznyechik kuz(_keyb);
	for (_iter = bytevect.begin(); _iter < bytevect.end(); _iter++)
	{
		kuz.decrypt(*_iter, *_iter);
	}
	_bytebstr2 = join_blocks(bytevect);
	return  _bytebstr2;
}

bool BYTEstrcmp(BYTE *str1, BYTE *str2)
{
	bool a=0;
	int i = 32;
	for (int k = 0; k < i; k++)
	{
		if (str1[k] == str2[k])
			a = 1;
		else
			return 0;
	}
	return a;
}

bool  _client::isalive(bool val)
{
	_isalive = val;
	return _isalive;
}

bool  _client::isalive() const {	return _isalive;}

void client_session(client_ptr client)
{
	BYTE _newdata[1024];
	memset(_newdata, 0, 1024);
	client->sock().read_some(buffer(_newdata));
	Sleep(100);
	if (_newdata[0] == '1')
	{
		memset(_newdata, 0, 1024);
		client->sock().read_some(buffer(_newdata, 1024));
		client->key(_newdata);
	}
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
				ByteBlock datablock = client->Decrypt(_newdata);
				//BYTEstrcat(client->_data(), _newdata);
				BYTEstrcat(client->_data(), datablock.byte_ptr());
				datablock = client->Encrypt(client->_data());
				for (array::iterator b = ++connections.begin(), e = connections.end(); b != e; ++b)
				{
					if (((*b)->isalive()) && (BYTEstrcmp((*b)->key(), client->key())))
					{
						size_t a = write((*b)->sock(), buffer(datablock.byte_ptr(), datablock.size()));
					}
				}
			}
			std::cout << client->_data() << std::endl;

			Sleep(100);
		}
	}
	catch (std::exception& e)
	{
		std::cout << "client disconnected" << std::endl;
		client->isalive(0);
		connections.erase(std::remove_if(connections.begin(), connections.end(), boost::bind(&_client::isalive, _1)), connections.end());
	}
}
