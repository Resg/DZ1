#pragma once
#include "stdafx.h"
#include <boost\asio.hpp>
#include <iostream>
#include <memory>
#include <boost\thread.hpp>
#include <vector>
#include <string>
#include <algorithm>
#include "mycrypto.hpp"
#include "Kuznyechik.hpp"
using namespace boost::asio;



int BYTEstrlen(BYTE *str);
void BYTEstrcat(BYTE *str1, BYTE *str2);
void BYTEstrcat(BYTE *str1, char *str2);
bool BYTEstrcmp(BYTE *str1, BYTE *str2);
static io_service service;
static int _intIter = 0;

class _client : public boost::enable_shared_from_this<_client>
{
public:
	_client();
	BYTE *username(BYTE *_newname);
	BYTE *username();
	BYTE *key(BYTE *_newkey);
	BYTE *key();
	ip::tcp::socket & sock();
	BYTE *_prepData();
	BYTE *_data();
	BYTE *room();
	int &clientIter();
	ByteBlock Encrypt(BYTE *_buffer);
	ByteBlock Decrypt(BYTE *_buffer);
	bool isalive;
	~_client();
private:
	ip::tcp::socket _sock;
	int _clientIter;
	BYTE *_key;
	BYTE *data;
	BYTE *_name;
	BYTE *_room;
};

