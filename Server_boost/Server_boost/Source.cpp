

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

_client::_client() : _sock(service)
{
	_name = new BYTE(1024);
	data = new BYTE(1024);
}
BYTE *_client::username() { return _name; }
BYTE *_client::username(BYTE *_newname)
{

	for (int i = 0; i <= BYTEstrlen(_newname); i++)
	{
		_name[i] = _newname[i];
	}
	return _name;
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
	delete _name;
	delete data;
}
ByteBlock Encrypt(BYTE *_buffer)
{
	ByteBlock _bytebstr1(_buffer, 1024);
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
	ByteBlock _bytebstr1(_buffer, 1024);
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
