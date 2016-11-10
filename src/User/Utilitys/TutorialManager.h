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
        cinder::gl::Texture* çUåÇÇÃã…à”;
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
        cinder::gl::Texture* ñhå‰ÇÃã…à”;
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
        void çUåÇ( );
        void ñhå‰( );
    public:
        void Set( Func func );
        void Draw( );
    public: // çUåÇ
        std::string KougekiGetEnemyCreatePath( ) { return kougeki.GetEnemyCreatePath( ); }
        bool KougekiIsNextStage( ) { return kougeki.IsNextStage( ); }
    public: // ñhå‰
        void BougyoPlayerGuardSucceed( ) { return bougyo.PlayerGuardSucceed( ); }
        bool BougyoIsNextStage( ) { return bougyo.IsNextStage( ); }
    };
}