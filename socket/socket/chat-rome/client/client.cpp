#include "client-config.hpp"

void client::Cl::Init() {
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		std::cerr << "ERROR!";
		WSACleanup();
	}
	clientSock = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSock == INVALID_SOCKET) {
		std::cerr << "ERROR";
		WSACleanup();
	}

	clientAddr.sin_family = AF_INET;
    clientAddr.sin_port = htons(PORT);
	inet_pton(AF_INET, "127.0.0.1", &clientAddr.sin_addr);

}

void client::Cl::conn() {
	Cl cli;
	if (connect(clientSock, (sockaddr*)&clientAddr, sizeof(clientAddr)) == SOCKET_ERROR) {
		std::cerr << "Can't connect to the server: " << WSAGetLastError();
		WSACleanup();
	}
	else {
		std::cout << "Connected to the server!";
	}

	cli.mainInterface();
	

}