#pragma once

#include "EnemyObject.h"
#include "cinder/Color.h"
#include "cinder/gl/Texture.h"
#include "cinder/Camera.h"
#include "EnemyBulletManager.h"
#include "../Effect/EffectManager.h"
#include "AttackTime.h"

namespace User
{
    class EnemyBase;
    using EnemyBaseRef = std::shared_ptr<EnemyBase>;
    using EnemyList = std::list<EnemyBaseRef>;

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
        cinder::ColorA hitColor;
        int deadTime;
        Status status;
        int mutekiFrame;
        int maxMutekiFrame;

    private: EnemyList enemyList;
    public: EnemyList EnemyRecovery( );
    protected:  template <class Ty>
        void EnemyCreate( cinder::Vec3f position, const cinder::CameraPersp& camera );

    private: EnemyBulletList bulletList;
    public: EnemyBulletList BulletRecovery( );
    protected: template <class Ty>
        void BulletCreate( Ty const& instans );

    private: EffectList effectList;
    public: EffectList EffectRecovery( );
    protected: template <class Ty>
        void EffectCreate( Ty const& instans );

    public:
        EnemyBase( cinder::Vec3f pos, const cinder::CameraPersp& camera );
        EnemyBase( cinder::Vec3f pos, const cinder::CameraPersp& camera, Status status, bool bullet );
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
        cinder::ColorA HitColor( ) { return hitColor; }
        float NormalizedHitPoint( ) { return status.HP / status.maxHP; }
        Status GetStatus( ) { return status; }
    public:
        bool IsAttackMotion( ) { return attackTime.IsAttackMotion( ); }
        bool IsAttackOneFramePrev( ) { return attackTime.IsAttackOneFramePrev( ); }
        float NormalizedAttackFrame( ) { return attackTime.NormalizedAttackFrame( ); }
    public:
        // ���� : �����������̒��S����̋���(���K���ς�) : 0.0 ~ 1.0(���a)
        int Hit( cinder::CameraPersp const& camera, float length, int scoreRate, float value = 1.0F );
        // �����I�Ƀ_���[�W��^����֐�
        int Damage( cinder::CameraPersp const& camera, int damage, int scoreRate );
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

        bool IsMuteki( );
    protected: // �ȉ� �A�b�v�f�[�g�ɋL������
        void CollideField( );
        // �n�ʂ̒��ɂ�����n��Ɉ����߂��܂��B
        void CollideGround( );
        // HP���Ȃ��Ȃ����玀�ʂ܂ŃJ�E���g���܂��B
        void Dying( );
    protected:
        // �����Ă��邩���m�F���܂��B
        void LiveCheck( );

        void MutekiEffect( );
        // �_���[�W���󂯂����ɑ̂̐F�����X�Ɍ��ʂ�ɂ��܂��B
        void DamageEffect( );
        // �J�����̕����������܂��B
        void CameraSee( cinder::CameraPersp const& camera );
    protected:
        void Jump( cinder::Vec3f jumpPower );
    private:
        void SpawnEffect( cinder::CameraPersp const& camera );
    };

    template<class Ty>
    inline void EnemyBase::EnemyCreate( cinder::Vec3f position, const cinder::CameraPersp& camera )
    {
        enemyList.emplace_back( std::make_shared<Ty>( position, camera ) );
    }
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