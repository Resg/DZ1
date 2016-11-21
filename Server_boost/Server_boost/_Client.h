#pragma once
#include "stdafx.h"
#include <boost\asio.hpp>
#include <iostream>
#include <memory>
#include <boost\thread.hpp>
#include <vector>
#include <string>
#include "mycrypto.hpp"
#include "Kuznyechik.hpp"
using namespace boost::asio;

int BYTEstrlen(BYTE *str);
void BYTEstrcat(BYTE *str1, BYTE *str2);
void BYTEstrcat(BYTE *str1, char *str2);
static io_service service;

class _client : public boost::enable_shared_from_this<_client>
{
public:
	_client() : _sock(service)
	{
		_name = new BYTE[4096];

		data = new BYTE[4096];
	}
	BYTE *username(BYTE *_newname)
	{

		for (int i = 0; i <= BYTEstrlen(_newname); i++)
		{
			_name[i] = _newname[i];
		}
		return _name;
	}
	BYTE *username() { return _name; }
	ip::tcp::socket & sock() { return _sock; }
	BYTE *_prepData()
	{
		for (int i = 0; i <= BYTEstrlen(_name); i++)
		{
			data[i] = _name[i];
		}
		BYTEstrcat(data, ": ");
		return data;
	}
	BYTE *_data() { return data; }

	~_client()
	{
		delete _name;
		delete data;
	}
private:
	ip::tcp::socket _sock;
	BYTE *data;
	BYTE *_name;
};

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
	for (int i = BYTEstrlen(str1), k = 0; ((i <= (BYTEstrlen(str1) + BYTEstrlen(str2)))&&(i<4096)); i++, k++)
		str1[i] = str2[k];
}
void BYTEstrcat(BYTE *str1, char *str2)
{
	for (int i = BYTEstrlen(str1), k = 0; ((i <= (BYTEstrlen(str1) + strlen(str2))) && (i<4096)); i++, k++)
		str1[i] = str2[k];
}