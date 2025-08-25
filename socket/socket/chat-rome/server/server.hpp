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
    WSADATA ws{};
    SOCKET serverSock = INVALID_SOCKET;
    std::vector<SOCKET> threads;

    fd_set masterReadSet{};
    fd_set masterWriteSet{};

    sockaddr_in serverAddr{};
    int maxFd = 0;
    int nVal = 0;

    void cleanup();
};

#endif // SERVER_HPP

