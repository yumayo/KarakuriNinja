#include "GlobalData.h"

#include "fileUtil.hpp"
#include "cinder/ImageIo.h"
#include "cinder/app/App.h"
#include "cinder/ObjLoader.h"

namespace User
{
    using namespace cinder;

    void GlobalTexture::Start( )
    {
        if ( IsSetuped( ) ) return;

        auto& files = search->Files( );

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

    void GlobalAudio::Start( )
    {
        if ( IsSetuped( ) ) return;

        auto& files = search->Files( );

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

    void GlobalObj::Start( )
    {
        if ( IsSetuped( ) ) return;

        auto& files = search->Files( );

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

    GlobalTexture GlobalData::textureHolder;
    GlobalAudio GlobalData::audioHolder;
    GlobalObj GlobalData::objHolder;

    void GlobalData::Setup( std::string const& appPath )
    {
        textureHolder.Setup( appPath + "..\\..\\assets\\TEXTURE" );
        audioHolder.Setup( appPath + "..\\..\\assets\\AUDIO" );
        objHolder.Setup( appPath + "..\\..\\assets\\OBJ" );
    }

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
        auto ptr = textureHolder.Datas( ).find( path );
        if ( ptr == nullptr )
        {
            return *textureHolder.Datas( ).find( "SYSTEMTEXTURENULL.png" );
        }
        else
        {
            return *ptr;
        }
    }
    Audio & GlobalData::FindAudio( std::string const & path )
    {
        auto ptr = audioHolder.Datas( ).find( path );
        if ( ptr == nullptr )
        {
            return *audioHolder.Datas( ).find( "SYSTEMAUDIONULL.wav" );
        }
        else
        {
            return *ptr;
        }
    }

    cinder::TriMesh & GlobalData::FindObj( std::string const & path )
    {
        auto ptr = objHolder.Datas( ).find( path );
        if ( ptr == nullptr )
        {
            return *objHolder.Datas( ).find( "SYSTEMOBJNULL.obj" );
        }
        else
        {
            return *ptr;
        }
    }
}