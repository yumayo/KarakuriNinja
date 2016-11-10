# pragma once

# include <string>

namespace User
{
    class Object
    {
    public:
        virtual std::string name( ) = 0;
    public:
        Object( ) { }
        virtual ~Object( ) {}
    };
}