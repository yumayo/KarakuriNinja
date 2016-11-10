# pragma once

# include "../Assets/Assets.h"
# include "cinder/gl/Texture.h"
# include "Audio.h"
# include "cinder/Trimesh.h"

# include "SearchSystem.h"

namespace User
{
    template <class Ty>
    class GlobalBase
    {
    protected:
        size_t index;
        std::shared_ptr<SearchSystem> search;
        Izanami::Holder<Ty> datas;
    public:
        GlobalBase( );
        void Setup( std::string const& path );
        virtual void Start( ) = 0;
        bool IsSetuped( );
        size_t MaxLoadIndex( );
        size_t NowLoadIndex( );
        Izanami::Holder<Ty>& Datas( );
    };

    template<class Ty>
    inline GlobalBase<Ty>::GlobalBase( )
        : index( 0 )
    {
    }
    template<class Ty>
    inline void GlobalBase<Ty>::Setup( std::string const& path )
    {
        index = 0;
        search = std::make_shared<SearchSystem>( path );
        datas.clear( );
    }
    template<class Ty>
    inline bool GlobalBase<Ty>::IsSetuped( )
    {
        return index == MaxLoadIndex( );
    }
    template<class Ty>
    inline size_t GlobalBase<Ty>::MaxLoadIndex( )
    {
        auto& files = search->Files( );
        return files.size( );
    }
    template<class Ty>
    inline size_t GlobalBase<Ty>::NowLoadIndex( )
    {
        return index;
    }
    template<class Ty>
    inline Izanami::Holder<Ty>& GlobalBase<Ty>::Datas( )
    {
        return datas;
    }

    class GlobalTexture : public GlobalBase<cinder::gl::Texture>
    {
    public:
        void Start( )override;
    };

    class GlobalAudio : public GlobalBase<Audio>
    {
    public:
        void Start( )override;
    };

    class GlobalObj : public GlobalBase<cinder::TriMesh>
    {
    public:
        void Start( )override;
    };

    class GlobalData
    {
        static GlobalTexture textureHolder;
        static GlobalAudio audioHolder;
        static GlobalObj objHolder;
    private:
        GlobalData( ) { /*nothing*/ }
    public:
        static void Setup( std::string const& appPath );
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