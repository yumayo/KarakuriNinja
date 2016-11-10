#include "TutorialData.h"

#include "cinder/Json.h"

namespace User
{
    using namespace cinder;

    std::list<std::string> TutorialData::talkString;
    TutorialAttackCircle TutorialData::attackCircle;
    TutorialGuard TutorialData::guard;
    TutorialSpawn TutorialData::spawn;
    TutorialPlayerAttack TutorialData::playerAttack;
    TutorialEnemyKill TutorialData::enemyKill;
    TutorialSpecial TutorialData::special;

    bool TutorialData::IsStopUpdate( )
    {
        return
            attackCircle.IsStopUpdate( ) ||
            guard.IsStopUpdate( ) ||
            spawn.IsStopUpdate( ) ||
            playerAttack.IsStopUpdate( ) ||
            enemyKill.IsStopUpdate( ) ||
            special.IsStopUpdate( );
    }
    void TutorialData::Reset( )
    {
        attackCircle.TutorialReset( );
        guard.TutorialReset( );
        spawn.TutorialReset( );
        playerAttack.TutorialReset( );
        enemyKill.TutorialReset( );
        special.TutorialReset( );
    }
    std::string TutorialData::Serif( )
    {
        if ( talkString.empty( ) ) return std::string( );
        return talkString.front( );
    }
    void TutorialData::PopFrontSerif( )
    {
        if ( talkString.empty( ) ) return;
        talkString.pop_front( );
    }
    bool TutorialData::IsSerifTalked( )
    {
        return talkString.empty( );
    }
    void TutorialData::SerifRead( std::string const & jsonPath )
    {
        talkString.clear( );
        JsonTree params( app::loadAsset( jsonPath ) );

        for ( auto& obj : params["Serif"] )
        {
            talkString.emplace_back( obj.getValue( ) );
        }
    }
    void TutorialGuard::TutorialStart( )
    {
        TutorialBase::TutorialStart( );
        TutorialData::SerifRead( "JSON/Tutorial/Guard.json" );
    }
    void TutorialGuard::Draw( )
    {
        static bool init = true;
        if ( init )
        {
            font = std::make_shared<Fonts>( u8"ƒƒCƒŠƒI", 380 );
            init = false;
        }

        if ( successFrame == 0 )
        {
            /*nothing*/
        }
        else
        {
            font->Draw( std::to_string( maxSuccessFrame / 60 - successFrame / 60 ), env.getWindowCenter( ) + cinder::Vec2f( 0, -380 / 2 ), cinder::Color::white( ), Fonts::Mode::CENTERUP );
        }
    }
    void TutorialAttackCircle::TutorialStart( )
    {
        TutorialBase::TutorialStart( );
        TutorialData::SerifRead( "JSON/Tutorial/AttackCircle.json" );
    }
    void TutorialSpawn::TutorialStart( )
    {
        TutorialBase::TutorialStart( );
        TutorialData::SerifRead( "JSON/Tutorial/Spawn.json" );
    }
    void TutorialPlayerAttack::TutorialStart( )
    {
        TutorialBase::TutorialStart( );
        TutorialData::SerifRead( "JSON/Tutorial/PlayerAttack.json" );
    }
    void TutorialSpecial::TutorialStart( )
    {
        TutorialBase::TutorialStart( );
        TutorialData::SerifRead( "JSON/Tutorial/Special.json" );
    }
    void TutorialEnemyKill::TutorialStart( )
    {
        TutorialBase::TutorialStart( );
        TutorialData::SerifRead( "JSON/Tutorial/EnemyKill.json" );
    }
}