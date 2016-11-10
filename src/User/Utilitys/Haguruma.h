
#pragma once
#include "cinder/Vector.h"
#include "Framework.hpp"
#include "cinder/app/App.h"
#include"../Utilitys/Hirasawa.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include"../Utilitys/Audio.h"
#include"GlobalData.hpp"
namespace User
{
    enum HagurumaType {
        LEFTHAGURUMA, RIGHTHAGURUMA
    };
    enum UPDATETYPE
    {
        NOTUPDATE, STARTUPDATE, ENDUPDATE

    };
    class Haguruma {
    public:
        Haguruma( );
        Haguruma( cinder::Vec2f pos, cinder::Vec2f scale, float starttime, float endtime, float rotatespeed, HagurumaType type, bool playsound = true );
        void draw( );
        void update( );
        void setUpdate( UPDATETYPE updatetype ) {
            updatetype_ = updatetype;
        }
        void set_anglescale_t( float anglescale_t_ ) { this->anglescale_t_ = anglescale_t_; }
    private:
        cinder::Vec2f pos_;
        cinder::Vec2f scale_;
        Audio* se;
        bool playsound_;
        float starttime_;
        float endtime_;
        cinder::gl::Texture* tex[6];
        UPDATETYPE updatetype_;
        float rotate[6];
        HagurumaType type_;
        float speed_;
        float rotatespeed_;
        float t_;
        float anglescale_t_;
        void drawTexture( cinder::Vec2f trancepos, int number );
        void startUpdate( );
        void endUpdate( );
        void angleUpdate( );
    };
}