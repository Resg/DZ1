#pragma once

#include <boost\asio.hpp>
#include <iostream>
#include <memory>
#include <boost\thread.hpp>
#include <vector>
#include <string>
using namespace boost::asio;

io_service service;

class _client
{
public:
	_client(std::string username): _sock(service),_name(username) {}
	_client() : _sock(service), _name("default") {}
	std::string username() const { return _name; }
	std::string username(std::string _newname) 
	{
		_name = _newname;
		return _name;
	}
	ip::tcp::socket & sock() { return _sock; }
private:
	ip::tcp::socket _sock;
	std::string _name;
};