#ifndef SERVER_HPP
#define SERVER_HPP

#include "server-config.hpp"

class Sv {
public:
	void Initialize();
	void Config();
private:
	WSADATA ws;
	SOCKET serverSock;
	std::vector<std::thread> threads;
	FD_SET readSet;
	sockaddr_in serverAddr{};
};
#endif // SERVER_HPP