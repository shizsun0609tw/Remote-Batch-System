#pragma once

#include <vector>
#include <string>
#include <boost/asio.hpp>

class Client
{
public:
	Client(boost::asio::io_context &io_context, int port, std::string addr, std::string file);

public:
	int serverPort;
	std::string serverAddr;
	std::string testFile;
	boost::asio::ip::tcp::socket socket;
	char data_[1024];
};

class Console
{
public:
	Console();
	
public:
	void InitClients();
	void Link2Server();
	void Run();

private:
	void SendInitialHTML();
	void SendShellInput(int session, std::vector<std::string> input);
	void SendShellOutput(int session, std::string content);
	std::vector<std::string> GetShellInput(std::string testFile);
	void GetShellOutput(int session, std::vector<std::string> input);

private:
	std::vector<Client> clients;
};

