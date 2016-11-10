# pragma once

# include <algorithm>
# include <list>
# include <string>

# include "Yumayo.h"

namespace User
{
    class TutorialBase
    {
        bool isComplete = false; // チュートリアルが終わったかどうか。
        bool isStopUpdate = false; // アップデートを止めるかどうか。
    public:
        virtual void TutorialStart( ) 
        {
            isComplete = false;
            isStopUpdate = true;
        }
        virtual void TutorialEnd( )
        {
            isComplete = true;
            isStopUpdate = false;
        }
        virtual void TutorialReset( )
        {
            isComplete = false;
            isStopUpdate = false;
        }
        virtual bool IsComplete( ) { return isComplete; }
        virtual bool IsStopUpdate( ) { return isStopUpdate; }
    protected:
        
    };

    class TutorialAttackCircle : public TutorialBase
    {
    public:
        void TutorialStart( ) override;
    };

    class TutorialGuard : public TutorialBase
    {
        std::shared_ptr<Fonts> font;
        int successFrame = 0; // ガード出来ているフレームを保存します。
        int maxSuccessFrame = 60 * 2; // ガードのチュートリアル終了までの時間
    public:
        void TutorialStart( ) override;
        void TutorialReset( ) override
        {
            TutorialBase::TutorialReset( );
            successFrame = 0;
            maxSuccessFrame = 60 * 2;
        }
    public:
        void Update( bool success )
        {
            if ( success ) successFrame = std::min( successFrame + 1, maxSuccessFrame );
            else successFrame = 0;
        }
        void Draw( );
        bool IsSuccessFull( ) { return successFrame == maxSuccessFrame; }
    };

    class TutorialSpawn : public TutorialBase
    {
    public:
        void TutorialStart( ) override;
    };

    class TutorialPlayerAttack : public TutorialBase
    {
    public:
        void TutorialStart( ) override;
    };

    class TutorialEnemyKill : public TutorialBase
    {
    public:
        void TutorialStart( ) override;
    };

    class TutorialSpecial : public TutorialBase
    {
    public:
        void TutorialStart( ) override;
    };

    class TutorialBossSpawn : public TutorialBase
    {
    public:
        void TutorialStart( ) override;
    };

    class TutorialBossSerif : public TutorialBase
    {
    public:
        void TutorialStart( ) override;
    };

    class TutorialData
    {
    public:
        static std::list<std::string> talkString;
    public:
        static TutorialAttackCircle attackCircle; // アタックサークル出現時のチュートリアル
        static TutorialGuard guard; // 相手の攻撃を防御するときのチュートリアル
        static TutorialSpawn spawn; // エネミーが出現した時のチュートリアル
        static TutorialPlayerAttack playerAttack; // プレイヤーの攻撃のチュートリアル
        static TutorialEnemyKill enemyKill; // エネミーを倒すチュートリアル
        static TutorialSpecial special; // スペシャルが溜まって、それを発動するときのチュートリアル
        static TutorialBossSpawn bossSpawn;
        static TutorialBossSerif bossSerif;
    public:
        static bool IsStopUpdate( );
        static void Reset( );
    public:
        static std::string Serif( );
        static void PopFrontSerif( );
        static bool IsSerifTalked( );
        static void SerifRead( std::string const& jsonPath );
    };
}