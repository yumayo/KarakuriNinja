# pragma once

# include <vector>
# include <string>

namespace Utl
{
    // 文字列を char文字 で分割します。
    std::vector<std::string> split( const std::string &str, char sep );
    std::string getFullPath( const std::string& assetPath );
}