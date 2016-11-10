#include "GlobalData.h"

#include "fileUtil.hpp"
#include "cinder/ImageIo.h"
#include "cinder/app/App.h"
#include "cinder/ObjLoader.h"

namespace User
{
    using namespace cinder;

    GlobalTexture::GlobalTexture( )
        : search( "..\\assets\\TEXTURE" )
        , index( 0 )
    {
        /*nothing*/
    }
    void GlobalTexture::Start( )
    {
        if ( IsSetuped( ) ) return;

        auto& files = search.Files( );

        for ( int count = 0; index < files.size( ) && count < 1; ++index, ++count )
        {
            auto extension = getFilenameExt( files[index] );
            if ( extension == "png" ||
                 extension == "jpeg" ||
                 extension == "jpg" ||
                 extension == "bmp"
                 )
            {
                datas.add( files[index], loadImage( app::loadAsset( "TEXTURE/" + files[index] ) ) );
            }
        }
    }
    bool GlobalTexture::IsSetuped( )
    {
        return index == MaxLoadIndex( );
    }
    size_t GlobalTexture::MaxLoadIndex( )
    {
        auto& files = search.Files( );
        return files.size( );
    }
    size_t GlobalTexture::NowLoadIndex( )
    {
        return index;
    }

    GlobalAudio::GlobalAudio( )
        : search( "..\\assets\\AUDIO" )
        , index( 0 )
    {
        /*nothing*/
    }
    void GlobalAudio::Start( )
    {
        if ( IsSetuped( ) ) return;

        auto& files = search.Files( );

        for ( int count = 0; index < files.size( ) && count < 1; ++index, ++count )
        {
            auto extension = getFilenameExt( files[index] );
            if ( extension == "wav" ||
                 extension == "mp3"
                 )
            {
                datas.add( files[index], Audio( "AUDIO/" + files[index] ) );
            }
        }
    }
    bool GlobalAudio::IsSetuped( )
    {
        return index == MaxLoadIndex( );
    }

    size_t GlobalAudio::MaxLoadIndex( )
    {
        auto& files = search.Files( );
        return files.size( );
    }

    size_t GlobalAudio::NowLoadIndex( )
    {
        return index;
    }

    GlobalObj::GlobalObj( )
        : search( "..\\assets\\Obj" )
        , index( 0 )
    {
    }
    void GlobalObj::Start( )
    {
        if ( IsSetuped( ) ) return;

        auto& files = search.Files( );

        for ( int count = 0; index < files.size( ) && count < 1; ++index, ++count )
        {
            auto extension = getFilenameExt( files[index] );
            if ( extension == "obj"
                 )
            {
                TriMesh mesh;
                ObjLoader loader( app::loadAsset( "Obj/" + files[index] ) );
                loader.load( &mesh,
                             boost::logic::indeterminate, boost::logic::indeterminate,
                             false );
                datas.add( files[index], mesh );
            }
        }
    }
    bool GlobalObj::IsSetuped( )
    {
        return index == MaxLoadIndex( );
    }
    size_t GlobalObj::MaxLoadIndex( )
    {
        auto& files = search.Files( );
        return files.size( );
    }
    size_t GlobalObj::NowLoadIndex( )
    {
        return index;
    }

    GlobalTexture GlobalData::textureHolder;
    GlobalAudio GlobalData::audioHolder;
    GlobalObj GlobalData::objHolder;

    void GlobalData::Start( )
    {
        textureHolder.Start( );
        audioHolder.Start( );
        objHolder.Start( );
    }
    bool GlobalData::IsSetuped( )
    {
        return textureHolder.IsSetuped( ) && audioHolder.IsSetuped( ) && objHolder.IsSetuped( );
    }
    size_t GlobalData::MaxLoadIndex( )
    {
        return textureHolder.MaxLoadIndex( ) + audioHolder.MaxLoadIndex( ) + objHolder.MaxLoadIndex( );
    }
    size_t GlobalData::NowLoadIndex( )
    {
        return textureHolder.NowLoadIndex( ) + audioHolder.NowLoadIndex( ) + objHolder.NowLoadIndex( );
    }
    double GlobalData::NormalizedLoadIndex( )
    {
        return static_cast<double>( NowLoadIndex( ) ) / MaxLoadIndex( );
    }
    cinder::gl::Texture& GlobalData::FindTexture( std::string const& path )
    {
        auto ptr = textureHolder.datas.find( path );
        if ( ptr == nullptr )
        {
            return *textureHolder.datas.find( "SYSTEMTEXTURENULL.png" );
        }
        else
        {
            return *ptr;
        }
    }
    Audio & GlobalData::FindAudio( std::string const & path )
    {
        auto ptr = audioHolder.datas.find( path );
        if ( ptr == nullptr )
        {
            return *audioHolder.datas.find( "SYSTEMAUDIONULL.wav" );
        }
        else
        {
            return *ptr;
        }
    }

    cinder::TriMesh & GlobalData::FindObj( std::string const & path )
    {
        auto ptr = objHolder.datas.find( path );
        if ( ptr == nullptr )
        {
            return *objHolder.datas.find( "SYSTEMOBJNULL.obj" );
        }
        else
        {
            return *ptr;
        }
    }

}