
#pragma once
#include "cinder/Vector.h"
#include "Framework.hpp"
#include "../Utilitys/Nomoto.h"
#include "cinder/app/App.h"
#include"../Utilitys/Hirasawa.h"
#include"../Special/SpecialSceneBase.h"
#include"ZKOO.hpp"
#include"../Utilitys/Audio.h"
#include"SpecialShuriken.h"
#include"GlobalData.hpp"
#include"cinder/Rand.h"
using namespace cinder;
const int ICONNUM = 3;
class SpecialSelect : public SpecialSceneBase {
public:
    SpecialSelect::SpecialSelect( ) {
        is_special = false;
        awake_t_ = 0.0f;
        delay_t_ = 0.0f;
        awake2_t_ = 0.0f;
        frame = 0;
        start_t = 0.0f;
        go_next_ = false;
        ischoosed_ = false;
        end_t_ = 0.0f;
        last_t_ = 0.0f;
        yazirushirotate_ = 0.0f;
        isshifteasing_ = false;
        for ( int i = 0; i < ICONNUM; i++ ) {
            Icon icon;
            icon.startpos = Vec2f( app::getWindowWidth( ) / 2 + float( ( i - 1 )*450.f ), app::getWindowHeight( ) / 2 + 700 );
            icon.pos = icon.startpos;
            icon.endpos = Vec2f( app::getWindowWidth( ) / 2 + float( ( i - 1 )*450.f ), app::getWindowHeight( ) / 2 );
            icon.shiftpos = icon.startpos;
            icon.size = Vec2f( 250.f, 250.f );
            icon.angle_t = 0.0f;
            icon.startangle = 5.f;
            icon.opened = false;
            icons.push_back( icon );
        }
        for ( int i = 0; i < 3; i++ ) {
            subangle[i] = 0.0f;
            subpos[i] = Vec2f( app::getWindowWidth( ) / 2 + float( ( i - 1 )*450.f ), app::getWindowHeight( ) + 500 );
        }
        for ( int i = 0; i < 3; i++ ) {
            icontex[i] = &GData::FindTexture( "Textures/select" + std::to_string( i ) + ".png" );
        }
        backanimation = &GData::FindTexture( "Textures/Effect/backanimation.png" );
        kuriko = &GData::FindTexture( "Textures/Effect/specialkuriko.png" );
        yazirushi = &GData::FindTexture( "Textures/specialyazirushi.png" );
        open_se.push_back( &GData::FindAudio( "SE/hiraku.wav" ) );
        subinfo[0] = &GData::FindTexture( "Textures/subfire.png" );
        subinfo[1] = &GData::FindTexture( "Textures/subwater.png" );
        subinfo[2] = &GData::FindTexture( "Textures/subtree.png" );
        hasudouinfotex = &GData::FindTexture( "Textures/specailhatudou.png" );
        selectinfotex = &GData::FindTexture( "Textures/specialselect.png" );
        beginposy = env.getWindowHeight( ) / 2.f;
        cutinsizey = env.getWindowHeight( ) / 2.f;
        pushShuriken( );
    }
    void draw( ) override;
    void update( bool isstop ) override;
    bool shift( ) override {
        return go_next_;
    }
    SpecialType getSpecialType( ) override {
        return specialtype_;
    }
    bool isFullScreen( ) override {
        return false;
    }
private:
    struct Icon {
        Vec2f pos;//実際のposです
        Vec2f startpos;
        Vec2f endpos;
        float startangle;
        float angle_t;
        float angle;
        Vec2f size;//四角形としてのサイズです。円なら半分にしてください
        Vec2f shiftpos;
        bool opened;
    };
    float beginposy;
    float cutinsizey;
    cinder::gl::Texture* backanimation;
    cinder::gl::Texture* kuriko;
    std::vector<User::Audio*> open_se;
    std::vector<Icon>icons;
    std::vector<SpecialShuriken> shurikens;
    void drawIcon( );
    Vec2f iconsize;
    bool is_special;
    bool isshifteasing_;
    bool go_next_;
    bool ischoosed_;
    SpecialType specialtype_;
    void updateIcon( );
    float awake_t_;
    float delay_t_;
    float awake2_t_;
    float start_t;
    float end_t_;
    float last_t_;
    float yazirushirotate_;
    int frame;
    Vec2f subpos[3];
    float subangle[3];
    gl::Texture* icontex[3];
    gl::Texture* yazirushi;
    gl::Texture* subinfo[3];
    gl::Texture* hasudouinfotex;
    gl::Texture* selectinfotex;
    bool isCanChoose( ) {
        return ( icons[icons.size( ) - 1].angle_t >= 1.0f ) && ( ischoosed_ == false );
    }
    bool circleCollision( const Vec2f pos1, const Vec2f pos2, const float r_size, float level = 1.f );
    void chooseSpecial( );//とりあえずタッチのプッシュでやっておきました
    void drawbeginSpecial( );
    void drawKuriko( );
    void pushShuriken( );
    void updateShuriken( );
    void drawYazirushi( );
    void drawHatsudouInfo( );
    bool shurikenEnd( ) {
        return shurikens[shurikens.size( ) - 1].end( );
    }
    void drawSelectInfo( );
    void drawSubInfo( );
    void updateSubInfo( );
    void DrawCutTexture( cinder::gl::Texture* texture, cinder::Vec2f position, cinder::Vec2f size, cinder::Vec2f offset, cinder::Vec2f cutSize );
};