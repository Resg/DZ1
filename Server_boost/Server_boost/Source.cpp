

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
	for (int i = BYTEstrlen(str1), k = 0; ((i <= (BYTEstrlen(str1) + BYTEstrlen(str2))) && (i<496)); i++, k++)
		str1[i] = str2[k];
}
void BYTEstrcat(BYTE *str1, char *str2)
{
	for (int i = BYTEstrlen(str1), k = 0; ((i <= (BYTEstrlen(str1) + strlen(str2))) && (i<496)); i++, k++)
		str1[i] = str2[k];
}

_client::_client() : _sock(service), _clientIter(_intIter)
{
	_intIter++;
	isalive = 1;
	//std::cout << "iter: " << _clientIter << "1111111" << std::endl;
	_name = new BYTE(496);
	data = new BYTE(496);
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
	ByteBlock _bytebstr1(_buffer, 496);
	ByteBlock _bytebstr2;
	std::vector<ByteBlock> bytevect = split_blocks(_bytebstr1, 16);
	std::vector<ByteBlock>::iterator _iter = bytevect.begin();
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
	
	ByteBlock _bytebstr1(_buffer, 496);
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
	std::cout << _bytebstr2.byte_ptr() << std::endl;
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