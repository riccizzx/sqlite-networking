#include "server.hpp"
#include "server-config.hpp"

ListNode* userList = NULL;

Sv::Sv() {
	FD_ZERO(&masterReadSet);
	FD_ZERO(&masterWriteSet);
}

Sv::~Sv() {
	void cleanup();
}

void Sv::cleanup() {
	// Close the server socket
	for (SOCKET s : threads) {
		if (s != INVALID_SOCKET) {
			closesocket(s);
		}
		threads.clear();

		if (serverSock != INVALID_SOCKET) {
			closesocket(serverSock);
			serverSock = INVALID_SOCKET;
		}
	}
	// Cleanup Winsock
	WSACleanup();
	std::cout << "Server cleaned up and closed." << std::endl;
}

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

//handle client with select for multiple clients
void Sv::handleClient() {
	//get client INFO

	char host[NI_MAXHOST];
	char service[NI_MAXSERV];
	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);

	FD_SET(serverSock, &masterReadSet);
	FD_SET(serverSock, &masterWriteSet);
	maxFd = serverSock;

	while (1) {
		FD_ZERO(&masterReadSet);
		FD_ZERO(&masterWriteSet);
		
		nVal = select(0, &masterReadSet, &masterWriteSet, NULL, NULL);
		if (nVal == SOCKET_ERROR) {
			std::cerr << "Select failed!" << std::endl;
			cleanup();
			exit(EXIT_FAILURE);
		}
		if (nVal == 0) {
			continue; // No sockets are ready, continue the loop
		}
		if (FD_ISSET(serverSock, &masterReadSet)) {
			sockaddr_in conAddr;
			int addrLen = sizeof(conAddr);
			SOCKET conSock = accept(serverSock, (sockaddr*)&conAddr, &addrLen);
			if (conSock == INVALID_SOCKET) {
				std::cerr << "Accept failed!" << std::endl;
				cleanup();
				exit(EXIT_FAILURE);
			}
			else {
				std::cout << "New connection accepted from: ";
				threads.push_back(conSock);
				FD_SET(conSock, &masterReadSet);
				FD_SET(conSock, &masterWriteSet);

				if (conSock > maxFd) maxFd = conSock;

				if (getnameinfo((sockaddr*)&conAddr, sizeof(conAddr), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
					std::cout << host << " on port: " << service << std::endl;
				}
				else {
					inet_ntop(AF_INET, &conAddr.sin_addr, host, NI_MAXHOST);
					std::cout << "New connection from " << host << ": " << ntohs(conAddr.sin_port) << std::endl;
				}
			}
		}nVal--;
	}
}

//from namespace
void* server::handleRequest(int* fd) {

}