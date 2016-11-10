#include "Network.h"

# pragma comment(lib, "wsock32.lib")

# include "NetworkDefines.h"

namespace User
{
    struct Network::UDP::P2P::Send::_m_Send
    {
        WSAData wsaData;
        Socket sock;
        SocketAddressIn addr;
        std::string address;
    };
    Network::UDP::P2P::Send::Send( const int & port, const std::string & address )
    {
        member = std::make_shared<Network::UDP::P2P::Send::_m_Send>();

        WSAStartup( MAKEWORD( 2, 0 ), &member->wsaData );

        member->sock = socket( AF_INET, SOCK_DGRAM, 0 );

        member->addr.sin_family = AF_INET;

        Send::port( port );
        Send::address( address.c_str( ) );
    }
    Network::UDP::P2P::Send::~Send( )
    {
        closesocket( member->sock );

        WSACleanup( );
    }
    void Network::UDP::P2P::Send::port( const int & port )
    {
        member->addr.sin_port = htons( port );
    }
    void Network::UDP::P2P::Send::address( const std::string & address )
    {
        member->addr.sin_addr.S_un.S_addr = inet_addr( address.c_str( ) );
        member->address = address;
    }
    void Network::UDP::P2P::Send::send( const char* str, const size_t size )
    {
        sendto( member->sock, str, size, 0, reinterpret_cast<SocketAddressPtr>( &member->addr ), sizeof( member->addr ) );
    }
}