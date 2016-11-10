#include "SearchSystem.h"

#include "ETC.h"

namespace User
{
    using namespace std;
    namespace sys = std::tr2::sys;

    SearchSystem::SearchSystem( std::string const & root )
        : root( root )
    {
        Search( root );
    }
    std::vector<std::string>& SearchSystem::Files( )
    {
        return files;
    }
    void SearchSystem::Directory( sys::path const & path )
    {
        Search( path );
    }
    void SearchSystem::File( sys::path const & path )
    {
        auto strs = Utl::split( path.string( ).substr( root.size( ) + 1 ), '\\' );
        std::string file = strs[0]; // splitで帰ってくる文字列は絶対にnullにはなっていないので。
        for ( int i = 1; i < strs.size( ); ++i )
        {
            file += '/' + strs[i];
        }
        files.emplace_back( file );
    }
    void SearchSystem::Search( sys::path const & path )
    {
        sys::path p( path ); // 列挙の起点
        std::for_each( sys::directory_iterator( p ), sys::directory_iterator( ), [ this ] ( sys::path const& p )
        {
            // ファイルなら...
            if ( sys::is_regular_file( p ) )
            {
                File( p );
            }
        } );

        p = sys::path( path ); // 列挙の起点
        std::for_each( sys::directory_iterator( p ), sys::directory_iterator( ), [ this ] ( sys::path const& p )
        {
            // ディレクトリなら...
            if ( sys::is_directory( p ) )
            {
                Directory( p );
            }
        } );
    }
}