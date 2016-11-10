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
        bool isLanding;
        cinder::gl::Texture texture;
    private:
        EnemyObject initObject; // ���������ꂽ���̃I�u�W�F�N�g��Ԃ�ۑ����Ă��邾���Ȃ̂�private.
        float HP; // HP�̕ϓ��̓v���C���[�̍U���݂̂Ȃ̂�private.
        cinder::Color hitColor; // �J���[�̕ϓ��̓v���C���[�̍U���݂̂Ȃ̂�private.
        bool isLive; // �����Ă��邩�ǂ����𔻒f����̂͊e���ł��Ȃ��̂�private.
        int deadTime; // ���ɂ䂭���Ԃ������邱�Ƃ͂Ȃ��̂�private.
        int attackPoint; // �v���C���[�ւ̍U���͂������邱�Ƃ͂Ȃ��̂�private.
        EnemyBulletList bulletList; // �ꎞ�I�ɒe���G�l�~�[�ɕۑ����Ă��������Ȃ̂�private.
    public:
        EnemyBase( cinder::Vec3f pos, const cinder::CameraPersp& camera );
        virtual void update( );
        virtual void draw( );
        // �G�l�~�[���U�����Ă���Ȃ� true ���Ԃ��ė��܂��B
        // �p�����čU�����@�����Ă��������B
        virtual bool Attack( const cinder::CameraPersp& camera );
    public: // �Q�b�^�[
        cinder::Vec3f Position( ) { return object.Position( ); }
        cinder::Vec3f Size( ) { return object.Size( ); }
        int AttackPoint( ) { return attackPoint; }
        bool IsLive( ) { return isLive; }
        cinder::Color HitColor( ) { return hitColor; }
    public:
        // ���� : �����������̒��S����̋���(���K���ς�) : 0.0 ~ 1.0(���a)
        int Hit( float length );
        // �G�l�~�[�������Ă������̊m�F
        bool IsActive( );
        // �U�����[�V��������߁A�v���C���[���牓������܂��B
        void ResetPosition( );
        // ���˂����e��S�ĉ�����܂��B���̊֐����ĂԂƂ��̃N���X�������Ă���e��S�ăN���A���܂��B
        EnemyBulletList BulletsRecovery( );
    protected:
        // �n�ʂ̒��ɂ��邩�ǂ����B
        bool IsUnderGround( );
        // �W�����v���Ă��邩�ǂ����B
        bool IsJumping( );
    protected: // �ȉ� �A�b�v�f�[�g�ɋL������
        // �d�͂������܂��B
        void Gravitate( );
        // �n�ʂ̒��ɂ�����n��Ɉ����߂��܂��B
        void CollideGround( );
        // �����Ă��邩���m�F���܂��B
        void LiveCheck( );
        // HP���Ȃ��Ȃ����玀�ʂ܂ŃJ�E���g���܂��B
        void Dying( );
        // �_���[�W���󂯂����ɑ̂̐F�����X�Ɍ��ʂ�ɂ��܂��B
        void DamageEffect( );
    protected:
        // �e���쐬���܂��B�쐬�����e�̓G�l�~�[�Ɉꎞ�I�ɕۑ�����܂��B
        template <class Ty>
        void BulletCreate( Ty const& instans );
    };

    template<class Ty>
    inline void EnemyBase::BulletCreate( Ty const& instans )
    {
        bulletList.emplace_back( std::make_shared<Ty>( Ty( instans ) ) );
    }
}