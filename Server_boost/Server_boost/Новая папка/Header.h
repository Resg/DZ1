#pragma once

#include <boost\asio.hpp>
#include <iostream>
#include <memory>
#include <boost\thread.hpp>
#include <vector>
#include <string>
using namespace boost::asio;



io_service service;
class _client : public boost::enable_shared_from_this<_client>
{
public:
	//_client(std::string username): _sock(service),_name(username) {}
	_client();
	char *username();
	char *username(char *_newname);
	ip::tcp::socket & sock();
	char *_prepData();
	char *_data();
private:
	ip::tcp::socket _sock;
	char *data;
	char *_name;
};

