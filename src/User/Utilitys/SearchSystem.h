# pragma once

#include <iostream>
#include <algorithm>
#include <filesystem> // std::tr2::sys::path etc.
#include <map>
#include <vector>
#include <string>

namespace User
{
    class SearchSystem
    {
        std::string root;
        std::vector<std::string> files;
    public:
        SearchSystem( std::string const& root );
        std::vector<std::string>& Files( );
    private:
        void Directory( std::tr2::sys::path const& path );
        void File( std::tr2::sys::path const& path );
        void Search( std::tr2::sys::path const& path );
    };
}