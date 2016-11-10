# pragma once

# include "cinder/Vector.h"

namespace Utl
{
    // 2D専用の当たり判定
    // ・矩形とポイント
    // ・矩形と矩形
    // ・矩形と円
    // ・円とポイント
    // ・円と円
    // ・楕円とポイント
    class Colli2D
    {
    public:
        static bool rectPoint( const cinder::Vec2f& pos, const cinder::Vec2f& size, const cinder::Vec2f& otherPos );
        static bool rectRect( const cinder::Vec2f& pos, const cinder::Vec2f& size, const cinder::Vec2f& otherPos, const cinder::Vec2f& otherSize );
        static bool rectCircle( const cinder::Vec2f& pos, const cinder::Vec2f& size, const cinder::Vec2f& otherPos, const float& otherSize );
        static bool circlePoint( const cinder::Vec2f& pos, const float& size, const cinder::Vec2f& otherPos );
        static bool circleCircle( const cinder::Vec2f& pos, const float& size, const cinder::Vec2f& otherPos, const float& otherSize );
        static bool ellipsePoint( const cinder::Vec2f& pos, const cinder::Vec2f& size, const float& angle, const cinder::Vec2f& otherPos );
    private:
        Colli2D( ) {}
        ~Colli2D( ) {}
    };
}