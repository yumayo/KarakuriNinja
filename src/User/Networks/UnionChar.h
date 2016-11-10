# pragma once

namespace User
{
    template<class _Ty>
    union UnionChar
    {
        UnionChar( ) {}
        UnionChar( const _Ty& structure )
            : structure( structure )
        { }
        UnionChar( const char* str, const size_t size )
        {
            for ( size_t i = 0; i < size; ++i )
            {
                this->str[i] = str[i];
            }
        }

        _Ty structure;
        char str[sizeof( _Ty )];
    };
}