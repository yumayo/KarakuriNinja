#include "Network.h"

# pragma comment(lib, "wsock32.lib")

# include "NetworkDefines.h"

namespace User
{
    struct Network::UDP::P2P::Recv::_m_Recv
    {
        WSAData wsaData;
        Socket sock;
        SocketAddressIn addr;
        SocketAddressIn sendAddr;
        SocketSize sendSocketSize;
        FdSet fds;
        FdSet readfds;
        TimeValue tv;
        char buf[256];
    };
    Network::UDP::P2P::Recv::Recv( const int & port )
    {
        member = std::make_shared<Network::UDP::P2P::Recv::_m_Recv>( );

        member->sendSocketSize = sizeof( SocketAddressIn );

        WSAStartup( MAKEWORD( 2, 0 ), &member->wsaData );

        member->sock = socket( AF_INET, SOCK_DGRAM, 0 );

        Recv::port( port );

        FD_ZERO( &member->readfds );
        FD_SET( member->sock, &member->readfds );

        member->tv.tv_sec = 0;
        member->tv.tv_usec = 100;
    }
    Network::UDP::P2P::Recv::~Recv( )
    {
        closesocket( member->sock );

        WSACleanup( );
    }
    void Network::UDP::P2P::Recv::port( const int & port )
    {
        member->addr.sin_family = AF_INET;
        member->addr.sin_port = htons( port );
        member->addr.sin_addr.S_un.S_addr = INADDR_ANY;

        bind( member->sock, reinterpret_cast<SocketAddressPtr>( &member->addr ), sizeof( member->addr ) );
    }
    int Network::UDP::P2P::Recv::recv( char* str, const size_t size )
    {
        memcpy( &member->fds, &member->readfds, sizeof( FdSet ) );

        if ( !select( 0, &member->fds, NULL, NULL, &member->tv ) ) return 1;

        if ( FD_ISSET( member->sock, &member->fds ) )
        {
            memset( member->buf, 0, sizeof( member->buf ) );
            auto ret = recvfrom( member->sock, member->buf, sizeof( member->buf ), 0, 
                                 reinterpret_cast<SocketAddressPtr>( &member->sendAddr ), &member->sendSocketSize );
            if ( ret == SOCKET_ERROR ) auto err = WSAGetLastError( );
            else
            {
                for ( size_t i = 0; i < size; ++i )
                {
                    str[i] = member->buf[i];
                }
            }
        }
        return 0;
    }
    int Network::UDP::P2P::Recv::recv( std::string& str )
    {
        memcpy( &member->fds, &member->readfds, sizeof( FdSet ) );

        if ( !select( 0, &member->fds, NULL, NULL, &member->tv ) ) return 1;

        if ( FD_ISSET( member->sock, &member->fds ) )
        {
            memset( member->buf, 0, sizeof( member->buf ) );
            auto ret = recvfrom( member->sock, member->buf, sizeof( member->buf ), 0,
                                 reinterpret_cast<SocketAddressPtr>( &member->sendAddr ), &member->sendSocketSize );
            if ( ret == SOCKET_ERROR ) auto err = WSAGetLastError( );
            else
            {
                str = member->buf;
            }
        }
        return 0;
    }
}