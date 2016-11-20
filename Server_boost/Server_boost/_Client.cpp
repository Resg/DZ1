
#include "stdafx.h"
#include "client.h"


_client::_client() : _sock(service)
{
	_name = new char[4096];

	data = new char[4096];
}
char *_client::username() { return _name; }
char *_client::username(char *_newname)
{

	for (int i = 0; i <= strlen(_newname); i++)
	{
		_name[i] = _newname[i];
	}
	return _name;
}

ip::tcp::socket & _client::sock() { return _sock; }

char * _client::_prepData()
{
	for (int i = 0; i <= strlen(_name); i++)
	{
		data[i] = _name[i];
	}
	strcat(data, ": ");
	return data;
}
char *_client::_data() { return data; }

_client::~_client()
{
	delete _name;
	delete data;
}

