
#pragma once

#ifndef WINSOCK_CLIENT_CONFIG_HPP
#define WINSOCK_CLIENT_CONFIG_HPP

#include "chat-rome/server/server-config.hpp"

namespace client {
    class Cl {
    public:
        void Init();
        void conn();
    private:
        WSADATA wsa;
        SOCKET clientSock;
        sockaddr_in clientAddr;
        void quit();
    };
}

/*
enum class Flag {
	YES,
	NO
};

// msg type for hadle options
enum class MessageType {
    REGISTER = 1,
    LOGIN,
    HELP,
    EXIT,
    VIEW_USER_LIST,
    GROUP_CHAT,
    PERSONAL_CHAT,
    VIEW_RECORDS,
    RESULT,
    UNKNOWN
};

enum class StateRet {
    EXCEED,
    SUCCESS,
    FAILED,
    DUPLICATE_ID,
    INVALID,
    ID_NOT_EXIST,
    WRONG_PWD,
    ALREADY_ONLINE,
    ID_NOT_ONLINE,
    ALL_NOT_ONLINE,
    MESSAGE_SELF
};

//message transfer class
class Message {
public:
    char content[2048];
    int msgType;
    int msgRet;
    sockaddr_in sendAddr;
    sockaddr_in recvAddr;
    char sendName[20];
    char recvName[20];
    char msgTime[20];
};

class User {
public:
    std::string userName;
    std::string password;
    sockaddr_in userAddr{};
    int sockfd = -1;
    bool canSpeak = true;
    std::string registerTime;

    // copy constructor
    User(const User& other) = default;
    User& operator=(const User& other) = default;

    User() = default;
};

class ListNode {
    User user;
    ListNode* next = nullptr;
};

extern ListNode* userList;
*/

#endif

