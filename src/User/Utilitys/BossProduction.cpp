#include "BossProduction.h"

#include <algorithm>

#include "Framework.hpp"

#include "Hirasawa.h"

#include "GlobalData.hpp"

#include "TutorialData.hpp"

namespace User
{
    using namespace cinder;

    Rect::Rect( )
        : in( 100 )
        , out( 100 )
    { }
    void Rect::End( )
    {
        isEnd = true;
    }
    bool Rect::IsEnd( )
    {
        return out.IsMax( );
    }
    void Rect::Update( )
    {
        if ( isEnd )
        {
            height = EasingBackOut( out.NormalizedRectSizeFrame( ), maxHeight, 0 );
            out.Update( );
        }
        else
        {
            height = EasingElasticOut( in.NormalizedRectSizeFrame( ), 0, maxHeight );
            in.Update( );
        }
    }
    void UpRect::Draw( )
    {
        gl::pushModelView( );

        gl::color( ColorA( 1, 0, 0, 0.5F ) );
        gl::drawSolidRect( Rectf( Vec2f::zero( ), Vec2f( env.getWindowWidth( ), height ) ) );

        gl::popModelView( );
    }
    void DownRect::Draw( )
    {
        gl::pushModelView( );

        gl::color( ColorA( 1, 0, 0, 0.5F ) );
        gl::drawSolidRect( Rectf( Vec2f( 0, env.getWindowHeight( ) ), Vec2f( env.getWindowWidth( ), env.getWindowHeight( ) - height ) ) );

        gl::popModelView( );
    }

    WarningRect::WarningRect( cinder::Vec2f position, cinder::Vec2f size )
        : position( position )
        , size( size )
        , offset_x( 0.0F )
    {
        warning = &GData::FindTexture( "warning.png" );
    }
    void WarningRect::Update( )
    {
        offset_x = std::fmodf( offset_x + 2.0F, size.x );
    }
    void WarningRect::Draw( )
    {
        auto offset = Vec2f( offset_x, 0 );
        DrawCutTexture( warning, position, size, offset, size );
    }
    void WarningRect::DrawCutTexture( cinder::gl::Texture* texture, cinder::Vec2f position, cinder::Vec2f size, cinder::Vec2f offset, cinder::Vec2f cutSize )
    {
        gl::pushModelView( );
        gl::translate( -offset + position );
        gl::color( Color::white( ) );
        gl::draw( *texture, Area( offset, offset + cutSize ), Area( offset, offset + size ) );
        gl::popModelView( );
    }
    WarningLine::WarningLine( cinder::Vec2f position, float angle )
        : position( position )
        , initPosition( position )
        , angle( angle )
        , out( 100 )
    {
        auto size = Vec2f( 200, 100 );

        float num = 8.0F * 2.0F;
        float num_2 = num / 2.0F;

        auto basePosition = Vec2f::zero( );
        basePosition.x -= size.x * num_2;
        basePosition.y -= size.y / 2.0F;

        for ( int i = 0; i < num; ++i )
        {
            line.emplace_back( basePosition, size );

            basePosition.x += size.x;
        }
    }
    void WarningLine::Update( )
    {
        if ( isEnd )
        {
            auto size = Vec2f( 200, 100 );
            float num = 8.0F * 2.0F;
            float num_2 = num / 2.0F;
            position.x = EasingBackOut( out.NormalizedRectSizeFrame( ), initPosition.x, -size.x * num_2 );
            out.Update( );
        }

        for ( auto& obj : line )
        {
            obj.Update( );
        }
    }
    void WarningLine::Draw( )
    {
        gl::pushModelView( );
        gl::translate( position );
        gl::rotate( angle );
        for ( auto& obj : line )
        {
            obj.Draw( );
        }
        gl::popModelView( );
    }

    void WarningLine::End( )
    {
        isEnd = true;
    }

    bool WarningLine::IsEnd( )
    {
        return out.IsMax( );
    }

    BossSpawnMoji::BossSpawnMoji( )
        : font( u8"HGs‘‘Ì", 300 )
        , out( 100 )
        , position( Vec2f::zero( ) )
        , initPosition( position )
    {
    }
    void BossSpawnMoji::Update( )
    {
        if ( isEnd )
        {
            position.x = EasingBackOut( out.NormalizedRectSizeFrame( ), initPosition.x, env.getWindowWidth( ) );
            out.Update( );
        }
    }
    void BossSpawnMoji::Draw( )
    {
        float offset;
        auto tex = font.MakeStringTexture( u8"e‹Êow", offset );
        tex.unbind( );
        auto size = Vec2f( tex.getSize( ).x, tex.getSize( ).y );

        gl::pushModelView( );
        gl::translate( position );
        gl::color( ColorA( 0, 0, 0, 0.5F ) );
        gl::drawSolidRect( Rectf( Vec2f( 0, 200 + 100 ), Vec2f( env.getWindowWidth( ), env.getWindowHeight( ) - 200 - 100 ) ) );
        gl::color( Color( 1, 0, 0 ) );
        gl::draw( tex, Rectf( Vec2f( 0, 200 + 100 ), Vec2f( env.getWindowWidth( ), env.getWindowHeight( ) - 200 - 100 ) ) );
        gl::popModelView( );
    }

    void BossSpawnMoji::End( )
    {
        isEnd = true;
    }

    bool BossSpawnMoji::IsEnd( )
    {
        return out.IsMax( );
    }

    BossWarning::BossWarning( )
    {
        se = &GData::FindAudio( "SE/bossSpawn.wav" );
    }

    void BossWarning::Start( )
    {
        isStart = true;
        se->Play( );
    }

    void BossWarning::Update( )
    {
        if ( !isStart ) return;
        if ( sePlayNum <= 0 ) return;
        if ( !se->IsPlaying( ) )
        {
            se->Play( );
            sePlayNum = std::max( sePlayNum - 1, 0 );
        }
    }

    bool BossWarning::First( )
    {
        return isStart && sePlayNum <= 2;
    }

    bool BossWarning::Second( )
    {
        return isStart && sePlayNum <= 1;
    }

    bool BossWarning::Thread( )
    {
        return isStart && sePlayNum <= 0;
    }

    bool BossWarning::IsEnd( )
    {
        return sePlayNum <= 0 && !se->IsPlaying( );
    }


    BossProduction::BossProduction( )
        : upLine( Vec2f( env.getWindowWidth( ) / 2.0F, 200 + 50 ), 0.0F )
        , downLine( Vec2f( env.getWindowWidth( ) / 2.0F, env.getWindowHeight( ) - 200 - 50 ), 0.0F )
    {

    }
    bool BossProduction::IsMainBGMGainDown( )
    {
        return !isEnd && isStart;
    }
    bool BossProduction::IsMainBGMStop( )
    {
        return bossWarning.Second( );
    }
    bool BossProduction::IsBossBGMStart( )
    {
        return bossWarning.Thread( );
    }
    void BossProduction::Start( )
    {
        isStart = true;
        bossWarning.Start( );

    }
    void BossProduction::Update( )
    {
        if ( isEnd ) return;
        if ( !isStart ) return;

        bossWarning.Update( );
        upRect.Update( );
        downRect.Update( );

        if ( bossWarning.Second( ) )
        {
            upLine.Update( );
            downLine.Update( );
            spawnMoji.Update( );
        }

        if ( bossWarning.Thread( ) )
        {
            upRect.End( );
            downRect.End( );
            upLine.End( );
            downLine.End( );
            spawnMoji.End( );
        }

        if ( upLine.IsEnd( ) && downLine.IsEnd( ) && spawnMoji.IsEnd( ) )
        {
            isEnd = true;
            TRData::bossSpawn.TutorialEnd( );
        }
    }
    void BossProduction::Draw( )
    {
        if ( isEnd ) return;
        if ( !isStart ) return;

        upRect.Draw( );
        downRect.Draw( );

        if ( bossWarning.Second( ) )
        {
            upLine.Draw( );
            downLine.Draw( );
            spawnMoji.Draw( );
        }

        if ( bossWarning.Thread( ) )
        {

        }
    }
}