# pragma once

# include <memory>
# include <string>

namespace User
{
    class Network
    {
    public:
        class UDP
        {
        public:
            class P2P
            {
            public:
                class Recv
                {
                    struct _m_Recv;
                    std::shared_ptr<_m_Recv> member;
                public:
                    Recv( const int& port );
                    ~Recv( );
                public:
                    void port( const int& port );
                public:
                    int recv( char* str, const size_t size );
                    int recv( std::string& str );
                };
            public:
                class Send
                {
                    struct _m_Send;
                    std::shared_ptr<_m_Send> member;
                public:
                    Send( const int& port, const std::string& address );
                    ~Send( );
                public:
                    void port( const int& port );
                    void address( const std::string& address );
                public:
                    void send( const char* str, const size_t size );
                };
            };
        };
    };
}