# pragma once

# include "cinder/gl/Texture.h"

# include "HundAnimation.h"

# include "../Interfaces/Description.h"

# include "Yumayo.h"

namespace User
{
    class Kougeki
    {
        std::list<std::string> enemyDataPath;
        cinder::gl::Texture* �U���̋Ɉ�;
    public:
        Kougeki( );
        void Draw( );
        std::string GetEnemyCreatePath( );
        bool IsNextStage( );
    };

    class Bougyo
    {
        int const maxGuardSucceedNum = 3;
        int guardSucceedNum;
        cinder::gl::Texture* �h��̋Ɉ�;
        Fonts font;
    public:
        Bougyo( );
        void PlayerGuardSucceed( );
        bool IsNextStage( );
        int GetRemainingSucceedNum( ) { return guardSucceedNum; }
        void Draw( );
        void DrawRemainin( );
    };

    class TutorialManager
    {
        using Func = void( TutorialManager::* )( );
        Kougeki kougeki;
        Bougyo bougyo;
        Description description;
        Func func;
        HundAnimation handAnimation;
    public:
        TutorialManager( );
        void �U��( );
        void �h��( );
    public:
        void Set( Func func );
        void Draw( );
    public: // �U��
        std::string KougekiGetEnemyCreatePath( ) { return kougeki.GetEnemyCreatePath( ); }
        bool KougekiIsNextStage( ) { return kougeki.IsNextStage( ); }
    public: // �h��
        void BougyoPlayerGuardSucceed( ) { return bougyo.PlayerGuardSucceed( ); }
        bool BougyoIsNextStage( ) { return bougyo.IsNextStage( ); }
    };
}