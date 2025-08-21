#pragma once

#ifndef WINSOCK_SERVER_CONFIG_HPP
#define WINSOCK_SERVER_CONFIG_HPP

#include <string>
#include <thread>
#include <vector>
#include <iostream>
#include <WS2tcpip.h>
#include <WinSock2.h>

//sqlite3 library
#include "winsqlite/winsqlite3.h"

// winsock library
#pragma comment(lib, "ws2_32.lib")

constexpr const char* DB_NAME = "/sqlite-networking/socket/chat-rome/chatRome.db";
constexpr int PORT = 9909;
constexpr int MAX_LINE = 8192;
constexpr int LISTEN_QUEUE = 6000;

enum class Flag {
    YES,
    NO
};

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

class Message {
public:
    std::string content;
    MessageType msgType{};
    StateRet msgRet{};
    sockaddr_in sendAddr{};
    sockaddr_in recvAddr{};
    std::string sendName;
    std::string recvName;
    std::string msgTime;
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
    struct _ListNode* next;
};

extern ListNode* userList;

//using UserList = std::vector<User>;

namespace server {
    const char* stateMsg(StateRet stateRet);
    void copyUser(User& dest, const User& src);

    void enterChat(int fd);
    int groupChat(const Message& msg, int sockfd);
    int personalChat(const Message& msg, int sockfd);
    int viewUserList(const Message& msg, int sockfd);
    int viewRecords(const Message& msg, int sockfd);

    int loginUser(const Message& msg, int sockfd);
    int registerUser(const Message& msg, int sockfd);
}

#endif // WINSOCK_SERVER_CONFIG_HPP
