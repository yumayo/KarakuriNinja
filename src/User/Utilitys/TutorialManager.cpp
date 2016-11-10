#include "TutorialManager.h"

#include "GlobalData.hpp"

namespace User
{
    using namespace cinder;

    Kougeki::Kougeki( )
        : font( u8"HGs‘‘Ì", 128 )
    {
        UŒ‚‚Ì‹ÉˆÓ = &GData::FindTexture( "Textures/kougeki.png" );
        à–¾ = &GData::FindTexture( "setsumeiwaku.png" );
        ƒNƒŠƒA = &GData::FindAudio( "SE/se_maoudamashii_onepoint22.wav" );
        for ( int i = 0; i < 4; ++i )
        {
            suuji.emplace_back( &GData::FindTexture( "UI/suuji (" + std::to_string( i ) + ").png" ) );
        }
        enemyDataPath.emplace_back( "JSON/TutorialEnemyData/Spawn1.json" );
        enemyDataPath.emplace_back( "JSON/TutorialEnemyData/Spawn2.json" );
        enemyDataPath.emplace_back( "JSON/TutorialEnemyData/Spawn3.json" );

        ok = false;
    }
    void Kougeki::Draw( )
    {
        Vec2f size_2 = UŒ‚‚Ì‹ÉˆÓ->getSize( ) / 2.0F;
        gl::pushModelView( );
        gl::translate( env.getWindowWidth( ) / 2.0F, 150 );
        gl::color( Color::white( ) );
        gl::draw( *UŒ‚‚Ì‹ÉˆÓ, Rectf( -size_2, size_2 ) );
        gl::popModelView( );

        if ( ok )
        {
            DrawClear( );
        }
        else
        {
            DrawRemainin( );
        }
    }
    void Kougeki::DrawRemainin( )
    {
        auto num = enemyDataPath.size( ) + 1;
        float offset;
        auto leftStr = u8"Œã";
        auto rightStr = u8"‰ñ“G‚ğ“|‚¹I";
        auto leftTex = font.MakeStringTexture( leftStr, offset );
        auto rightTex = font.MakeStringTexture( rightStr, offset );
        auto size = Vec2f( leftTex.getWidth( ) + suuji[num]->getWidth( ) + rightTex.getWidth( ), leftTex.getHeight( ) - offset );

        gl::pushModelView( );
        gl::translate( 50, env.getWindowHeight( ) - 150 );

        gl::color( Color::white( ) );
        gl::draw( *à–¾, Rectf( Vec2f::zero( ), size ) );

        gl::color( Color::black( ) );
        gl::draw( leftTex, Rectf( Vec2f::zero( ), leftTex.getSize( ) ) );

        gl::translate( leftTex.getWidth( ), 0 );

        gl::pushModelView( );
        gl::translate( 0, size.y - suuji[num]->getHeight( ) );
        gl::color( Color::white( ) );
        gl::draw( *suuji[num], Rectf( Vec2f::zero( ), suuji[num]->getSize( ) ) );
        gl::popModelView( );

        gl::translate( suuji[num]->getWidth( ), 0 );

        gl::color( Color::black( ) );
        gl::draw( rightTex, Rectf( Vec2f::zero( ), rightTex.getSize( ) ) );

        gl::popModelView( );
    }
    void Kougeki::DrawClear( )
    {
        float offset;
        auto str = u8"‚æ‚­‚â‚Á‚½I";
        auto tex = font.MakeStringTexture( str, offset );
        auto size = Vec2f( tex.getWidth( ), tex.getHeight( ) - offset );

        gl::pushModelView( );
        gl::translate( 50, env.getWindowHeight( ) - 150 );

        gl::color( Color::white( ) );
        gl::draw( *à–¾, Rectf( Vec2f::zero( ), size ) );

        gl::color( Color::black( ) );
        gl::draw( tex, Rectf( Vec2f::zero( ), size ) );

        gl::popModelView( );
    }
    bool Kougeki::IsNextStage( )
    {
        return enemyDataPath.empty( );
    }
    void Kougeki::End( )
    {
        if ( ok ) return;
        ok = true;
        ƒNƒŠƒA->Play( );
    }
    std::string Kougeki::GetEnemyCreatePath( )
    {
        if ( !enemyDataPath.empty( ) )
        {
            auto ret = enemyDataPath.front( );
            enemyDataPath.pop_front( );
            return ret;
        }
        return "";
    }
    Bougyo::Bougyo( )
        : font( u8"HGs‘‘Ì", 128 )
    {
        –hŒä‚Ì‹ÉˆÓ = &GData::FindTexture( "Textures/bougyo.png" );
        à–¾ = &GData::FindTexture( "setsumeiwaku.png" );
        for ( int i = 0; i < 4; ++i )
        {
            suuji.emplace_back( &GData::FindTexture( "UI/suuji (" + std::to_string( i ) + ").png" ) );
        }
        ƒNƒŠƒA = &GData::FindAudio( "SE/se_maoudamashii_onepoint22.wav" );
        guardSucceedNum = maxGuardSucceedNum;

        ok = false;
    }

    void Bougyo::PlayerGuardSucceed( )
    {
        guardSucceedNum = std::max( guardSucceedNum - 1, 0 );
    }

    bool Bougyo::IsNextStage( )
    {
        return guardSucceedNum == 0;
    }

    void Bougyo::Draw( )
    {
        Vec2f size_2 = –hŒä‚Ì‹ÉˆÓ->getSize( ) / 2.0F;
        gl::pushModelView( );
        gl::translate( env.getWindowWidth( ) / 2.0F, 150 );
        gl::color( Color::white( ) );
        gl::draw( *–hŒä‚Ì‹ÉˆÓ, Rectf( -size_2, size_2 ) );
        gl::popModelView( );

        if ( ok )
        {
            DrawClear( );
        }
        else
        {
            DrawRemainin( );
        }
    }

    void Bougyo::DrawRemainin( )
    {
        auto num = guardSucceedNum;
        float offset;
        auto leftStr = u8"Œã";
        auto rightStr = u8"‰ñ–hŒä‚¹‚æI";
        auto leftTex = font.MakeStringTexture( leftStr, offset );
        auto rightTex = font.MakeStringTexture( rightStr, offset );
        auto size = Vec2f( leftTex.getWidth( ) + suuji[num]->getWidth( ) + rightTex.getWidth( ), leftTex.getHeight( ) - offset );

        gl::pushModelView( );
        gl::translate( 50, env.getWindowHeight( ) - 150 );

        gl::color( Color::white( ) );
        gl::draw( *à–¾, Rectf( Vec2f::zero( ), size ) );

        gl::color( Color::black( ) );
        gl::draw( leftTex, Rectf( Vec2f::zero( ), leftTex.getSize( ) ) );

        gl::translate( leftTex.getWidth( ), 0 );

        gl::pushModelView( );
        gl::translate( 0, size.y - suuji[num]->getHeight( ) );
        gl::color( Color::white( ) );
        gl::draw( *suuji[num], Rectf( Vec2f::zero( ), suuji[num]->getSize( ) ) );
        gl::popModelView( );

        gl::translate( suuji[num]->getWidth( ), 0 );

        gl::color( Color::black( ) );
        gl::draw( rightTex, Rectf( Vec2f::zero( ), rightTex.getSize( ) ) );

        gl::popModelView( );
    }
    void Bougyo::DrawClear( )
    {
        float offset;
        auto str = u8"‚æ‚­‚â‚Á‚½I";
        auto tex = font.MakeStringTexture( str, offset );
        auto size = Vec2f( tex.getWidth( ), tex.getHeight( ) - offset );

        gl::pushModelView( );
        gl::translate( 50, env.getWindowHeight( ) - 150 );

        gl::color( Color::white( ) );
        gl::draw( *à–¾, Rectf( Vec2f::zero( ), size ) );

        gl::color( Color::black( ) );
        gl::draw( tex, Rectf( Vec2f::zero( ), size ) );

        gl::popModelView( );
    }
    void Bougyo::End( )
    {
        if ( ok ) return;
        ok = true;
        ƒNƒŠƒA->Play( );
    }

    TutorialManager::TutorialManager( )
    {
        –â‘è’ñ‹N = &GData::FindAudio( "SE/se_maoudamashii_onepoint24.wav" );

        func = nullptr;
    }
    void TutorialManager::UŒ‚( )
    {
        kougeki.Draw( );

        Vec2f end( env.getWindowWidth( ) / 2 - 512, env.getWindowHeight( ) / 2 );
        Vec2f begin( env.getWindowWidth( ) / 2 + 512, env.getWindowHeight( ) / 2 );
        handAnimation.updateAttack( begin, end, Vec2f( 768, 768 ) );
        handAnimation.drawAttack( Vec2f( 600, 200 ) );

        //description.SetMode( Description::KOUGEKI );
        //description.Draw( Vec2f( env.getWindowWidth( ) - 768 - 50, env.getWindowHeight( ) - 256 - 50 ), Vec2f( 768, 256 ) );
    }
    void TutorialManager::–hŒä( )
    {
        bougyo.Draw( );

        Vec2f end( env.getWindowWidth( ) / 2 - 512, env.getWindowHeight( ) / 2 );
        Vec2f begin( env.getWindowWidth( ) / 2 + 512, env.getWindowHeight( ) / 2 );
        handAnimation.updateAttack( begin, end, Vec2f( 768, 768 ) );
        handAnimation.drawAttack( Vec2f( 600, 200 ) );

        //description.SetMode( Description::BOUGYO );
        //description.Draw( Vec2f( env.getWindowWidth( ) - 768 - 50, env.getWindowHeight( ) - 256 - 50 ), Vec2f( 768, 256 ) );
    }
    void TutorialManager::Set( Func func )
    {
        if ( this->func != func )
        {
            –â‘è’ñ‹N->Play( );
            this->func = func;
        }
    }
    void TutorialManager::Draw( )
    {
        ( this->*func )( );
    }
}