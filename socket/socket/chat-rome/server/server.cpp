#include "server.hpp"
#include "server-config.hpp"

ListNode* userList = NULL;

void Sv::Initialize() {
	// Initialize Winsock
	if (WSAStartup(MAKEWORD(2, 2), &ws) != 0) {
		std::cerr << "WSAStartup failed!" << std::endl;
		exit(EXIT_FAILURE);
	}
	// Create a socket
	serverSock = socket(AF_INET, SOCK_STREAM,0);
	if (serverSock == INVALID_SOCKET) {
		std::cerr << "Socket creation failed!" << std::endl;
		WSACleanup();
		exit(EXIT_FAILURE);
	}
}

void Sv::Config() {
	// Set up the server address structure
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = INADDR_ANY;

	// Bind the socket
	if (bind(serverSock, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
		std::cerr << "Bind failed!" << std::endl;
		closesocket(serverSock);
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	// Start listening for incoming connections
	if (listen(serverSock, LISTEN_QUEUE) == SOCKET_ERROR) {
		std::cerr << "Listen failed!" << std::endl;
		closesocket(serverSock);
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	std::cout << "Server is ready to accept connections on port " << PORT << std::endl;
}