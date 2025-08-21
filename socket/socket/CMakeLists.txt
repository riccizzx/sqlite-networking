# CMakeList.txt : CMake project for socket, include source and define
# Ensure server.hpp is in the include path
include_directories(${CMAKE_SOURCE_DIR}/socket)
#include "server.hpp"
# project specific logic here.
#

# Add source to this project's executable.
add_executable (socket "socket.cpp" "socket.h" "chat-rome/server/server-config.cpp" "chat-rome/client/client-config.cpp" "chat-rome/server/server.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET socket PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
