# pragma once

#include "cinder/audio/Context.h"
#include "cinder/audio/NodeEffects.h"
#include "cinder/audio/SamplePlayerNode.h"

namespace User
{
    class Audio
    {
    public:
        Audio( const std::string& filePath, const float initialValue = 1.0f );
        ~Audio( );
    public:
        void Play( );
        void Stop( );
        bool IsPlaying( );
        void Looping( const bool islooping );
        void Gain( const float gainSize );
    public:
        void SetLoopBeginTime( const double positionSeconds );
        void SetLoopEndTime( const double positionSeconds );
        double GetLoopBeginTime( ) const;
        double GetLoopEndTime( ) const;
        void SeekToTime( const double positionSeconds );
    private:
        cinder::audio::BufferPlayerNodeRef bufferPlayerRef;
        cinder::audio::GainNodeRef gainRef;
    };
}