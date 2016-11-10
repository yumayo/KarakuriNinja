#pragma once 
#include "cinder/Vector.h"
#include "Framework.hpp"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include"../Utilitys/Audio.h"
#include<vector>
#include"cinder\Rand.h"
//! @file SlashEffect 
//! @brief 動作で切る動きをしたときに 
//!　　　　 それに合わせてEffectの描画をします 
//! @note 
//!       大ちゃんがいじるところです 
//! @date 2016-07-12 
//! @author Taka Nomoto 

#include "GlobalData.hpp"


class Slash
{
public:
    Slash( int combo = 0 ) : isActive( false ), maxactiveTime( 30 ),
        activeTime( 0 )
    {
        float color_ = std::max(0.f,1-float(combo)/5.f );
        color = cinder::Color( 1, color_, color_ );

        using namespace cinder;
        tex = &GData::FindTexture( "slash.png" );
        delete_t = 0.0f;
        for ( int i = 0; i < 3; i++ ) {
            slash_ses.push_back( &GData::FindAudio( "SE/kiru" + std::to_string( i ) + ".wav" ) );
        }

    }
    //Effectの描画を設定します 
    void Set( ci::Vec2f pos_, ci::Vec2f size_, float angle_ )
    {
        pos = pos_;
        size = size_;
        angle = angle_;
        activeTime = 0;
        isActive = true;
        slash_ses[cinder::randInt( 3 )]->Play( );
    }
    void Update( );
    void Draw( );
    bool Active( ) { return isActive; }
private:
    ci::Vec2f pos;
    ci::Vec2f size;
    ci::Vec2f realsize;
    ci::Vec2f realpos;
    cinder::Color color;
    std::vector<User::Audio*> slash_ses;
    float angle;
    float delete_t;
    int activeTime;
    int maxactiveTime;
    cinder::gl::Texture* tex;
    bool isActive;
    void textureDraw( cinder::gl::Texture const& texture, cinder::Vec2f position = cinder::Vec2f::zero( ) );
    void textureDraw( cinder::gl::Texture const& texture, cinder::Vec2f position, cinder::Area area, float value );
    float EasingLinear( float t, float b, float e ) {
        return ( e - b ) * t + b;

    }

};
