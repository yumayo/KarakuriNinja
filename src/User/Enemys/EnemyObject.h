#pragma once

#include "cinder/Vector.h"
#include "cinder/Quaternion.h"

namespace User
{
    class EnemyObject
    {
    public:
        EnemyObject( )
            : pos( cinder::Vec3f::zero( ) ), size( cinder::Vec3f::zero( ) ), speed( cinder::Vec3f::zero( ) ), radius( 0.0F )
        {
            Direction( cinder::Vec3f::zAxis( ) );
        }
        EnemyObject( cinder::Vec3f pos, cinder::Vec3f size )
            : pos( pos ), size( size ), speed( cinder::Vec3f::zero( ) ), radius( size.length( ) / 2.0F )
        {
            Direction( cinder::Vec3f::zAxis( ) );
        }
        EnemyObject( cinder::Vec3f pos, cinder::Vec3f size, cinder::Vec3f speed )
            : pos( pos ), size( size ), speed( speed ), radius( size.length( ) / 2.0F )
        {
            Direction( cinder::Vec3f::zAxis( ) );
        }
        EnemyObject( cinder::Vec3f pos, cinder::Vec3f size, cinder::Vec3f speed, float radius )
            : pos( pos ), size( size ), speed( speed ), radius( radius )
        {
            Direction( cinder::Vec3f::zAxis( ) );
        }
        EnemyObject( cinder::Vec3f pos, cinder::Vec3f size, cinder::Vec3f speed, cinder::Vec3f direction )
            : pos( pos ), size( size ), speed( speed ), radius( size.xy( ).length( ) / 2.0F )
        {
            Direction( direction );
        }
        EnemyObject( cinder::Vec3f pos, cinder::Vec3f size, cinder::Vec3f speed, float radius, cinder::Vec3f direction )
            : pos( pos ), size( size ), speed( speed ), radius( radius )
        {
            Direction( direction );
        }
        EnemyObject( const EnemyObject& other )
            : pos( other.pos ), size( other.size ), speed( other.speed ), radius( other.radius )
        {
            Direction( other.direction );
        }
    private:
        cinder::Vec3f pos;
        cinder::Vec3f size;
        cinder::Vec3f speed;
        float radius;
        cinder::Vec3f direction;
        cinder::Quatf quaternion;
    public: // ゲッター
        cinder::Vec3f Position( ) { return pos; }
        cinder::Vec3f Size( ) { return size; }
        cinder::Vec3f Speed( ) { return speed; }
        float Radius( ) { return radius; }
        cinder::Vec3f Direction( ) { return direction; }
        cinder::Quatf Quaternion( ) { return quaternion; }
    public: // セッターなどなど
        void Position( cinder::Vec3f pos ) { this->pos = pos; }
        void PositionAdd( cinder::Vec3f value ) { pos += value; }

        void Speed( cinder::Vec3f speed ) { this->speed = speed; }
        void SpeedAdd( cinder::Vec3f speed ) { this->speed += speed; }
        
        void Direction( cinder::Vec3f direction ) { this->direction = direction.normalized( ); quaternion = cinder::Quatf( cinder::Vec3f::zAxis( ), this->direction ); }
        void DirectionRotate( cinder::Vec3f axis, float radian ) { direction.rotate( axis, radian ); Direction( direction ); }
    };
}