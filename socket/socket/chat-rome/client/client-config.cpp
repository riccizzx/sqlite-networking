#include "client-config.hpp"

const char* stateMsg(StateRet stateRet) {
    switch (stateRet) {
    case StateRet::EXCEED:          return "Limit for connection exceeded!\n"; break;
    case StateRet::SUCCESS:         return "Operation successful!\n"; break;
    case StateRet::FAILED:          return "Operation failed!\n"; break;
    case StateRet::DUPLICATE_ID:    return "Duplicate username!\n"; break;
    case StateRet::INVALID:         return "Invalid input!\n"; break;
    case StateRet::ID_NOT_EXIST:    return "Account does not exist!\n"; break;
    case StateRet::WRONG_PWD:       return "Wrong password!\n"; break;
    case StateRet::ALREADY_ONLINE:  return "The user is already online!\n"; break;
    case StateRet::ID_NOT_ONLINE:   return "User is not online!\n"; break;
    case StateRet::ALL_NOT_ONLINE:  return "No one is online!\n"; break;
    case StateRet::MESSAGE_SELF:    return "Cannot send message to myself\n"; break;
    default:                        return "Unknown operation!\n"; break;
    }
}

void copyUser(User& dest, const User& src) {
    dest = src; // use the default copy assignment operator
}