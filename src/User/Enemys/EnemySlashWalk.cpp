# include "EnemySlashWalk.h"

namespace User
{
    using namespace cinder;

    EnemySlashWalk::EnemySlashWalk( cinder::Vec3f pos, const cinder::CameraPersp & camera )
        : EnemyBase( pos, camera, Status( 1.0F, 0 ) )
    {
        待機 = &GData::FindTexture( "Enemy/SlashWalk/SlashWalk (1).png" );


        左に移動 = &GData::FindTexture( "Enemy/SlashWalk/SlashWalk (4).png" );
        右に移動 = &GData::FindTexture( "Enemy/SlashWalk/SlashWalk (5).png" );
        knockBackTexture = &GData::FindTexture( "Enemy/SlashWalk/SlashWalk (6).png" );

        texture = 待機;

        SetFunction( &EnemySlashWalk::タイマーが鳴るまで待機 );
    }

    void EnemySlashWalk::update( cinder::CameraPersp const& camera )
    {
        if ( IsLive( ) )
        {
            // ノックバック時は一時的に処理をしません。
            if ( !IsKnockBack( ) )
            {
                // メイン処理
                behavior( camera );

                // タイマー処理
                timer.Update( );
            }
        }
        else
        {
            texture = knockBackTexture;
        }

        EnemyBase::update( camera );
    }
    void EnemySlashWalk::タイマーが鳴るまで待機( cinder::CameraPersp const& camera )
    {
        // タイマーが鳴ったら次の関数へ。
        if ( timer.IsAction( ) )
        {
            timer.Advance( randInt( 30, 60 ) );
            if ( randBool( ) )
            {
                texture = 左に移動;
                SetFunction( &EnemySlashWalk::左へ移動 );
            }
            else
            {
                texture = 右に移動;
                SetFunction( &EnemySlashWalk::右へ移動 );
            }
        }
    }
    void EnemySlashWalk::左へ移動( cinder::CameraPersp const & camera )
    {
        auto direction = object.Direction( );
        direction.rotateY( M_PI / 2.0 );
        object.PositionAdd( direction * 0.05 );

        if ( !IsInField( ) )
        {
            object.PositionAdd( -direction * 0.05 );

            // タイマーをセットしてまた待機状態にします。
            timer.Advance( randInt( 20, 30 ) );
            texture = 右に移動;
            SetFunction( &EnemySlashWalk::右へ移動 );
        }

        if ( timer.IsAction( ) )
        {
            // タイマーをセットしてまた待機状態にします。
            timer.Advance( randInt( 60, 180 ) );
            texture = 待機;
            SetFunction( &EnemySlashWalk::タイマーが鳴るまで待機 );
        }
    }
    void EnemySlashWalk::右へ移動( cinder::CameraPersp const & camera )
    {
        auto direction = object.Direction( );
        direction.rotateY( -M_PI / 2.0 );
        object.PositionAdd( direction * 0.05 );

        if ( !IsInField( ) )
        {
            object.PositionAdd( -direction * 0.05 );

            // タイマーをセットしてまた待機状態にします。
            timer.Advance( randInt( 20, 30 ) );
            texture = 左に移動;
            SetFunction( &EnemySlashWalk::左へ移動 );
        }

        if ( timer.IsAction( ) )
        {
            // タイマーをセットしてまた待機状態にします。
            timer.Advance( randInt( 60, 180 ) );
            texture = 待機;
            SetFunction( &EnemySlashWalk::タイマーが鳴るまで待機 );
        }
    }
    void EnemySlashWalk::SetFunction( void( EnemySlashWalk::* function )( cinder::CameraPersp const &camera ) )
    {
        behavior = std::bind( function, this, std::placeholders::_1 );
    }
}