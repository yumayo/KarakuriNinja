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
        cinder::gl::Texture* ê‡ñæ;
        std::vector<cinder::gl::Texture*> suuji;
        Fonts font;
        Audio* ÉNÉäÉA;
        bool ok;
    public:
        Kougeki( );
        void Draw( );
        void DrawRemainin( );
        void DrawClear( );
        std::string GetEnemyCreatePath( );
        bool IsNextStage( );
        void End( );
    };

    class Bougyo
    {
        int const maxGuardSucceedNum = 3;
        int guardSucceedNum;
        cinder::gl::Texture* ñhå‰ÇÃã…à”;
        cinder::gl::Texture* ê‡ñæ;
        std::vector<cinder::gl::Texture*> suuji;
        Fonts font;
        Audio* ÉNÉäÉA;
        bool ok;
    public:
        Bougyo( );
        void PlayerGuardSucceed( );
        bool IsNextStage( );
        int GetRemainingSucceedNum( ) { return guardSucceedNum; }
        void Draw( );
        void DrawRemainin( );
        void DrawClear( );
        void End( );
    };

    class TutorialManager
    {
        using Func = void( TutorialManager::* )( );
        Kougeki kougeki;
        Bougyo bougyo;
        Description description;
        Func func;
        HundAnimation handAnimation;
        Audio* ñ‚ëËíÒãN;
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
        void KougekiEnd( ) { kougeki.End( ); }
    public: // ñhå‰
        void BougyoPlayerGuardSucceed( ) { return bougyo.PlayerGuardSucceed( ); }
        bool BougyoIsNextStage( ) { return bougyo.IsNextStage( ); }
        void BougyoEnd( ) { bougyo.End( ); }
    };
}