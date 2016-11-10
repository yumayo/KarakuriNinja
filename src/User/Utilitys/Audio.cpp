#include "Audio.h"

#include "cinder/gl/gl.h"

#include "cinder/Rand.h"

# include "cinder/app/App.h"

namespace User
{
    using namespace cinder;

    Audio::Audio( const std::string & filePath, const float initialValue )
    {
        auto ctx = audio::master( );

        ctx->enable( );

        gainRef = ctx->makeNode( new audio::GainNode( initialValue ) );

        bufferPlayerRef = ctx->makeNode( new audio::BufferPlayerNode( ) );
        bufferPlayerRef->loadBuffer( audio::load( app::loadAsset( filePath ) ) );

        bufferPlayerRef >> gainRef >> ctx->getOutput( );
    }
    Audio::~Audio( )
    {
        Stop( );
    }
    void Audio::Play( )
    {
        bufferPlayerRef->start( );
    }
    void Audio::Stop( )
    {
        bufferPlayerRef->stop( );
    }
    bool Audio::IsPlaying( )
    {
        return bufferPlayerRef->isEnabled( );
    }
    void Audio::Looping( const bool isLooping )
    {
        bufferPlayerRef->setLoopEnabled( isLooping );
    }
    void Audio::Gain( const float gainSize )
    {
        gainRef->setValue( gainSize );
    }
    void Audio::SetLoopBeginTime( const double positionSeconds )
    {
        bufferPlayerRef->setLoopBeginTime( positionSeconds );
    }
    void Audio::SetLoopEndTime( const double positionSeconds )
    {
        bufferPlayerRef->setLoopEndTime( positionSeconds );
    }
    double Audio::GetLoopBeginTime( ) const
    {
        return bufferPlayerRef->getLoopBeginTime( );
    }
    double Audio::GetLoopEndTime( ) const
    {
        return bufferPlayerRef->getLoopEndTime( );
    }
    void Audio::SeekToTime( const double positionSeconds )
    {
        bufferPlayerRef->seekToTime( positionSeconds );
    }
}