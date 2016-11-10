#include "EnemyManager.h"

#include <algorithm>

#include "cinder/Rand.h"

#include "EnemyTutorial.h"
#include "EnemySlash.h"
#include "EnemyBullet.h"
#include "EnemyBoss.h"

#include "cinder/app/App.h"
#include "cinder/Json.h"

#include "../Utilitys/fileUtil.hpp"

#include "GlobalData.hpp"

# include "../Utilitys/GlobalDraw.h"

namespace User
{
    using namespace cinder;
    EnemyManager::EnemyManager( const cinder::CameraPersp& camera, std::string const& path )
    {
        JsonTree params( app::loadAsset( path ) );

        for ( auto& obj : params["Enemy"] )
        {
            if ( obj.getValue( ) == "Tutorial" )
            {
                Create<EnemyTutorial>( Vec3f( randFloat( -2.0F, 2.0F ), 0, randFloat( -2, 1 ) ), camera, getFilenameNoExt( path ) );
            }
            else if ( obj.getValue( ) == "Bullet" )
            {
                Create<EnemyBullet>( Vec3f( randFloat( -2.0F, 2.0F ), 2, randFloat( -2, 1 ) ), camera, getFilenameNoExt( path ) );
            }
            else if ( obj.getValue( ) == "Slash" )
            {
                Create<EnemySlash>( Vec3f( randFloat( -2.0F, 2.0F ), 0, randFloat( -2, 1 ) ), camera, getFilenameNoExt( path ) );
            }
            else if ( obj.getValue( ) == "Boss" )
            {
                Create<EnemyBoss>( Vec3f( randFloat( -2.0F, 2.0F ), 0, randFloat( -2, 1 ) ), camera, getFilenameNoExt( path ) );
            }
        }

        gurad_se = &GData::FindAudio( "SE/guard.wav" );
        playerdamaged_se = &GData::FindAudio( "SE/damage.wav" );
        adddamage = &GData::FindAudio( "SE/adddamage.wav" );
    }

    void EnemyManager::update( cinder::CameraPersp const& camera )
    {
        Each( [ &camera ] ( EnemyBaseRef& enemyRef ) { enemyRef->update( camera ); } );

        EnemyEraser( camera );

        EnemyBulletsRecovery( );

        EnemyEffectsRecovery( );
    }

    void EnemyManager::draw( cinder::CameraPersp const& camera )
    {
        for ( auto itr = enemyList.begin( ); itr != enemyList.end( ); ++itr )
        {
            GlobalDraw::InsertAlphaObject( std::make_pair( camera.worldToEyeDepth( ( *itr )->Position( ) ), std::bind( &EnemyBase::draw, *itr ) ) );
        }
    }

    void EnemyManager::drawUI( cinder::CameraPersp const& camera )
    {
        Each( [ camera ] ( EnemyBaseRef& enemyRef ) { enemyRef->drawUI( camera ); } );
    }

    bool EnemyManager::IsAttack( const cinder::CameraPersp& camera )
    {
        bool isAttack = false;
        Each( [ &isAttack, &camera ] ( EnemyBaseRef& enemyRef )
        {
            if ( enemyRef->Attack( camera ) == true )
                isAttack = true;
        } );
        return isAttack;
    }

    int EnemyManager::PlayerToEnemyDamage( Line& line_, const cinder::CameraPersp& camera, float value )
    {
        int drainMp = 0;
        Each( [ &drainMp, &line_, &camera, &value, this ] ( EnemyBaseRef& enemyRef )
        {
            Vec2f vec = camera.worldToScreen( enemyRef->Position( ), env.getWindowWidth( ), env.getWindowHeight( ) );
            Vec2f size = camera.worldToScreen( enemyRef->Position( ) + enemyRef->Size( ), env.getWindowWidth( ), env.getWindowHeight( ) );
            float radius = Vec2f( size - vec ).length( ) / 2.0F * playerAttackColliedSize;
            float length = CheckDefLineOfCircle( line_, vec, radius );
            drainMp += enemyRef->Hit( length, value );
            if ( length < 1.0F )
            {
                EffectCreate( EffectBase( "Textures/Effect/pipo-btleffect085.png",
                                          vec,
                                          Vec2f( vec - size ),
                                          Vec2f( 240, 240 ),
                                          EffectBase::Mode::CENTERCENTER, true
                ) );
            }
        } );
        if ( drainMp != 0 )
        {
            adddamage->Play( );
        }
        score += drainMp * 100;
        return drainMp;
    }

    int EnemyManager::PlayerToEnemyAttackCheck( Line & line_, const cinder::CameraPersp & camera )
    {
        std::vector<bool> success;
        Each( [ &success, &line_, &camera, this ] ( EnemyBaseRef& enemyRef )
        {
            Vec2f vec = camera.worldToScreen( enemyRef->Position( ), env.getWindowWidth( ), env.getWindowHeight( ) );
            Vec2f size = camera.worldToScreen( enemyRef->Position( ) + enemyRef->Size( ), env.getWindowWidth( ), env.getWindowHeight( ) );
            float radius = Vec2f( size - vec ).length( ) / 2.0F * enemyAttackColliedSize;

            success.emplace_back( CheckDefLineOfCircle( line_, vec, radius ) < 1.0F );
        } );

        int successNum = 0;
        for ( auto& obj : success )
        {
            successNum += static_cast<int>( obj );
        }

        return successNum;
    }

    int EnemyManager::PlayerSpecialAttackToEnemyDamage( int damage, const cinder::CameraPersp& camera, SpecialType specialState )
    {
        int drainMp = 0;
        Each( [ &drainMp, &damage, &camera, &specialState, this ] ( EnemyBaseRef& enemyRef )
        {
            Vec2f vec = camera.worldToScreen( enemyRef->Position( ), env.getWindowWidth( ), env.getWindowHeight( ) );
            Vec2f size = camera.worldToScreen( enemyRef->Position( ) + enemyRef->Size( ), env.getWindowWidth( ), env.getWindowHeight( ) );

            drainMp += enemyRef->Damage( damage );
            switch ( specialState )
            {
            case SpecialType::FIRE: // 火
                EffectCreate( EffectBase( "Textures/Effect/effect1.png",
                                          vec,
                                          Vec2f( vec - size ),
                                          Vec2f( 240, 240 ),
                                          EffectBase::Mode::CENTERCENTER
                ) );
                break;
            case SpecialType::WATER: // 水
                EffectCreate( EffectBase( "Textures/Effect/attack_blue.png",
                                          vec,
                                          Vec2f( vec - size ),
                                          Vec2f( 512, 512 ),
                                          EffectBase::Mode::CENTERCENTER
                ) );
                break;
            case SpecialType::TREE: // 木
                EffectCreate( EffectBase( "Textures/Effect/effect4.png",
                                          vec,
                                          Vec2f( vec - size ),
                                          Vec2f( 240, 240 ),
                                          EffectBase::Mode::CENTERCENTER
                ) );
                break;
            default:
                break;
            }
        } );
        score += drainMp * 100;
        return drainMp;
    }

    int EnemyManager::EnemyToPlayerDamage( const cinder::CameraPersp& camera )
    {
        int damage = 0;
        Each( [ &damage, &camera, this ] ( EnemyBaseRef& enemyRef )
        {
            if ( enemyRef->Attack( camera ) == true ) {
                damage += enemyRef->AttackPoint( );
                playerdamaged_se->Play( );
            }

        } );

        return damage;
    }

    int EnemyManager::EnemyToPlayerDamage( Line& line_, const cinder::CameraPersp& camera )
    {
        int totalDamage = 0;
        Each( [ &totalDamage, &line_, &camera, this ] ( EnemyBaseRef& enemyRef )
        {
            if ( enemyRef->Attack( camera ) )
            {
                Vec2f vec = camera.worldToScreen( enemyRef->Position( ), env.getWindowWidth( ), env.getWindowHeight( ) );
                Vec2f size = camera.worldToScreen( enemyRef->Position( ) + enemyRef->Size( ), env.getWindowWidth( ), env.getWindowHeight( ) );
                float radius = Vec2f( size - vec ).length( ) / 2.0F * enemyAttackColliedSize;
                if ( IsDamage( line_, vec, radius ) ) {
                    totalDamage += enemyRef->AttackPoint( );
                    playerdamaged_se->Play( );
                }
                else {
                    gurad_se->Play( );
                    Vec2f enemypos = camera.worldToScreen( enemyRef->Position( ), env.getWindowWidth( ), env.getWindowHeight( ) );
                    float a = ( line_.startPos.y - line_.endPos.y ) / ( line_.startPos.x - line_.endPos.x );
                    float b = line_.startPos.y - a * line_.startPos.x;
                    float pos_x = ( a*( enemypos.y - b ) + enemypos.x ) / ( ( a*a ) + 1 );
                    float pos_y = a*( a*( enemypos.y - b ) + enemypos.x ) / ( ( a*a ) + 1 ) + b;
                    EffectCreate( EffectBase( "Textures/Effect/guard3.png",
                                              Vec2f( pos_x, pos_y ),
                                              Vec2f( 480, 480 ),
                                              Vec2f( 480, 480 ),
                                              EffectBase::Mode::CENTERCENTER, true
                    ) );
                }
            }
        } );
        return totalDamage;
    }

    bool EnemyManager::EnemyToPlayerGuardCheck( Line & line_, const cinder::CameraPersp & camera )
    {
        std::vector<bool> success;
        Each( [ &success, &line_, &camera, this ] ( EnemyBaseRef& enemyRef )
        {
            if ( !enemyRef->IsAttackMotion( ) ) return;

            Vec2f vec = camera.worldToScreen( enemyRef->Position( ), env.getWindowWidth( ), env.getWindowHeight( ) );
            Vec2f size = camera.worldToScreen( enemyRef->Position( ) + enemyRef->Size( ), env.getWindowWidth( ), env.getWindowHeight( ) );
            float radius = Vec2f( size - vec ).length( ) / 2.0F * enemyAttackColliedSize;

            success.emplace_back( !IsDamage( line_, vec, radius ) );
        } );

        bool allSuccess = true;
        for ( auto& obj : success )
        {
            allSuccess = allSuccess && obj;
        }

        return allSuccess;
    }

    void EnemyManager::DrawAttackCircle( cinder::CameraPersp const & camera )
    {
        Each( [ &camera, this ] ( EnemyBaseRef& enemyRef )
        {
            if ( !enemyRef->IsAttackMotion( ) ) return;

            Vec2f vec = camera.worldToScreen( enemyRef->Position( ), env.getWindowWidth( ), env.getWindowHeight( ) );
            Vec2f size = camera.worldToScreen( enemyRef->Position( ) + enemyRef->Size( ), env.getWindowWidth( ), env.getWindowHeight( ) );
            float radius = Vec2f( size - vec ).length( ) / 2.0F * enemyAttackColliedSize;

            gl::color( ColorA( 1, 0, 0, 0.5F ) );
            gl::drawSolidCircle( vec, radius, radius );
            gl::color( ColorA( 1, 1, 0, 1.0F ) );
            float time = 1.8 - 0.8 * enemyRef->NormalizedAttackFrame( );
            gl::drawStrokedCircle( vec, radius * time, radius * time );
        } );
    }

    EnemyBulletList EnemyManager::BulletsRecovery( )
    {
        auto ret = bulletList;
        bulletList.clear( );
        return ret;
    }

    EffectList EnemyManager::EffectRecovery( )
    {
        auto ret = effectList;
        effectList.clear( );
        return ret;
    }

    void EnemyManager::DrawCollisionCircle( cinder::CameraPersp const & camera )
    {
        Each( [ &camera, this ] ( EnemyBaseRef& enemyRef )
        {
            Vec2f vec = camera.worldToScreen( enemyRef->Position( ), env.getWindowWidth( ), env.getWindowHeight( ) );
            Vec2f size = camera.worldToScreen( enemyRef->Position( ) + enemyRef->Size( ), env.getWindowWidth( ), env.getWindowHeight( ) );
            float radius = Vec2f( size - vec ).length( ) / 2.0F * playerAttackColliedSize;

            gl::color( ColorA( 1, 1, 1, 0.25F ) );
            gl::drawSolidCircle( vec, radius, radius );
        } );
    }

    bool EnemyManager::IsEmpty( )
    {
        return enemyList.empty( );
    }

    void EnemyManager::Each( std::function<void( EnemyBaseRef& )> function )
    {
        std::for_each( enemyList.begin( ), enemyList.end( ), function );
    }

    void EnemyManager::EnemyEraser( cinder::CameraPersp const& camera )
    {
        auto eraceList = std::remove_if( enemyList.begin( ), enemyList.end( ), [ &camera, this ] ( EnemyBaseRef& enemyRef )
        {
            if ( !enemyRef->IsActive( ) )
            {
                Vec2f vec = camera.worldToScreen( enemyRef->Position( ), env.getWindowWidth( ), env.getWindowHeight( ) );
                Vec2f size = camera.worldToScreen( enemyRef->Position( ) + enemyRef->Size( ), env.getWindowWidth( ), env.getWindowHeight( ) );
                EffectCreate( EffectBase( "Textures/Effect/pipo-btleffect102c.png",
                                          vec,
                                          Vec2f( vec - size ) * 3.0F,
                                          Vec2f( 240, 240 ),
                                          EffectBase::Mode::CENTERCENTER, true
                ) );

                score += 1000;
                return true;
            }
            return false;
        } );
        enemyList.erase( eraceList, enemyList.end( ) );
    }

    void EnemyManager::EnemyBulletsRecovery( )
    {
        Each( [ this ] ( EnemyBaseRef& enemyRef )
        {
            // spliceはlist同士をつなげるときに使います。
            // 以下のサイトを参考にしました。
            // http://d.hatena.ne.jp/pknight/20090814/1250227222
            bulletList.splice( bulletList.end( ), enemyRef->BulletsRecovery( ) );
        } );
    }
    void EnemyManager::EnemyEffectsRecovery( )
    {
        Each( [ this ] ( EnemyBaseRef& enemyRef )
        {
            effectList.splice( effectList.end( ), enemyRef->EffectRecovery( ) );
        } );
    }
    bool EnemyManager::IsDamage( Line line_, cinder::Vec2f pos_, float size_ )
    {
        return 1.0F < CheckDefLineOfCircle( line_, pos_, size_ );
    }
}