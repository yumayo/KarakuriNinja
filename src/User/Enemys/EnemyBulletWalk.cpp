# include "EnemyBulletWalk.h"

namespace User
{
    using namespace cinder;

    EnemyBulletWalk::EnemyBulletWalk( cinder::Vec3f pos, const cinder::CameraPersp & camera )
        : EnemyBase( pos, camera, Status( 1.0F, 0 ) )
    {
        待機 = &GData::FindTexture( "Enemy/BulletWalk/BulletWalk (1).png" );


        左に移動 = &GData::FindTexture( "Enemy/BulletWalk/BulletWalk (4).png" );
        右に移動 = &GData::FindTexture( "Enemy/BulletWalk/BulletWalk (5).png" );
        knockBackTexture = &GData::FindTexture( "Enemy/BulletWalk/BulletWalk (6).png" );

        texture = 待機;

        SetFunction( &EnemyBulletWalk::タイマーが鳴るまで待機 );
    }

    void EnemyBulletWalk::update( cinder::CameraPersp const& camera )
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
    void EnemyBulletWalk::タイマーが鳴るまで待機( cinder::CameraPersp const& camera )
    {
        // タイマーが鳴ったら次の関数へ。
        if ( timer.IsAction( ) )
        {
            timer.Advance( randInt( 30, 60 ) );
            if ( randBool( ) )
            {
                texture = 左に移動;
                SetFunction( &EnemyBulletWalk::左へ移動 );
            }
            else
            {
                texture = 右に移動;
                SetFunction( &EnemyBulletWalk::右へ移動 );
            }
        }
    }
    void EnemyBulletWalk::左へ移動( cinder::CameraPersp const & camera )
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
            SetFunction( &EnemyBulletWalk::右へ移動 );
        }

        if ( timer.IsAction( ) )
        {
            // タイマーをセットしてまた待機状態にします。
            timer.Advance( randInt( 60, 180 ) );
            texture = 待機;
            SetFunction( &EnemyBulletWalk::タイマーが鳴るまで待機 );
        }
    }
    void EnemyBulletWalk::右へ移動( cinder::CameraPersp const & camera )
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
            SetFunction( &EnemyBulletWalk::左へ移動 );
        }

        if ( timer.IsAction( ) )
        {
            // タイマーをセットしてまた待機状態にします。
            timer.Advance( randInt( 60, 180 ) );
            texture = 待機;
            SetFunction( &EnemyBulletWalk::タイマーが鳴るまで待機 );
        }
    }
    void EnemyBulletWalk::SetFunction( void( EnemyBulletWalk::* function )( cinder::CameraPersp const &camera ) )
    {
        behavior = std::bind( function, this, std::placeholders::_1 );
    }
}