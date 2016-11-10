# include "EnemyBoss.h"

# include "Framework.hpp"
# include "cinder/gl/gl.h"
# include "cinder/Rand.h"

#include "EnemyTutorial.h"
#include "EnemySlash.h"
#include "EnemySlashNot.h"
#include "EnemyBullet.h"
#include "EnemyBulletNot.h"
#include "EnemyBoss.h"
#include "EnemyBulletTutorial.h"
#include "EnemySlashWalk.h"
#include "EnemyBulletWalk.h"

# include "../Utilitys/Hirasawa.h"

# include "EnemyBulletTexture.h"

# include "GlobalData.hpp"

# include "TutorialData.hpp"

namespace User
{
    using namespace cinder;

    EnemyBoss::EnemyBoss( cinder::Vec3f pos, const cinder::CameraPersp& camera )
        : EnemyBase( pos, camera, Status( 22.0F, 7 ) )
        , timer( )
        , isAttack( false )
        , isDeadStop( true )
        , font( u8"HG�s����", 90 )
        , serif( u8"" )
        , prevAttackHP( 0 )
    {
        TRData::bossSpawn.TutorialStart( );
        {
            int index = 1;
            messageBoxLeft = &GData::FindTexture( "UI/e-serihu" + std::to_string( index++ ) + ".png" );
            messageBoxCenter = &GData::FindTexture( "UI/e-serihu" + std::to_string( index++ ) + ".png" );
            messageBoxRight = &GData::FindTexture( "UI/e-serihu" + std::to_string( index++ ) + ".png" );
        }

        {
            int index = 1;
            �ҋ@ = &GData::FindTexture( "Enemy/Boss/Boss (" + std::to_string( index++ ) + ").png" );
            texture = �ҋ@;
            �����郂�[�V�����摜 = &GData::FindTexture( "Enemy/Boss/Boss (" + std::to_string( index++ ) + ").png" );
            ������摜 = &GData::FindTexture( "Enemy/Boss/Boss (" + std::to_string( index++ ) + ").png" );
            �U�����[�V�����摜 = &GData::FindTexture( "Enemy/Boss/Boss (" + std::to_string( index++ ) + ").png" );
            �U���摜 = &GData::FindTexture( "Enemy/Boss/Boss (" + std::to_string( index++ ) + ").png" );
            ���Ɉړ� = &GData::FindTexture( "Enemy/Boss/Boss (" + std::to_string( index++ ) + ").png" );
            �E�Ɉړ� = &GData::FindTexture( "Enemy/Boss/Boss (" + std::to_string( index++ ) + ").png" );
            �|�ꂩ���郂�[�V���� = &GData::FindTexture( "Enemy/Boss/Boss (" + std::to_string( index++ ) + ").png" );
            �|��郂�[�V���� = &GData::FindTexture( "Enemy/Boss/Boss (" + std::to_string( index++ ) + ").png" );
            knockBackTexture = &GData::FindTexture( "Enemy/Boss/Boss (" + std::to_string( index++ ) + ").png" );

        }
        {
            int index = 1;
            �I�[��1 = &GData::FindTexture( "Enemy/Boss/�I�[��_1_" + std::to_string( index++ ) + ".png" );
            �I�[��2 = &GData::FindTexture( "Enemy/Boss/�I�[��_1_" + std::to_string( index++ ) + ".png" );
            auraTex = nullptr;
        }

        timer.Advance( 240 );
        SetFunction( &EnemyBoss::�{�X�o���̃G�t�F�N�g���I������܂őҋ@ );
        SetSerifFunction( u8"", &EnemyBoss::�k���Z���t );

        for ( int i = 0; i < 3; i++ )
        {
            se.push_back( &GData::FindAudio( "SE/shuri" + std::to_string( i ) + ".wav" ) );
            �e�J�E���g = 0;
        }

        jsonEnemys = JsonTree( app::loadAsset( "JSON/Bosspawn.json" ) )["Enemy"];
        jsonItr = jsonEnemys.begin( );
    }
    void EnemyBoss::update( cinder::CameraPersp const& camera )
    {
        if ( IsDeadSerif( ) )
        {
            object.Speed( Vec3f::zero( ) );
            prevPosition = object.Position( );
            timer.Advance( 120 );
            SetFunction( &EnemyBoss::���ʎ��̃��[�V���� );
        }
        if ( IsLive( ) )
        {
            // �d�͏���
            Gravitate( );
            // �m�b�N�o�b�N���͈ꎞ�I�ɏ��������܂���B
            if ( !IsKnockBack( ) )
            {
                // ���C������
                behavior( camera );

                // �^�C�}�[����
                timer.Update( );
            }
        }
        else
        {
            //texture = knockBackTexture;
        }


        frame += 1;
        CameraSee( camera );

        if ( IsLive( ) )
        {
            LiveCheck( );
            if ( !IsKnockBack( ) )
            {
                object.PositionAdd( object.Speed( ) );
                attackTime.Update( );
            }
        }
        MutekiEffect( );
        DamageEffect( );
        CollideGround( );// ����ł��Ă����s���܂��B
        //CollideField( );// ����ł��Ă����s���܂��B
        Dying( );// ����ł��Ă����s���܂��B

                 // �f�o�b�O�_���[�W
        if ( inputs.isPressKey( Key::KEY_LCTRL ) && inputs.isPushKey( Key::KEY_0 ) ) Kill( );
    }
    void EnemyBoss::draw( )
    {
        if ( isLive )
        {
            DrawAura( );
        }

        EnemyBase::draw( );

        if ( isLive )
        {
            if ( IsKnockBack( ) )
            {
                objects.clear( );
            }
            int i = 1;
            for ( auto itr = objects.begin( ); itr != objects.end( ); ++itr, ++i )
            {
                DrawAfterimage( *itr, i );
            }
        }
    }
    void EnemyBoss::drawUI( const cinder::CameraPersp& camera )
    {
        EnemyBase::drawUI( camera );
        serifBehavior( );
    }
    bool EnemyBoss::Attack( const cinder::CameraPersp& camera )
    {
        return isAttack && isDeadStop;
    }
    bool EnemyBoss::IsLive( )
    {
        return isLive || isDeadStop;
    }
    void EnemyBoss::Gravitate( )
    {
        //�@�W�����v���Ȃ�d�͂������܂��B
        if ( IsJumping( ) )
        {
            object.SpeedAdd( Vec3f( 0, -0.01, 0 ) );
        }
    }
    void EnemyBoss::DrawAfterimage( EnemyObject& object, int index )
    {
        gl::disable( GL_CULL_FACE );

        gl::pushModelView( );
        gl::translate( object.Position( ).x, 0, object.Position( ).z );
        gl::translate( 0, 0.05, 0 );
        gl::rotate( Vec3f( 90, 0, 0 ) );
        gl::color( ColorA( 0, 0, 0, 0.5 / index ) );
        auto distans = 1 - ( object.Position( ).y - object.Size( ).y / 2 ) / 2;
        if ( distans < 0 ) distans = 0;
        gl::drawSolidCircle( Vec2f::zero( ), ( object.Size( ).xy( ).length( ) / 4.0F ) * distans, 100 );
        gl::popModelView( );

        gl::pushModelView( );
        gl::translate( object.Position( ) );
        gl::multModelView( object.Quaternion( ).toMatrix44( ) );

        if ( IsKnockBack( ) )
        {
            gl::pushModelView( );
            gl::rotate( Vec3f( 0, 180, 180 ) );
            ColorA col = HitColor( );
            col.a = 1.0 / index;
            gl::color( col );
            knockBackTexture->bind( );
            gl::drawSolidRect( Rectf( -object.Size( ).xy( ) / 2.0F, object.Size( ).xy( ) / 2.0F ) );
            knockBackTexture->unbind( );
            gl::popModelView( );
        }
        else
        {
            gl::pushModelView( );
            gl::rotate( Vec3f( 0, 180, 180 ) );
            ColorA col = HitColor( );
            col.a = 1.0 / index;
            gl::color( col );
            texture->bind( );
            gl::drawSolidRect( Rectf( -object.Size( ).xy( ) / 2.0F, object.Size( ).xy( ) / 2.0F ) );
            texture->unbind( );
            gl::popModelView( );
        }

        gl::popModelView( );

        gl::enable( GL_CULL_FACE );
    }
    void EnemyBoss::�^�C�}�[����܂őҋ@( cinder::CameraPersp const& camera )
    {
        if ( IsFrieldKilledSerif( ) )
        {
            object.Speed( Vec3f::zero( ) );

            isFrieldKilledSerif = true;
            SetSerifFunction( u8"�悭�����̒��Ԃ��I�I", &EnemyBoss::�Z���t );

            timer.Advance( 120 ); // �Z���t��f���t���[��
            SetFunction( &EnemyBoss::������|���ꂽ���̃Z���t );
            return;
        }

        // �^�C�}�[�������玟�̊֐��ցB
        if ( timer.IsAction( ) )
        {
            SetFunction( &EnemyBoss::�ǂ����̍U�����o�����̊m�F );
            return;
        }
    }
    void EnemyBoss::�ǂ����̍U�����o�����̊m�F( cinder::CameraPersp const & camera )
    {
        auto anyAttack = randBool( );
        if ( anyAttack && ( ���ڍU���J�E���g < ���ڍU���̏���� ) )
        {
            �e�J�E���g = 0;

            SetFunction( &EnemyBoss::���ڍU���I�� );
            return;
        }
        else if ( �e�J�E���g < �e�̏���� )
        {
            ���ڍU���J�E���g = 0;

            SetFunction( &EnemyBoss::�ԐڍU���I�� );
            return;
        }
        else // �ǂ���Ƃ��o���Ȃ��ꍇ
        {
            if ( anyAttack )
            {
                SetFunction( &EnemyBoss::���ڍU���I�� );
                return;
            }
            else
            {
                SetFunction( &EnemyBoss::�ԐڍU���I�� );
                return;
            }
        }
    }
    void EnemyBoss::���ڍU���I��( cinder::CameraPersp const & camera )
    {
        auto cameraPos = camera.getEyePoint( ) + camera.getViewDirection( ) * camera.getNearClip( ) + camera.getViewDirection( );
        cameraPos.y = 0;

        auto pos = object.Position( );
        pos.y = 0;

        moveSpeed = cameraPos - pos;
        moveSpeed /= 60.0F;

        moveLeftRightSpeed = Vec3f::xAxis( ) * 0.4F;

        timer.Advance( 60 ); // �����ړ��t���[��

        ���ڍU���J�E���g++;
        SetFunction( &EnemyBoss::���E�ɍ����ړ����Ȃ���J�����֋߂Â� );
        return;
    }
    void EnemyBoss::�ԐڍU���I��( cinder::CameraPersp const & camera )
    {
        timer.Advance( 120 ); // �e�𓊂��铮��
        texture = �����郂�[�V�����摜;

        �e�J�E���g++;
        SetFunction( &EnemyBoss::�e�𓊂���܂ł̃��[�V���� );
        return;
    }
    void EnemyBoss::���E�ɍ����ړ����Ȃ���J�����֋߂Â�( cinder::CameraPersp const& camera )
    {
        if ( 3 < objects.size( ) ) objects.pop_front( );

        if ( !IsInTheScreen( camera ) )
        {
            moveLeftRightSpeed *= -1;
            object.PositionAdd( moveLeftRightSpeed );
        }

        auto cameraRightVec = camera.getViewDirection( );
        cameraRightVec.rotate( camera.getWorldUp( ), -M_PI / 2.0 );
        if ( moveLeftRightSpeed.dot( cameraRightVec ) < 0 )
        {
            texture = �E�Ɉړ�;
        }
        else
        {
            texture = ���Ɉړ�;
        }

        object.Speed( moveSpeed + moveLeftRightSpeed );

        objects.emplace_back( object );

        if ( timer.IsAction( ) )
        {
            auto cameraPos = camera.getEyePoint( ) + camera.getViewDirection( ) * camera.getNearClip( ) + camera.getViewDirection( );
            cameraPos.y = 0;
            object.Position( cameraPos );

            object.Speed( Vec3f::zero( ) );

            timer.Advance( 140 ); // �U�����[�V�����t���[������
            attackTime.AttackFrame( 140 );

            prevAttackHP = status.HP;
            objects.clear( );
            texture = �U�����[�V�����摜;
            prevPosition = object.Position( );
            SetFunction( &EnemyBoss::�U�����[�V���� );
            return;
        }
    }
    void EnemyBoss::�U�����[�V����( cinder::CameraPersp const& camera )
    {
        auraTex = ( ( frame / 15 ) % 2 == 0 ? �I�[��1 : �I�[��2 );
        auto yureru = Vec3f( randFloat( -0.01, 0.01 ), randFloat( -0.01, 0.01 ), 0 );
        object.Position( prevPosition + yureru );
        // �U�����[�V������A���̊֐��ցB
        if ( timer.IsAction( ) )
        {
            auraTex = nullptr;
            texture = �U���摜;
            object.Position( prevPosition );
            SetFunction( &EnemyBoss::�U�� );
            return;
        }

        if ( prevAttackHP != status.HP )
        {
            auraTex = nullptr;
            texture = �ҋ@;

            timer.Advance( 20 ); // �d���t���[��
            SetFunction( &EnemyBoss::�U����d�� );
            return;
        }
    }
    void EnemyBoss::�U��( cinder::CameraPersp const& camera )
    {
        // �U�����Ă������̊֐��ցB
        isAttack = true;

        timer.Advance( 20 ); // �d���t���[��
        SetFunction( &EnemyBoss::�U����d�� );
        return;
    }
    void EnemyBoss::�U����d��( cinder::CameraPersp const& camera )
    {
        // �U���t���O���I�t�ɁA�d������10�t���[���̌�A���̊֐��ցB
        isAttack = false;
        if ( timer.IsAction( ) )
        {
            texture = �ҋ@;
            SetFunction( &EnemyBoss::�W�����v�Ŗ߂� );
            return;
        }
    }
    void EnemyBoss::�W�����v�Ŗ߂�( cinder::CameraPersp const& camera )
    {
        auto backDirection = -object.Direction( ) * 6.0F / 60.0F;
        backDirection.rotate( Vec3f::yAxis( ), randFloat( -M_PI / 12.0F, M_PI / 12.0F ) );
        auto jumpPower = Vec3f( 0, 0.15, 0 );
        // �d����A�����ɃW�����v���Ď��̊֐��ցB
        Jump( jumpPower + backDirection );

        SetFunction( &EnemyBoss::���n );
        return;
    }
    void EnemyBoss::���n( cinder::CameraPersp const& camera )
    {
        // �n�ʂɒ�������A���̊֐��ցB
        if ( isLanding )
        {
            // �X�s�[�h��0�ɂ��܂��B�������Ⴄ�̂ŁB
            object.Speed( Vec3f::zero( ) );
            // ���̍U���܂ł̃t���[�������B
            timer.Advance( randInt( 180, 280 ) );
            SetFunction( &EnemyBoss::�^�C�}�[����܂őҋ@ );
            return;
        }
    }
    void EnemyBoss::���ʎ��̃��[�V����( cinder::CameraPersp const & camera )
    {
        isAttack = false;
        isDeadSerif = true;
        texture = �|�ꂩ���郂�[�V����;

        SetSerifFunction( u8"�����A��͖łт�", &EnemyBoss::�Z���t );

        timer.Advance( 120 ); // �Z���t��f���t���[��
        SetFunction( &EnemyBoss::���ʎ��̃Z���t );
        return;
    }
    void EnemyBoss::����( cinder::CameraPersp const & camera )
    {
        isDeadStop = false;
    }
    void EnemyBoss::�e�𓊂���܂ł̃��[�V����( cinder::CameraPersp const & camera )
    {
        if ( timer.IsAction( ) )
        {
            texture = ������摜;
            SetFunction( &EnemyBoss::�e�𓊂��� );
        }
    }
    void EnemyBoss::�e�𓊂���( cinder::CameraPersp const & camera )
    {
        //WavyBulletFiring( randFloat( 0, M_PI * 2 ), camera );
        RandomWavyBulletFiring( camera );
        for ( int i = 0; i < 3; i++ )
        {
            se[i]->Play( );
        }
        timer.Advance( 40 ); // �d���t���[��

        SetFunction( &EnemyBoss::�e�𓊂�����̍d�� );
        return;
    }
    void EnemyBoss::�e�𓊂�����̍d��( cinder::CameraPersp const & camera )
    {
        if ( timer.IsAction( ) )
        {
            // �^�C�}�[���Z�b�g���Ă܂��ҋ@��Ԃɂ��܂��B
            timer.Advance( randInt( 340, 440 ) );

            texture = �ҋ@;

            SetFunction( &EnemyBoss::�^�C�}�[����܂őҋ@ );
            return;
        }
    }
    void EnemyBoss::�{�X�o���̃G�t�F�N�g���I������܂őҋ@( cinder::CameraPersp const & camera )
    {
        if ( TRData::bossSpawn.IsComplete( ) )
        {
            TRData::bossSerif.TutorialStart( );
            timer.Advance( 180 );

            SetSerifFunction( u8"���ł�A�䂪���l��I", &EnemyBoss::�Z���t );
            SetFunction( &EnemyBoss::�o���������ɃG�l�~�[���X�|�[�������� );
            return;
        }
    }
    void EnemyBoss::�o���������ɃG�l�~�[���X�|�[��������( cinder::CameraPersp const & camera )
    {
        if ( frame % 5 == 0 )
        {
            if ( jsonItr != jsonEnemys.end( ) )
            {
                auto type = jsonItr->getValueForKey<std::string>( "type" );
                auto position = getVec3f( ( *jsonItr )["position"] );
                if ( type == "slashNot" )
                {
                    EnemyCreate<EnemySlashNot>( position, camera );
                }
                else if ( type == "bulletNot" )
                {
                    EnemyCreate<EnemyBulletNot>( position, camera );
                }
                else if ( type == "slashWalk" )
                {
                    EnemyCreate<EnemySlashWalk>( position, camera );
                }
                else if ( type == "bulletWalk" )
                {
                    EnemyCreate<EnemyBulletWalk>( position, camera );
                }
                else if ( type == "tutorial" )
                {
                    EnemyCreate<EnemyTutorial>( position, camera );
                }
                else if ( type == "bulletTutorial" )
                {
                    EnemyCreate<EnemyBulletTutorial>( position, camera );
                }
                else if ( type == "bullet" )
                {
                    EnemyCreate<EnemyBullet>( position, camera );
                }
                else if ( type == "slash" )
                {
                    EnemyCreate<EnemySlash>( position, camera );
                }
                else if ( type == "boss" )
                {
                    EnemyCreate<EnemyBoss>( position, camera );
                }
                jsonItr++;
            }
        }

        if ( timer.IsAction( ) )
        {
            TRData::bossSerif.TutorialEnd( );
            TRData::special.TutorialStart( );
            SetSerifFunction( u8"", &EnemyBoss::�k���Z���t );
            SetFunction( &EnemyBoss::�^�C�}�[����܂őҋ@ );
            return;
        }
    }
    void EnemyBoss::������|���ꂽ���̃Z���t( cinder::CameraPersp const & camera )
    {
        if ( timer.IsAction( ) )
        {
            SetSerifFunction( u8"", &EnemyBoss::�k���Z���t );
            SetFunction( &EnemyBoss::�^�C�}�[����܂őҋ@ );
            return;
        }
    }
    void EnemyBoss::���ʎ��̃Z���t( cinder::CameraPersp const & camera )
    {
        if ( timer.IsAction( ) )
        {
            texture = �|��郂�[�V����;
            timer.Advance( 60 );
            SetSerifFunction( u8"", &EnemyBoss::�k���Z���t );
            SetFunction( &EnemyBoss::���� );
        }
    }
    void EnemyBoss::�k���Z���t( )
    {
        /*nothing*/
    }
    void EnemyBoss::�Z���t( )
    {
        auto textSize = font.BoundingBox( serif ).getSize( );
        auto messageBoxHeight = messageBoxLeft->getHeight( );
        auto rectLeft = Rectf( Vec2f::zero( ), messageBoxLeft->getSize( ) );
        auto rectCenter = Rectf( Vec2f::zero( ), Vec2f( textSize.x, messageBoxHeight ) );
        auto rectRight = Rectf( Vec2f::zero( ), messageBoxRight->getSize( ) );

        int ue = 34;
        int inMessageHeight = 111;
        int fontStartHeightPosition = ue + ( inMessageHeight - textSize.y ) / 2;

        // ���b�Z�[�W�{�b�N�X�̕\��
        gl::pushModelView( );

        gl::translate( -( rectLeft.getWidth( ) + rectCenter.getWidth( ) + rectRight.getWidth( ) ) / 2, 0 );

        Vec2f position = env.getWindowCenter( );
        position.y = 300;
        gl::translate( position + Vec2f( 0, -messageBoxHeight ) );
        messageBoxLeft->enableAndBind( );
        gl::color( ColorA::white( ) );
        gl::drawSolidRect( rectLeft );
        messageBoxLeft->unbind( );

        gl::translate( Vec2f( rectLeft.getWidth( ), 0 ) );
        messageBoxCenter->enableAndBind( );
        gl::color( ColorA::white( ) );
        gl::drawSolidRect( rectCenter );
        messageBoxCenter->unbind( );

        gl::pushModelView( );
        gl::translate( 0, fontStartHeightPosition );
        font.Draw( serif, Vec2f::zero( ), Color::white( ), Fonts::Mode::LEFTUP );
        gl::popModelView( );

        gl::translate( Vec2f( rectCenter.getWidth( ), 0 ) );
        messageBoxRight->enableAndBind( );
        gl::color( ColorA::white( ) );
        gl::drawSolidRect( rectRight );
        messageBoxRight->unbind( );

        gl::popModelView( );
    }
    bool EnemyBoss::IsFrieldKilledSerif( )
    {
        return !isFrieldKilledSerif && status.HP != status.maxHP;
    }
    bool EnemyBoss::IsDeadSerif( )
    {
        return !isDeadSerif && isLive == false;
    }
    void EnemyBoss::SetFunction( void( EnemyBoss::* function )( cinder::CameraPersp const &camera ) )
    {
        behavior = std::bind( function, this, std::placeholders::_1 );
    }
    void EnemyBoss::SetSerifFunction( std::string const& serif, void( EnemyBoss::* function )( ) )
    {
        this->serif = serif;
        serifBehavior = std::bind( function, this );
    }
    void EnemyBoss::WavyBulletFiring( float radians, cinder::CameraPersp const & camera )
    {
        int attackPoint = 2;

        auto direction = Vec2f::xAxis( );
        direction.rotate( radians );

        float length = std::min( env.getWindowWidth( ), env.getWindowHeight( ) );
        length /= 2.0F;
        length -= 50;

        {
            float u = 0.5F;
            float v = 0.5F;
            auto ray = camera.generateRay( u, v, env.getWindowAspectRatio( ) );
            BulletCreate( EnemyBulletTexture( object.Position( ), ray.getOrigin( ) + ray.getDirection( ), "shuriken.png", attackPoint ) );
        }

        {
            float u = direction.x * length / env.getWindowWidth( ) + 0.5F;
            float v = direction.y * length / env.getWindowHeight( ) + 0.5F;
            auto ray = camera.generateRay( u, v, env.getWindowAspectRatio( ) );
            BulletCreate( EnemyBulletTexture( object.Position( ), ray.getOrigin( ) + ray.getDirection( ), "shuriken.png", attackPoint ) );
        }

        {
            float u = direction.x * -length / env.getWindowWidth( ) + 0.5F;
            float v = direction.y * -length / env.getWindowHeight( ) + 0.5F;
            auto ray = camera.generateRay( u, v, env.getWindowAspectRatio( ) );
            BulletCreate( EnemyBulletTexture( object.Position( ), ray.getOrigin( ) + ray.getDirection( ), "shuriken.png", attackPoint ) );
        }
    }
    void EnemyBoss::RandomWavyBulletFiring( cinder::CameraPersp const & camera )
    {
        int attackPoint = 2;

        Vec2f pos1, pos2, pos3;

        pos1.x = randFloat( env.getWindowWidth( )*0.25, env.getWindowWidth( )*0.4 ) / env.getWindowWidth( );
        pos1.y = randFloat( env.getWindowHeight( )*0.25, env.getWindowHeight( )*0.75 ) / env.getWindowHeight( );
        auto ray = camera.generateRay( pos1.x, pos1.y, env.getWindowAspectRatio( ) );
        BulletCreate( EnemyBulletTexture( object.Position( ), ray.getOrigin( ) + ray.getDirection( ), "shuriken.png", attackPoint ) );

        pos2.x = randFloat( env.getWindowWidth( )*0.6, env.getWindowWidth( )*0.75 ) / env.getWindowWidth( );
        pos2.y = randFloat( env.getWindowHeight( )*0.25, env.getWindowHeight( )*0.75 ) / env.getWindowHeight( );
        ray = camera.generateRay( pos2.x, pos2.y, env.getWindowAspectRatio( ) );
        BulletCreate( EnemyBulletTexture( object.Position( ), ray.getOrigin( ) + ray.getDirection( ), "shuriken.png", attackPoint ) );

        pos3.x = ( pos1.x + pos2.x ) / 2.0F;
        pos3.y = ( pos1.y + pos2.y ) / 2.0F;
        ray = camera.generateRay( pos3.x, pos3.y, env.getWindowAspectRatio( ) );
        BulletCreate( EnemyBulletTexture( object.Position( ), ray.getOrigin( ) + ray.getDirection( ), "shuriken.png", attackPoint ) );
    }

    void EnemyBoss::DrawAura( )
    {
        if ( auraTex )
        {
            auto object = this->object;
            object.Position( object.Position( ) - object.Direction( ) * 0.1 );

            gl::disable( GL_CULL_FACE );

            gl::pushModelView( );
            gl::translate( object.Position( ) );
            gl::multModelView( object.Quaternion( ).toMatrix44( ) );

            if ( !IsKnockBack( ) )
            {
                gl::pushModelView( );
                float s = 2.0F + ( 0.3F * math<float>::sin( float( frame ) / 2.5F ) );
                gl::scale( s, s, s );
                gl::rotate( Vec3f( 0, 180, 180 + frame ) );
                ColorA col = ColorA::white( );
                col.a = 0.5F + ( 0.5 * math<float>::cos( float( frame ) / 2.5F ) );
                gl::color( col );
                auraTex->bind( );
                gl::drawSolidRect( Rectf( -object.Size( ).xy( ) / 2.0F, object.Size( ).xy( ) / 2.0F ) );
                auraTex->unbind( );
                gl::popModelView( );
            }

            gl::popModelView( );

            gl::enable( GL_CULL_FACE );
        }
    }
}