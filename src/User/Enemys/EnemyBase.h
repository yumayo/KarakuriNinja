#pragma once

#include "EnemyObject.h"
#include "cinder/Color.h"
#include "cinder/gl/Texture.h"
#include "cinder/Camera.h"
#include "EnemyBulletManager.h"

namespace User
{
    class EnemyBase
    {
    protected:
        EnemyObject object;
        EnemyObject initObject;
        bool isLanding;
        cinder::gl::TextureRef textureRef;
    private:
        float maxHP;
        float HP; // HP�̕ϓ��̓v���C���[�̍U���݂̂Ȃ̂�private.
        cinder::Color hitColor; // �J���[�̕ϓ��̓v���C���[�̍U���݂̂Ȃ̂�private.
        bool isLive; // �����Ă��邩�ǂ����𔻒f����̂͊e���ł��Ȃ��̂�private.
        int deadTime; // ���ɂ䂭���Ԃ������邱�Ƃ͂Ȃ��̂�private.
        int attackPoint; // �v���C���[�ւ̍U���͂������邱�Ƃ͂Ȃ��̂�private.
        EnemyBulletList bulletList; // �ꎞ�I�ɒe���G�l�~�[�ɕۑ����Ă��������Ȃ̂�private.
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
    public:
        // ���� : �����������̒��S����̋���(���K���ς�) : 0.0 ~ 1.0(���a)
        int Hit( float length );
        // �����I�Ƀ_���[�W��^����֐�
        int Damage( int damage );
        // �G�������I�ɎE���֐�
        void Kill( );
        // �G�l�~�[�������Ă������̊m�F
        bool IsActive( );
        // ���˂����e��S�ĉ�����܂��B���̊֐����ĂԂƂ��̃N���X�������Ă���e��S�ăN���A���܂��B
        EnemyBulletList BulletsRecovery( );
    protected:
        bool IsJumping( );
        // �n�ʂ̒��ɂ��邩�ǂ����B
        bool IsUnderGround( );
        // �X�N���[���̒��ɂ��鎞��true���Ԃ��Ă��܂��B
        bool IsInTheScreen( cinder::CameraPersp const& camera );
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
        void CameraSee( cinder::CameraPersp const& camera );
        void Jump( cinder::Vec3f jumpPower );
    };

    template<class Ty>
    inline void EnemyBase::BulletCreate( Ty const& instans )
    {
        bulletList.emplace_back( std::make_shared<Ty>( Ty( instans ) ) );
    }
}