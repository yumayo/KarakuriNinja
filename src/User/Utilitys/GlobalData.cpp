#include "GlobalData.h"

#include "fileUtil.hpp"
#include "cinder/ImageIo.h"
#include "cinder/app/App.h"

namespace User
{
    using namespace cinder;

    GlobalTexture::GlobalTexture( )
        : search( "..\\assets" )
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
                 extension == "jpg"
                 )
            {
                datas.add( files[index], loadImage( app::loadAsset( files[index] ) ) );
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
        : search( "..\\assets" )
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
                datas.add( files[index], Audio( files[index] ) );
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

    GlobalTexture GlobalData::textureHolder;
    GlobalAudio GlobalData::audioHolder;
    GlobalData::GlobalData( )
    {
        /*nothing*/
    }
    void GlobalData::Start( )
    {
        textureHolder.Start( );
        audioHolder.Start( );
    }
    bool GlobalData::IsSetuped( )
    {
        return textureHolder.IsSetuped( ) && audioHolder.IsSetuped( );
    }
    size_t GlobalData::MaxLoadIndex( )
    {
        return textureHolder.MaxLoadIndex( ) + audioHolder.MaxLoadIndex( );
    }
    size_t GlobalData::NowLoadIndex( )
    {
        return textureHolder.NowLoadIndex( ) + audioHolder.NowLoadIndex( );
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
}