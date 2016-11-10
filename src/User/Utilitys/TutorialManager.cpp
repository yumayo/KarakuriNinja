#include "TutorialManager.h"

#include "GlobalData.hpp"

namespace User
{
    using namespace cinder;

    Kougeki::Kougeki( )
    {
        UŒ‚‚Ì‹ÉˆÓ = &GData::FindTexture( "Textures/kougeki.png" );

        enemyDataPath.emplace_back( "JSON/TutorialEnemyData/Spawn1.json" );
        enemyDataPath.emplace_back( "JSON/TutorialEnemyData/Spawn2.json" );
    }
    void Kougeki::Draw( )
    {
        Vec2f size_2 = UŒ‚‚Ì‹ÉˆÓ->getSize( ) / 2.0F;
        gl::pushModelView( );
        gl::translate( env.getWindowWidth( ) / 2.0F, 150 );
        gl::color( Color::white( ) );
        gl::draw( *UŒ‚‚Ì‹ÉˆÓ, Rectf( -size_2, size_2 ) );
        gl::popModelView( );
    }
    bool Kougeki::IsNextStage( )
    {
        return enemyDataPath.empty( );
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
        : font( u8"HGs‘‘Ì", 48 )
    {
        guardSucceedNum = maxGuardSucceedNum;
        –hŒä‚Ì‹ÉˆÓ = &GData::FindTexture( "Textures/bougyo.png" );
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

        DrawRemainin( );
    }

    void Bougyo::DrawRemainin( )
    {
        gl::pushModelView( );
        gl::translate( 50, env.getWindowHeight( ) - 200 );
        font.Draw( u8"ƒeƒXƒg", Vec2f::zero( ), Color::white( ) );
        gl::popModelView( );
    }

    TutorialManager::TutorialManager( )
    {
        func = &TutorialManager::UŒ‚;
    }
    void TutorialManager::UŒ‚( )
    {
        kougeki.Draw( );

        Vec2f end( env.getWindowWidth( ) / 2 - 350, env.getWindowHeight( ) / 2 );
        Vec2f begin( env.getWindowWidth( ) / 2 + 350, env.getWindowHeight( ) / 2 );
        handAnimation.updateAttack( begin, end, Vec2f( 512, 512 ) );
        handAnimation.drawAttack( Vec2f( 600, 50 ) );

        description.SetMode( Description::KOUGEKI );
        description.Draw( Vec2f( env.getWindowWidth( ) - 768 - 50, env.getWindowHeight( ) - 256 - 50 ), Vec2f( 768, 256 ) );
    }
    void TutorialManager::–hŒä( )
    {
        bougyo.Draw( );

        Vec2f left( env.getWindowWidth( ) / 2 - 350, env.getWindowHeight( ) / 2 );
        Vec2f right( env.getWindowWidth( ) / 2 + 350, env.getWindowHeight( ) / 2 );
        handAnimation.updateGuard( left, right, Vec2f( 512, 512 ) );
        handAnimation.drawGuard( Vec2f( 600, 50 ) );

        description.SetMode( Description::BOUGYO );
        description.Draw( Vec2f( env.getWindowWidth( ) - 768 - 50, env.getWindowHeight( ) - 256 - 50 ), Vec2f( 768, 256 ) );
    }
    void TutorialManager::Set( Func func )
    {
        this->func = func;
    }
    void TutorialManager::Draw( )
    {
        ( this->*func )( );
    }

}