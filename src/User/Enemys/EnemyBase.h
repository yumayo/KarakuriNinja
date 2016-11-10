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
        float NormalizedAttackFrame( ) { return static_cast<float>( attackFrame ) / maxAttackFrame; }
    };

    class EnemyBase
    {
    protected:
        EnemyObject object;
        EnemyObject initObject;
        bool isLanding;
        cinder::gl::Texture* texture;
        AttackTime attackTime;
    private:
        float maxHP;
        float HP;
        cinder::Color hitColor;
        bool isLive;
        int deadTime;
        int attackPoint;
    private:
        EnemyBulletList bulletList;
        EffectList effectList;
    public:
        // ���˂����e��S�ĉ�����܂��B���̊֐����ĂԂƂ��̃N���X�������Ă���e��S�ăN���A���܂��B
        EnemyBulletList BulletsRecovery( );
        EffectList EffectRecovery( );
    protected: // �摜���쐬���Ȃ��R���X�g���N�^
        EnemyBase( cinder::Vec3f pos, const cinder::CameraPersp& camera );
        EnemyBase( cinder::Vec3f pos, const cinder::CameraPersp& camera, float sizeScale );
    public: // �摜���쐬����R���X�g���N�^
        EnemyBase( cinder::Vec3f pos, const cinder::CameraPersp& camera, std::string const& path );
        EnemyBase( cinder::Vec3f pos, float sizeScale, const cinder::CameraPersp& camera, std::string const& path );
        EnemyBase( cinder::Vec3f pos, float sizeScale, float HP, int attackPoint, const cinder::CameraPersp& camera, std::string const& path );
        virtual void update( cinder::CameraPersp const& camera );
        virtual void draw( );
        virtual void drawUI( );
        // �G�l�~�[���U�����Ă���Ȃ� true ���Ԃ��ė��܂��B
        // �p�����čU�����@�����Ă��������B
        virtual bool Attack( const cinder::CameraPersp& camera );
    public: // �Q�b�^�[
        cinder::Vec3f Position( ) { return object.Position( ); }
        cinder::Vec3f Size( ) { return object.Size( ); }
        int AttackPoint( ) { return attackPoint; }
        bool IsLive( ) { return isLive; }
        cinder::Color HitColor( ) { return hitColor; }
        float NormalizedHitPoint( ) { return HP / maxHP; }
        bool IsAttackMotion( ) { return attackTime.IsAttackMotion( ); }
        float NormalizedAttackFrame( ) { return attackTime.NormalizedAttackFrame( ); }
    public:
        // ���� : �����������̒��S����̋���(���K���ς�) : 0.0 ~ 1.0(���a)
        int Hit( float length );
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

        bool IsInField( );
        void CollideField( );
    protected: // �ȉ� �A�b�v�f�[�g�ɋL������
        // �n�ʂ̒��ɂ�����n��Ɉ����߂��܂��B
        void CollideGround( );
        // �����Ă��邩���m�F���܂��B
        void LiveCheck( );
    protected:
        // HP���Ȃ��Ȃ����玀�ʂ܂ŃJ�E���g���܂��B
        void Dying( );
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