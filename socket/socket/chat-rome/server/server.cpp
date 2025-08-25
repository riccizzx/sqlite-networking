#include "server.hpp"
#include "server-config.hpp"

ListNode* userList = NULL;

Sv::Sv() {
	FD_ZERO(&masterReadSet);
	FD_ZERO(&masterWriteSet);
}

Sv::~Sv() {
	cleanup();
}

void Sv::cleanup() {
	for (SOCKET s : threads) {
		if (s != INVALID_SOCKET) {
			closesocket(s);
		}
	}
	threads.clear();

	if (serverSock != INVALID_SOCKET) {
		closesocket(serverSock);
		serverSock = INVALID_SOCKET;
	}
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
    char host[NI_MAXHOST];
    char service[NI_MAXSERV];
    ZeroMemory(host, NI_MAXHOST);
    ZeroMemory(service, NI_MAXSERV);

    fd_set allset, rset;
    FD_ZERO(&allset);
    FD_SET(serverSock, &allset);   // add listening socket
    maxFd = serverSock;

    // keep track of clients
    std::vector<SOCKET> client_sockets(FD_SETSIZE, INVALID_SOCKET);

    while (true) {
        rset = allset; // copy master set

        nVal = select(maxFd + 1, &rset, NULL, NULL, NULL);
        if (nVal == SOCKET_ERROR) {
            std::cerr << "Select failed!" << std::endl;
            cleanup();
            exit(EXIT_FAILURE);
        }

        // Check if the listening socket is ready
        if (FD_ISSET(serverSock, &rset)) {
            sockaddr_in conAddr;
            int addrLen = sizeof(conAddr);
            SOCKET conSock = accept(serverSock, (sockaddr*)&conAddr, &addrLen);
            if (conSock == INVALID_SOCKET) {
                std::cerr << "Accept failed!" << std::endl;
                cleanup();
                exit(EXIT_FAILURE);
            }

            // print info
            if (getnameinfo((sockaddr*)&conAddr, sizeof(conAddr),
                host, NI_MAXHOST,
                service, NI_MAXSERV, 0) == 0) {
                std::cout << "New connection accepted from: "
                    << host << " on port " << service << std::endl;
            }
            else {
                inet_ntop(AF_INET, &conAddr.sin_addr, host, NI_MAXHOST);
                std::cout << "New connection from " << host
                    << ": " << ntohs(conAddr.sin_port) << std::endl;
            }

            // add to client list
            bool added = false;
            for (int i = 0; i < FD_SETSIZE; i++) {
                if (client_sockets[i] == INVALID_SOCKET) {
                    client_sockets[i] = conSock;
                    added = true;
                    break;
                }
            }
            if (!added) {
                std::cerr << "Too many clients" << std::endl;
                closesocket(conSock);
            }
            else {
                FD_SET(conSock, &allset);
                if (conSock > maxFd) maxFd = conSock;
            }

            if (--nVal <= 0) continue; // no more fds ready
        }

        // check all client sockets
        for (int i = 0; i < FD_SETSIZE; i++) {
            SOCKET sockfd = client_sockets[i];
            if (sockfd == INVALID_SOCKET) continue;

            if (FD_ISSET(sockfd, &rset)) {
                char buffer[4096];
                int bytesIn = recv(sockfd, buffer, sizeof(buffer), 0);
                if (bytesIn <= 0) {
                    // connection closed
                    closesocket(sockfd);
                    FD_CLR(sockfd, &allset);
                    client_sockets[i] = INVALID_SOCKET;
                }
                else {
                    buffer[bytesIn] = '\0';
                    std::cout << "Received from client: " << buffer << std::endl;
                    // Example echo back
                    send(sockfd, buffer, bytesIn, 0);
                }
                if (--nVal <= 0) break; // no more ready sockets
            }
        }
    }
}

/*
void* server::handleRequest(int* fd) {
    int sockfd = *fd;
    int ret, rec;

    Message message{};
    char buf[MAX_LINE]{};

    rec = recv(sockfd, buf, sizeof(buf), 0);
    if (rec <= 0) {
        std::cout << "Disconnected!" << std::endl;
        closesocket(sockfd);
        *fd = -1;
        return nullptr;
    }

    memcpy(&message, buf, sizeof(message));

    switch (message.msgType) {
    case static_cast<int>(MessageType::REGISTER): {
        std::cout << "REGISTER from: " << inet_ntoa(message.sendAddr.sin_addr) << "\n";
        ret = registerUser(message, sockfd);

        message.msgType = static_cast<int>(MessageType::RESULT);
        message.msgRet = static_cast<int>(ret);

        send(sockfd, reinterpret_cast<char*>(&message), sizeof(message), 0);
        closesocket(sockfd);
        *fd = -1;
        return nullptr;
    }
    case static_cast<int>(MessageType::LOGIN): {
        std::cout << "LOGIN from: " << inet_ntoa(message.sendAddr.sin_addr) << "\n";
        ret = loginUser(message, sockfd);

        message.msgType = static_cast<int>(MessageType::RESULT);
        message.msgRet = static_cast<int>(ret);

        send(sockfd, reinterpret_cast<char*>(&message), sizeof(message), 0);
        enterChat(&sockfd);
        break;
    }
    default:
        std::cout << "Unknown operation.\n";
        break;
    }

    closesocket(sockfd);
    *fd = -1;
    return nullptr;
}
*/