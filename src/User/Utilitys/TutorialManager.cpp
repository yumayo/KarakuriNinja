#include "TutorialManager.h"

#include "GlobalData.hpp"

namespace User
{
    using namespace cinder;

    Kougeki::Kougeki( )
        : font( u8"HG�s����", 128 )
    {
        �U���̋Ɉ� = &GData::FindTexture( "Textures/kougeki.png" );
        ���� = &GData::FindTexture( "setsumeiwaku.png" );
        �N���A = &GData::FindAudio( "SE/se_maoudamashii_onepoint22.wav" );
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
        Vec2f size_2 = �U���̋Ɉ�->getSize( ) / 2.0F;
        gl::pushModelView( );
        gl::translate( env.getWindowWidth( ) / 2.0F, 150 );
        gl::color( Color::white( ) );
        gl::draw( *�U���̋Ɉ�, Rectf( -size_2, size_2 ) );
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
        auto leftStr = u8"��";
        auto rightStr = u8"��G��|���I";
        auto leftTex = font.MakeStringTexture( leftStr, offset );
        auto rightTex = font.MakeStringTexture( rightStr, offset );
        auto size = Vec2f( leftTex.getWidth( ) + suuji[num]->getWidth( ) + rightTex.getWidth( ), leftTex.getHeight( ) - offset );

        gl::pushModelView( );
        gl::translate( 50, env.getWindowHeight( ) - 150 );

        gl::color( Color::white( ) );
        gl::draw( *����, Rectf( Vec2f::zero( ), size ) );

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
        auto str = u8"�悭������I";
        auto tex = font.MakeStringTexture( str, offset );
        auto size = Vec2f( tex.getWidth( ), tex.getHeight( ) - offset );

        gl::pushModelView( );
        gl::translate( 50, env.getWindowHeight( ) - 150 );

        gl::color( Color::white( ) );
        gl::draw( *����, Rectf( Vec2f::zero( ), size ) );

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
        �N���A->Play( );
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
        : font( u8"HG�s����", 128 )
    {
        �h��̋Ɉ� = &GData::FindTexture( "Textures/bougyo.png" );
        ���� = &GData::FindTexture( "setsumeiwaku.png" );
        for ( int i = 0; i < 4; ++i )
        {
            suuji.emplace_back( &GData::FindTexture( "UI/suuji (" + std::to_string( i ) + ").png" ) );
        }
        �N���A = &GData::FindAudio( "SE/se_maoudamashii_onepoint22.wav" );
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
        Vec2f size_2 = �h��̋Ɉ�->getSize( ) / 2.0F;
        gl::pushModelView( );
        gl::translate( env.getWindowWidth( ) / 2.0F, 150 );
        gl::color( Color::white( ) );
        gl::draw( *�h��̋Ɉ�, Rectf( -size_2, size_2 ) );
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
        auto leftStr = u8"��";
        auto rightStr = u8"��h�䂹��I";
        auto leftTex = font.MakeStringTexture( leftStr, offset );
        auto rightTex = font.MakeStringTexture( rightStr, offset );
        auto size = Vec2f( leftTex.getWidth( ) + suuji[num]->getWidth( ) + rightTex.getWidth( ), leftTex.getHeight( ) - offset );

        gl::pushModelView( );
        gl::translate( 50, env.getWindowHeight( ) - 150 );

        gl::color( Color::white( ) );
        gl::draw( *����, Rectf( Vec2f::zero( ), size ) );

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
        auto str = u8"�悭������I";
        auto tex = font.MakeStringTexture( str, offset );
        auto size = Vec2f( tex.getWidth( ), tex.getHeight( ) - offset );

        gl::pushModelView( );
        gl::translate( 50, env.getWindowHeight( ) - 150 );

        gl::color( Color::white( ) );
        gl::draw( *����, Rectf( Vec2f::zero( ), size ) );

        gl::color( Color::black( ) );
        gl::draw( tex, Rectf( Vec2f::zero( ), size ) );

        gl::popModelView( );
    }
    void Bougyo::End( )
    {
        if ( ok ) return;
        ok = true;
        �N���A->Play( );
    }

    TutorialManager::TutorialManager( )
    {
        ����N = &GData::FindAudio( "SE/se_maoudamashii_onepoint24.wav" );

        func = nullptr;
    }
    void TutorialManager::�U��( )
    {
        kougeki.Draw( );

        Vec2f end( env.getWindowWidth( ) / 2 - 512, env.getWindowHeight( ) / 2 );
        Vec2f begin( env.getWindowWidth( ) / 2 + 512, env.getWindowHeight( ) / 2 );
        handAnimation.updateAttack( begin, end, Vec2f( 768, 768 ) );
        handAnimation.drawAttack( Vec2f( 600, 200 ) );

        //description.SetMode( Description::KOUGEKI );
        //description.Draw( Vec2f( env.getWindowWidth( ) - 768 - 50, env.getWindowHeight( ) - 256 - 50 ), Vec2f( 768, 256 ) );
    }
    void TutorialManager::�h��( )
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
            ����N->Play( );
            this->func = func;
        }
    }
    void TutorialManager::Draw( )
    {
        ( this->*func )( );
    }
}