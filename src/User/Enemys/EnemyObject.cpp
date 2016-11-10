# include "EnemyObject.h"

namespace User
{
    using namespace cinder;

    void EnemyObject::Direction( cinder::Vec3f direction )
    {
        this->direction = direction.normalized( );
        quaternion = Quatf( Vec3f::zAxis( ), this->direction );
    }

    void EnemyObject::Target( cinder::Vec3f targetPosition )
    {
        // ホーミングの計算
        // TIPS:クオータニオンと外積を使えばごく簡単に計算できる
        Vec3f d = direction - pos;
        // 目標までの向きベクトルを正規化
        d.normalize( );
        // 自分の向きベクトルと目標までの向きベクトルの外積
        // →クオータニオンの回転軸
        Vec3f cross = this->direction.cross( d );
        if ( cross.lengthSquared( ) > 0.0f ) {
            // FIXME:外積で求めたベクトルの長さが角度差になるのだが、
            //       角度を表しているわけではないので微妙
            Quatf rotate( cross.normalized( ), cross.length( ) );

            // 回転を表すクオータニオンを再計算
            // TIPS:行列と同じく、掛け算で向きが積算させる
            quaternion = quaternion * rotate;
            // 向きベクトルを再計算
            this->direction = quaternion * Vec3f::zAxis( );
        }
    }
}