#pragma once

#include <boost/asio.hpp>

class Panel
{
public:
	Panel();

public:
	void Run(boost::asio::ip::tcp::socket &socket);

private:
	void DoWrite(boost::asio::ip::tcp::socket &socket, std::string content);
};
