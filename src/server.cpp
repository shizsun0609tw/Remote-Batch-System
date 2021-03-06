#include "server.h"

boost::asio::io_context *Session::io_context_;

Session::Session(tcp::socket socket) : socket_(std::move(socket))
{
	status_str = "HTTP/1.1 200 OK\n";
}

void Session::SetContext(boost::asio::io_context *io_context)
{
	io_context_ = io_context;
}

void Session::Start()
{
	DoRead();
}

void Session::SetEnv()
{
	#ifdef __linux__
	setenv("REQUEST_METHOD", REQUEST_METHOD.c_str(), 1);
	setenv("REQUEST_URI", REQUEST_URI.c_str(), 1);
	setenv("QUERY_STRING", QUERY_STRING.c_str(), 1);
	setenv("SERVER_PROTOCOL", SERVER_PROTOCOL.c_str(), 1);
	setenv("HTTP_HOST", HTTP_HOST.c_str(), 1);
	setenv("SERVER_ADDR", SERVER_ADDR.c_str(), 1);
	setenv("SERVER_PORT", SERVER_PORT.c_str(), 1);
	setenv("REMOTE_ADDR", REMOTE_ADDR.c_str(), 1);
	setenv("REMOTE_PORT", REMOTE_PORT.c_str(), 1);
	setenv("EXEC_FILE", EXEC_FILE.c_str(), 1);
	#endif
}

void Session::PrintEnv()
{
	#ifdef __linux__
	REQUEST_METHOD = getenv("REQUEST_METHOD");
	REQUEST_URI = getenv("REQUEST_URI");
	QUERY_STRING = getenv("QUERY_STRING");
	SERVER_PROTOCOL = getenv("SERVER_PROTOCOL");
	HTTP_HOST = getenv("HTTP_HOST");
	SERVER_ADDR = getenv("SERVER_ADDR");
	SERVER_PORT = getenv("SERVER_PORT");
	REMOTE_ADDR = getenv("REMOTE_ADDR");
	REMOTE_PORT = getenv("REMOTE_PORT");
	EXEC_FILE = getenv("EXEC_FILE");
	#endif

	std::cout << "--------------------------------------------" << std::endl;
	std::cout << "REQUEST_METHOD: " << REQUEST_METHOD << std::endl;
	std::cout << "REQUEST_URI: " << REQUEST_URI << std::endl;
	std::cout << "QUERY_STRING: " << QUERY_STRING << std::endl;
	std::cout << "SERVER_PROTOCOL: " << SERVER_PROTOCOL << std::endl;
	std::cout << "HTTP_HOST: " << HTTP_HOST << std::endl;
	std::cout << "SERVER_ADDR: " << SERVER_ADDR << std::endl;
	std::cout << "SERVER_PORT: " << SERVER_PORT << std::endl;
	std::cout << "REMOTE_ADDR: " << REMOTE_ADDR << std::endl;
	std::cout << "REMOTE_PORT: " << REMOTE_PORT << std::endl;
	std::cout << "EXEC_FILE: " << EXEC_FILE << std::endl;
	std::cout << "--------------------------------------------" << std::endl;
}

void Session::DoRead()
{
	auto self(shared_from_this());
	socket_.async_read_some(boost::asio::buffer(data_, max_length),
		[this, self](boost::system::error_code ec, std::size_t length)
		{
			if (ec) return;
			
			std::string temp(data_);
			std::istringstream iss(temp);

			iss >> REQUEST_METHOD >> REQUEST_URI >> SERVER_PROTOCOL >> temp >> HTTP_HOST;

			SERVER_ADDR = socket_.local_endpoint().address().to_string();
			REMOTE_ADDR = socket_.remote_endpoint().address().to_string();

			SERVER_PORT = std::to_string(socket_.local_endpoint().port());
			REMOTE_PORT = std::to_string(socket_.remote_endpoint().port());

			iss = std::istringstream(REQUEST_URI);
			std::getline(iss, REQUEST_URI, '?');
			std::getline(iss, QUERY_STRING);

			#ifdef __linux__
			EXEC_FILE = boost::filesystem::current_path().string() + REQUEST_URI;
			#endif

			SetEnv();
			PrintEnv();		

			DoWrite(length);
		});
}

void Session::DoWrite(std::size_t length)
{
	auto self(shared_from_this());
	boost::asio::async_write(socket_, boost::asio::buffer(status_str, status_str.length()),
		[this, self](boost::system::error_code ec, std::size_t )
		{
			if (ec) return;

			DoCGI();
		});
}

void Session::DoCGI()
{
	#ifdef __linux__
	(*Session::io_context_).notify_fork(boost::asio::io_context::fork_prepare);

	if (fork() != 0)
	{
		(*Session::io_context_).notify_fork(boost::asio::io_context::fork_parent);
		socket_.close();
	}
	else
	{
		(*Session::io_context_).notify_fork(boost::asio::io_context::fork_child);
		int sock = socket_.native_handle();
		
		dup2(sock, STDERR_FILENO);
		dup2(sock, STDIN_FILENO);
		dup2(sock, STDOUT_FILENO);

		socket_.close();

		if (execlp(EXEC_FILE.c_str(), EXEC_FILE.c_str(), NULL) < 0)
		{
			std::cout << "Content-type:text/html\r\n\r\n<h1>Fail</h1>";
			fflush(stdout);
		}
	}
	#else
	if (REQUEST_URI == "/console.cgi")
	{
		myConsole.SetWebSocket(&socket_);
		myConsole.SetQuery(QUERY_STRING);
		myConsole.InitClients();
		myConsole.Link2Server();
		myConsole.Run();
	}
	else if (REQUEST_URI == "/panel.cgi")
	{
		myPanel.Run(socket_);
	}
	#endif
}

Server::Server(boost::asio::io_context& io_context, short port)
		: acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
{
	Session::SetContext(&io_context);

	DoAccept();
}

void Server::DoAccept()
{
	acceptor_.async_accept(
		[this](boost::system::error_code ec, tcp::socket socket)
		{
			if (!ec) std::make_shared<Session>(std::move(socket))->Start();
			DoAccept();
		}
	);
}
