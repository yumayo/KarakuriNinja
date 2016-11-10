#pragma once

#include "EnemyObject.h"
#include "cinder/Color.h"
#include "cinder/gl/Texture.h"
#include "cinder/Camera.h"
#include "EnemyBulletManager.h"
#include "../Effect/EffectManager.h"

namespace User
{
    class AttackTime
    {
        bool isAttackMotion;
        int attackFrame;
        int maxAttackFrame;
    public:
        AttackTime( )
            : isAttackMotion( false )
        {
            AttackFrame( 0 );
        }
        void Update( )
        {
            attackFrame = std::min( attackFrame + 1, maxAttackFrame );
            if ( attackFrame == maxAttackFrame ) isAttackMotion = false;
        }
        void AttackFrame( int attackFrame )
        {
            if ( attackFrame <= 0 ) // 0���Z���
            {
                isAttackMotion = false;
                this->attackFrame = 1;
                maxAttackFrame = 1;
            }
            else
            {
                isAttackMotion = true;
                this->attackFrame = 0;
                maxAttackFrame = attackFrame;
            }
        }
        bool IsAttackMotion( ) { return isAttackMotion; }
        bool IsAttackOneFramePrev( ) { return maxAttackFrame == attackFrame + 1; }
        float NormalizedAttackFrame( ) { return static_cast<float>( attackFrame ) / maxAttackFrame; }
    };

    class EnemyBase
    {
    public:
        class Status
        {
        public:
            Status( )
                : maxHP( 1 )
                , HP( maxHP )
                , attackPoint( 0 )
            { }
            Status( float HP, int attackPoint )
                : maxHP( HP )
                , HP( maxHP )
                , attackPoint( attackPoint )
            { }
            float maxHP;
            float HP;
            int attackPoint;
        };
        // ���J���
    protected:
        EnemyObject object;
        EnemyObject initObject;
        bool isLanding;
        bool isLive;
        cinder::gl::Texture* texture;
        cinder::gl::Texture* knockBackTexture;
        AttackTime attackTime;
        int frame = 0;
        // ����J���
    private:
        cinder::Color hitColor;
        int deadTime;
        Status status;
        EnemyBulletList bulletList;
        EffectList effectList;
    public:
        // ���˂����e��S�ĉ�����܂��B���̊֐����ĂԂƂ��̃N���X�������Ă���e��S�ăN���A���܂��B
        EnemyBulletList BulletsRecovery( );
        EffectList EffectRecovery( );
    public:
        EnemyBase( cinder::Vec3f pos, const cinder::CameraPersp& camera );
        EnemyBase( cinder::Vec3f pos, const cinder::CameraPersp& camera, Status status, float scale = 1.0F );
        virtual void update( cinder::CameraPersp const& camera );
        virtual void draw( );
        virtual void drawUI( cinder::CameraPersp const& camera );
        // �G�l�~�[���U�����Ă���Ȃ� true ���Ԃ��ė��܂��B
        // �p�����čU�����@�����Ă��������B
        virtual bool Attack( const cinder::CameraPersp& camera );
        // �����Ă��邩�̏��
        virtual bool IsLive( ) { return isLive; }
    public: // �Q�b�^�[
        cinder::Vec3f Position( ) { return object.Position( ); }
        cinder::Vec3f Size( ) { return object.Size( ); }
        int AttackPoint( ) { return status.attackPoint; }
        cinder::Color HitColor( ) { return hitColor; }
        float NormalizedHitPoint( ) { return status.HP / status.maxHP; }
        Status GetStatus( ) { return status; }
    public:
        bool IsAttackMotion( ) { return attackTime.IsAttackMotion( ); }
        bool IsAttackOneFramePrev( ) { return attackTime.IsAttackOneFramePrev( ); }
        float NormalizedAttackFrame( ) { return attackTime.NormalizedAttackFrame( ); }
    public:
        // ���� : �����������̒��S����̋���(���K���ς�) : 0.0 ~ 1.0(���a)
        int Hit( cinder::CameraPersp const& camera, float length, int scoreRate, float value = 1.0F);
        // �����I�Ƀ_���[�W��^����֐�
        int Damage( int damage );
        // �G�������I�ɎE���֐�
        void Kill( );
        // �G�l�~�[�������Ă������̊m�F
        bool IsActive( );
    protected:
        bool IsJumping( );
        // �n�ʂ̒��ɂ��邩�ǂ����B
        bool IsUnderGround( );
        // �X�N���[���̒��ɂ��鎞��true���Ԃ��Ă��܂��B
        bool IsInTheScreen( cinder::CameraPersp const& camera );
        // �t�B�[���h�̒��ɂ��鎞��true���Ԃ��Ă��܂��B
        bool IsInField( );
        // �m�b�N�o�b�N���Ă��邩�ǂ����B
        bool IsKnockBack( );
    protected: // �ȉ� �A�b�v�f�[�g�ɋL������
        void CollideField( );
        // �n�ʂ̒��ɂ�����n��Ɉ����߂��܂��B
        void CollideGround( );
        // HP���Ȃ��Ȃ����玀�ʂ܂ŃJ�E���g���܂��B
        void Dying( );
    protected:
        // �����Ă��邩���m�F���܂��B
        void LiveCheck( );
        // �_���[�W���󂯂����ɑ̂̐F�����X�Ɍ��ʂ�ɂ��܂��B
        void DamageEffect( );
    protected:
        // �e���쐬���܂��B�쐬�����e�̓G�l�~�[�Ɉꎞ�I�ɕۑ�����܂��B
        template <class Ty>
        void BulletCreate( Ty const& instans );
    protected:
        // �e���쐬���܂��B�쐬�����e�̓G�l�~�[�Ɉꎞ�I�ɕۑ�����܂��B
        template <class Ty>
        void EffectCreate( Ty const& instans );
    protected:
        void CameraSee( cinder::CameraPersp const& camera );
        void Jump( cinder::Vec3f jumpPower );
    private:
        void SpawnEffect( cinder::CameraPersp const& camera );
    };

    template<class Ty>
    inline void EnemyBase::BulletCreate( Ty const& instans )
    {
        bulletList.emplace_back( std::make_shared<Ty>( Ty( instans ) ) );
    }
    template<class Ty>
    inline void EnemyBase::EffectCreate( Ty const & instans )
    {
        effectList.emplace_back( std::make_shared<Ty>( Ty( instans ) ) );
    }
}