# pragma once

# include "../Assets/Assets.h"
# include "cinder/gl/Texture.h"
# include "Audio.h"
# include "cinder/Trimesh.h"

# include "SearchSystem.h"

namespace User
{
    class GlobalTexture
    {
        size_t index;
        SearchSystem search;
        Izanami::Holder<cinder::gl::Texture> datas;
    private:
        GlobalTexture( );
    private:
        void Start( );
        bool IsSetuped( );
        size_t MaxLoadIndex( );
        size_t NowLoadIndex( );
    private:
        friend class GlobalData;
    };

    class GlobalAudio
    {
        size_t index;
        SearchSystem search;
        Izanami::Holder<Audio> datas;
    private:
        GlobalAudio( );
    private:
        void Start( );
        bool IsSetuped( );
        size_t MaxLoadIndex( );
        size_t NowLoadIndex( );
    private:
        friend class GlobalData;
    };

    class GlobalObj
    {
        size_t index;
        SearchSystem search;
        Izanami::Holder<cinder::TriMesh> datas;
    private:
        GlobalObj( );
    private:
        void Start( );
        bool IsSetuped( );
        size_t MaxLoadIndex( );
        size_t NowLoadIndex( );
    private:
        friend class GlobalData;
    };

    class GlobalData
    {
        static GlobalTexture textureHolder;
        static GlobalAudio audioHolder;
        static GlobalObj objHolder;
    private:
        GlobalData( ) { /*nothing*/ }
    public:
        static void Start( );
        static bool IsSetuped( );
        static size_t MaxLoadIndex( );
        static size_t NowLoadIndex( );
        static double NormalizedLoadIndex( );
        static cinder::gl::Texture& FindTexture( std::string const& path );
        static Audio& FindAudio( std::string const& path );
        static cinder::TriMesh& FindObj( std::string const& path );
    };
}