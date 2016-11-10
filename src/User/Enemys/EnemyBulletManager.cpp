# include "EnemyBulletManager.h"

# include "GlobalData.hpp"
# include "../Utilitys/GlobalDraw.h"

namespace User
{
    using namespace cinder;

    EnemyBulletManager::EnemyBulletManager( )
    {
        guard_se = &GData::FindAudio( "SE/guard.wav" );
        playerdamaged_se = &GData::FindAudio( "SE/damage.wav" );
        adddamage = &GData::FindAudio( "SE/adddamage.wav" );
    }

    void EnemyBulletManager::update( )
    {
        Each( [ ] ( EnemyBulletBaseRef& bulletRef ) { bulletRef->update( ); } );
        BulletEraser( );
    }

    void EnemyBulletManager::draw( cinder::CameraPersp const& camera )
    {
        for ( auto itr = bulletList.begin( ); itr != bulletList.end( ); ++itr )
        {
            GlobalDraw::InsertAlphaObject( std::make_pair( camera.worldToEyeDepth( ( *itr )->Position( ) ), std::bind( &EnemyBulletBase::draw, *itr ) ) );
        }
    }

    void EnemyBulletManager::BulletRegister( EnemyBulletList& bulletList )
    {
        this->bulletList.splice( this->bulletList.end( ), bulletList );
    }

    int EnemyBulletManager::PlayerToEnemyDamage( Line & line_, const cinder::CameraPersp & camera )
    {
        int drainMp = 0;
        Each( [ &drainMp, &line_, &camera ] ( EnemyBulletBaseRef& bulletRef )
        {
            Vec2f vec = camera.worldToScreen( bulletRef->Position( ), env.getWindowWidth( ), env.getWindowHeight( ) );
            Vec2f size = camera.worldToScreen( bulletRef->Position( ) + bulletRef->Size( ), env.getWindowWidth( ), env.getWindowHeight( ) );
            float radius = Vec3f( size - vec ).length( ) / 2.0F;
            drainMp += bulletRef->Hit( CheckDefLineOfCircle( line_, vec, radius ) );
        } );
        if ( drainMp != 0 )
            adddamage->Play( );
        score += drainMp * 100;
        return drainMp;
    }

    int EnemyBulletManager::PlayerSpecialAttackToEnemyDamage( )
    {
        int drainMp = 0;
        Each( [ &drainMp ] ( EnemyBulletBaseRef& bulletRef )
        {
            drainMp += bulletRef->Kill( );
        } );
        score += drainMp * 100;
        return drainMp;
    }

    int EnemyBulletManager::EnemyToPlayerDamage( const cinder::CameraPersp& camera )
    {
        int damage = 0;
        Each( [ &damage, &camera, this ] ( EnemyBulletBaseRef& bulletRef )
        {
            if ( bulletRef->Attack( camera ) == true )
            {
                damage += bulletRef->AttackPoint( );
                bulletRef->Erase( );
                playerdamaged_se->Play( );
            }
        } );
        return damage;
    }

    int EnemyBulletManager::EnemyToPlayerDamage( Line & line_, const cinder::CameraPersp & camera )
    {
        int damage = 0;
        Each( [ &damage, &line_, &camera, this ] ( EnemyBulletBaseRef& bulletRef )
        {
            if ( bulletRef->Attack( camera ) )
            {
                Vec2f vec = camera.worldToScreen( bulletRef->Position( ), env.getWindowWidth( ), env.getWindowHeight( ) );
                Vec2f size = camera.worldToScreen( bulletRef->Position( ) + bulletRef->Size( ), env.getWindowWidth( ), env.getWindowHeight( ) );
                float radius = Vec3f( size - vec ).length( ) / 2.0F;
                if ( CheckDefLineOfCircle( line_, vec, radius + 50 ) > 1.0f ) {
                    damage += bulletRef->AttackPoint( );
                    if ( bulletRef->NormalizedMoveTime( ) == 1 ) {
                        playerdamaged_se->Play( );
                    }
                }
                else {
                    if ( bulletRef->NormalizedMoveTime( ) == 1 ) {
                        guard_se->Play( );
                        Vec2f enemypos = camera.worldToScreen( bulletRef->Position( ), env.getWindowWidth( ), env.getWindowHeight( ) );
                        float a = ( line_.startPos.y - line_.endPos.y ) / ( line_.startPos.x - line_.endPos.x );
                        float b = line_.startPos.y - a * line_.startPos.x;
                        float pos_x = ( a*( enemypos.y - b ) + enemypos.x ) / ( ( a*a ) + 1 );
                        float pos_y = a*( a*( enemypos.y - b ) + enemypos.x ) / ( ( a*a ) + 1 ) + b;
                        EffectCreate( EffectBase( "Textures/Effect/guard3.png",
                                                  Vec2f( pos_x, pos_y ),
                                                  Vec2f( 240, 240 ),
                                                  Vec2f( 480, 480 ),
                                                  EffectBase::Mode::CENTERCENTER, true
                        ) );
                    }
                }

                bulletRef->Erase( );
            }
        } );
        return damage;
    }

    void EnemyBulletManager::DrawCollisionCircle( cinder::CameraPersp const & camera )
    {
        Each( [ &camera ] ( EnemyBulletBaseRef& bulletRef )
        {
            Vec2f vec = camera.worldToScreen( bulletRef->Position( ), env.getWindowWidth( ), env.getWindowHeight( ) );
            Vec2f size = camera.worldToScreen( bulletRef->Position( ) + bulletRef->Size( ), env.getWindowWidth( ), env.getWindowHeight( ) );
            float radius = Vec3f( size - vec ).length( ) / 2.0F;

            gl::color( ColorA( 1, 1, 1, 0.25F ) );
            gl::drawSolidCircle( vec, radius, radius );
        } );
    }

    void EnemyBulletManager::DrawBulletCircle( cinder::CameraPersp const & camera )
    {
        Each( [ &camera, this ] ( EnemyBulletBaseRef& bulletRef )
        {
            Vec2f vec = camera.worldToScreen( bulletRef->EndPosition( ), env.getWindowWidth( ), env.getWindowHeight( ) );
            Vec2f size = camera.worldToScreen( bulletRef->EndPosition( ) + bulletRef->Size( ), env.getWindowWidth( ), env.getWindowHeight( ) );
            float radius = Vec3f( size - vec ).length( ) / 2.0F * colliedSize;

            gl::color( ColorA( 1, 0, 0, 0.5F ) );
            gl::drawSolidCircle( vec, radius, radius );
            gl::color( ColorA( 1, 1, 0, 1.0F ) );
            float time = 1.8 - 0.8 * bulletRef->NormalizedMoveTime( );
            gl::drawStrokedCircle( vec, radius * time, radius * time );
        } );
    }

    void EnemyBulletManager::Each( std::function<void( EnemyBulletBaseRef& )> func )
    {
        std::for_each( bulletList.begin( ), bulletList.end( ), func );
    }

    void EnemyBulletManager::BulletEraser( )
    {
        auto eraceList = std::remove_if( bulletList.begin( ), bulletList.end( ), [ ] ( EnemyBulletBaseRef& bulletRef ) { return !bulletRef->IsActive( ); } );
        bulletList.erase( eraceList, bulletList.end( ) );
    }

    EffectList EnemyBulletManager::EffectRecovery( )
    {
        auto ret = effectList;
        effectList.clear( );
        return ret;
    }
}