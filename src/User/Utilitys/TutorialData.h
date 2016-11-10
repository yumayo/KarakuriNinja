# pragma once

# include <algorithm>
# include <list>
# include <string>

# include "Yumayo.h"

namespace User
{
    class TutorialBase
    {
        bool isComplete = false; // �`���[�g���A�����I��������ǂ����B
        bool isStopUpdate = false; // �A�b�v�f�[�g���~�߂邩�ǂ����B
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
        int successFrame = 0; // �K�[�h�o���Ă���t���[����ۑ����܂��B
        int maxSuccessFrame = 60 * 2; // �K�[�h�̃`���[�g���A���I���܂ł̎���
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
        static TutorialAttackCircle attackCircle; // �A�^�b�N�T�[�N���o�����̃`���[�g���A��
        static TutorialGuard guard; // ����̍U����h�䂷��Ƃ��̃`���[�g���A��
        static TutorialSpawn spawn; // �G�l�~�[���o���������̃`���[�g���A��
        static TutorialPlayerAttack playerAttack; // �v���C���[�̍U���̃`���[�g���A��
        static TutorialEnemyKill enemyKill; // �G�l�~�[��|���`���[�g���A��
        static TutorialSpecial special; // �X�y�V���������܂��āA����𔭓�����Ƃ��̃`���[�g���A��
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