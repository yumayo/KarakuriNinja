# pragma once

# include "Scene.hpp"

# include "../Utilitys/Yumayo.h"
# include "../Utilitys/Audio.h"
#include"../Utilitys/Hirasawa.h"
#include"Framework.hpp"
namespace User
{
    class Fusuma
    {
        float husuma_t;
        cinder::Vec2f lefthusumapos;
        cinder::Vec2f righthusumapos;
        cinder::Vec2f husumasize;
        cinder::gl::Texture* lefthusuma;
        cinder::gl::Texture* righthusuma;
    public:
        Fusuma( );
        bool IsMoveFinished( ) { return 1.0F <= husuma_t; }
		bool getT() { return husuma_t; }
        void Fusuma::openFusuma( );
        void Fusuma::closeFusuma( );
        void Fusuma::drawFusuma( );
    };

    class SceneResult : public SceneBase
    {
    public:
        SceneResult( int score, int maxcombo, int nowhp, int time );
        ~SceneResult( );
    public:
        void resize( );
        void update( );
        void draw( );
        void select( );
    public:
        void UpdateLogoAlpha( );
    public:
        void beginDrawUI( );
        void drawUI( );
        void endDrawUI( );
    private:
        enum TS {
            MAKIMONO, HONOO, TITLE, NINJA, TABLE, SCORE, COMBO, HP, TIME, LASTSCORE, RANKING, ALFA, MAX
        };
        enum SPE {
            E_SCORE, E_COMBO, E_HP, E_TIME, E_LASTSCORE, E_RANKING, E_MAX
        };
        struct Icon
        {
            cinder::Vec2f pos;
            cinder::Vec2f size;
            int score;
            int rank;
            bool isscoredraw;
            bool ismy;
        };
        int score;
        Fonts font;
        Fonts tables;
        Audio* audio;
        // スラッシュでの操作を行いますので、インスタンス化します。
        // Yumayo.h
        SlashInput slashInput;
        // シーン遷移に使います。
        bool isEnd;
        // スタートボタンの変数。
        float logoAlpha;
        float logoAlphaSpeed;
        int endscore;
        int ranking;
        int datanum;
		int maxcombo_;
		int nowhp_;
		int time_;
        std::vector<int> player_scores;
        std::vector<Icon> icons;
        cinder::gl::Texture* texture;

        cinder::gl::Texture* makimonotex;
        cinder::gl::Texture* makimonotex2;
        cinder::gl::Texture* fire;



        cinder::gl::Texture* background;
        cinder::gl::Texture* window;
        cinder::gl::Texture* ninja;
        cinder::Vec2f ninjapos;
        cinder::Vec2f ninjasize;
        float iconcamera;
        float camera_t;
        float move_t;
        float delay_t;
        float myrank_camera_t;
        float myrankpos_;
        Audio* taiko1;
        Audio* taiko2;
		Audio* bgm;
        float flash_t;
        float flash_alfa;
        float t_[TS::MAX];
        bool s_end[SPE::E_MAX];
        bool rankin;
		float endt_[3];
        Fusuma fusuma;

        void drawBackground( );
        void updateflash( );
        void drawflash( );
        void drawNinja( );
        void drawWindow( );
        void updateNinja( );
        void drawTitleBase( );
        void drawFire( );
        void drawTitle( );
        void drawTables( );
        void drawBeginScore( );
        void soundplay( Audio* audio, bool& flag );
        void pushIcon( );
        int calcurateScore(int score, int maxcombo, int nowhp, int time);
        void roadScores( );
        void sortScores( );
        int calcurateRanking( );
        void writeScores( );
        void drawIcon( );
        void updatecamera( );
        void drawMyIcon( );
		void drawEnd();
		void textureDraw(cinder::gl::Texture const & texture, cinder::Vec2f position, cinder::Vec2f realsize, cinder::Area area, float size);
    };
}