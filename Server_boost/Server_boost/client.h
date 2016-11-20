#pragma once

#include <boost\asio.hpp>
#include <iostream>
#include <memory>
#include <boost\thread.hpp>
#include <vector>
#include <string>
using namespace boost::asio;

static io_service service;

class _client : public boost::enable_shared_from_this<_client>
{
public:
	_client();
	char *username(char *_newname);
	char *username();
	ip::tcp::socket & sock();
	char *_prepData();
	char *_data();
	~_client();
private:
	ip::tcp::socket _sock;
	char *data;
	char *_name;
};

