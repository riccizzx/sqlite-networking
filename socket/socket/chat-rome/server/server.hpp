#ifndef SERVER_HPP
#define SERVER_HPP

#include "server-config.hpp"

class Sv {
public:
	Sv();
	~Sv();
	void Initialize();
	void Config();
	void handleClient();

private:
	WSADATA ws;
	SOCKET serverSock;
	std::vector<SOCKET> threads;

	FD_SET masterReadSet;
	FD_SET masterWriteSet;

	sockaddr_in serverAddr{};
	int maxFd = 0;
	int nVal = 0;

	Message message;

	User user;

	char buf[MAX_LINE]{};
	void cleanup();
};
#endif // SERVER_HPP

