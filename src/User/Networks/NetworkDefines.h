# pragma once

# include <winsock2.h>
# include <ws2tcpip.h>

using SocketSize = socklen_t;
using SocketAddress = struct sockaddr;
using SocketAddressPtr = SocketAddress*;
using SocketAddressIn = struct sockaddr_in;
using TimeValue = struct timeval;
using WSAData = WSAData;
using Socket = SOCKET;
using FdSet = fd_set;