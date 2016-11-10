# pragma once

# include "Component.hpp"

# include <map>
# include <string>
# include <functional>

namespace User
{
    class CPMeshFilter : public Component
    {
    public:
        CPMeshFilter( const std::string& meshName = "null" );
        virtual ~CPMeshFilter( ) { }
    public:
        virtual void draw( )override;
    protected:
        std::function<void()> meshFunction;
        static std::map<std::string, std::function<void( )>> filters;
    public: NAME
    };
}