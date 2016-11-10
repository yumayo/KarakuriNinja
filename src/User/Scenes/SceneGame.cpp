# include "SceneGame.h"
# include "SceneResult.h"

# include "ZKOO.hpp"
# include "cinder/Rand.h"

namespace User
{
    using namespace cinder;

    SceneGame::SceneGame( )
    {
        cameraEyePosition = Vec3f( 0, 0.7, -5 );
        camera.lookAt( cameraEyePosition, Vec3f( 0, 0.7, 0 ) );
        camera.setWorldUp( Vec3f( 0, 1, 0 ) );
        camera.setPerspective( 60.0F, env.getWindowAspectRatio( ), 1.0F, 100.0F );

        // ���[�}�����Ǘ�������̂��쐬�B
        fieldManager = std::make_shared<FieldManager>( );
        enemyManager = std::make_shared<EnemyManager>( camera, fieldManager->FieldDataPath( ) );
        enemyBulletManager = std::make_shared<EnemyBulletManager>( );
        UI = std::make_shared<Interface>( );
        gameClearFrame = 60 * 3;
        �J������h�炷 = false;
        damageColor = ColorA( 1, 0, 0, 0 );

        player = Player( 100, 100 );
        mainbgm.push_back( Audio( "BGM/mainbgm0.wav" ) );
        mainbgm[0].Looping( true );
        mainbgm[0].Gain( 0.4 );
        mainbgm[0].Play( );
    }
    SceneGame::~SceneGame( )
    {

    }
    void SceneGame::resize( )
    {
        camera.setAspectRatio( env.getWindowAspectRatio( ) );
    }

    void SceneGame::UpdatePlayer( )
    {
        player.Update( );
        player.AttackPhase( );

        int playerHP = player.NowHp( );

        if ( player.Command( ) != CommandType::GUARD )
        {
            player.TranseNowHp( -enemyManager->EnemyToPlayerDamage( camera ) );
            player.TranseNowHp( -enemyBulletManager->EnemyToPlayerDamage( camera ) );
        }

        if ( player.Command( ) == CommandType::GUARD )
        {
            player.TranseNowHp( -enemyManager->EnemyToPlayerDamage( player.GuardLine( ), camera ) );
            player.TranseNowHp( -enemyBulletManager->EnemyToPlayerDamage( player.GuardLine( ), camera ) );
        }

        if ( playerHP != player.NowHp( ) )
        {
            �J������h�炷 = true;
            damageColor.a += 0.2;
            timer.On( );
            timer.Advance( 60 );
        }

        if ( player.IsAttack( ) == true )
        {
            player.TranseNowMp( enemyManager->PlayerToEnemyDamage( player.MakeLine( ), camera ) );
            player.TranseNowMp( enemyBulletManager->PlayerToEnemyDamage( player.MakeLine( ), camera ) );
            player.ShiftIsAttack( );
        }
    }
    void SceneGame::UpdateColor( )
    {
        damageColor.a = std::max( damageColor.a - 0.01F, 0.0F );
    }
    void SceneGame::UpdateScore( )
    {
        UI->AddScore( enemyManager->ScoreRecovery( ) );
        UI->AddScore( enemyBulletManager->ScoreRecovery( ) );
    }
    void SceneGame::update( )
    {
        inputzkoo.Resumption( );
        if ( !inputzkoo.IsActive( ) ) return;

        // �X�y�V�����͍ŏ��ɃA�b�v�f�[�g���܂��B
        // �G�l�~�[�����Ȃ��ꍇ�̓X�y�V�����𔭓��ł��Ȃ��悤�ɂ��܂��B
        if ( !enemyManager->IsEmpty( ) )
        {
            special.update( player.NowMp( ) == player.MaxMp( ) );
        }

        // �v���C���[�̃X�y�V�����G�t�F�N�g���ɃJ������h�炵�܂��B
        if ( special.isEffecting( ) )
        {
            camera.lookAt( Vec3f( randFloat( -0.05F, 0.05F ), randFloat( -0.05F, 0.05F ), 0.0F ) + cameraEyePosition, camera.getCenterOfInterestPoint( ) );
            player.TranseNowMp( -1 );
        }

        // �G�t�F�N�g�I�����A�J���������ʂ�ɂ��܂��B
        if ( special.getEffectEnd( ) )
        {
            camera.lookAt( cameraEyePosition, camera.getCenterOfInterestPoint( ) );
        }

        timer.Update( );

        if ( �J������h�炷 )
        {
            camera.lookAt( Vec3f( randFloat( -0.05F, 0.05F ), randFloat( -0.05F, 0.05F ), 0.0F ) + cameraEyePosition, camera.getCenterOfInterestPoint( ) );
        }

        if ( timer.IsAction( ) )
        {
            timer.Off( );
            camera.lookAt( cameraEyePosition, camera.getCenterOfInterestPoint( ) );
            �J������h�炷 = false;
        }

        // �G�t�F�N�g�I�����A�S�ẴG�l�~�[�y�ђe�Ƀ_���[�W��^���܂��B
        if ( special.getEffectEnd( ) )
        {
            const float damagevalue = 10.0F;
            player.TranseNowMp( enemyManager->PlayerSpecialAttackToEnemyDamage( special.getspecialPower( ) * damagevalue ) );
            player.TranseNowMp( enemyBulletManager->PlayerSpecialAttackToEnemyDamage( ) );
        }

        // �X�y�V�����I��������A�G�t�F�N�g���I���܂őS�Ă̓�����~�߂܂��B
        if ( !special.getIsSpecial( ) )
        {
            fieldManager->Update( );
            enemyManager->update( camera );
            enemyBulletManager->BulletRegister( enemyManager->BulletsRecovery( ) );
            enemyBulletManager->update( );
            UpdatePlayer( );
            UpdateColor( );
            UpdateScore( );
        }

        // �G�l�~�[���S�ł�����A���̃X�e�[�W�������B
        if ( enemyManager->IsEmpty( ) )
        {
            fieldManager->End( );
        }

        // �X�e�[�W�̈ړ����I�������玟�̃X�e�[�W�ցB
        if ( fieldManager->IsChange( ) )
        {
            fieldManager->ChangeField( );
            enemyManager = std::make_shared<EnemyManager>( camera, fieldManager->FieldDataPath( ) );
            enemyBulletManager = std::make_shared<EnemyBulletManager>( );
        }

        // ���X�{�X��|���I�������A���炩�̃A�N�V������A���̃V�[���ցB
        if ( enemyManager->IsEmpty( ) && fieldManager->IsLastField( ) )
        {
            gameClearFrame = std::max( gameClearFrame - 1, 0 );
            camera.lookAt( Vec3f( randFloat( -0.05F, 0.05F ), randFloat( -0.05F, 0.05F ), 0.0F ) + cameraEyePosition, camera.getCenterOfInterestPoint( ) );
        }
    }
    void SceneGame::draw( )
    {
        beginDrawMain( );
        drawMain( );
        endDrawMain( );

        beginDrawUI( );
        drawUI( );
        endDrawUI( );
    }
    void SceneGame::select( )
    {
        if ( player.NowHp( ) <= 0.0F )
        {
            create( new SceneResult( UI->Score( ) ) );
            return;
        }

        if ( inputs.isPressKey( Key::KEY_LCTRL ) && inputs.isPushKey( Key::KEY_e ) )
        {
            create( new SceneResult( UI->Score( ) ) );
            return;
        }

        if ( gameClearFrame == 0 )
        {
            create( new SceneResult( UI->Score( ) ) );
            return;
        }
    }
    void SceneGame::beginDrawMain( )
    {
        gl::pushMatrices( );
        gl::enable( GL_TEXTURE_2D );
        gl::enable( GL_CULL_FACE );

        gl::enableAlphaBlending( );

        gl::enableDepthWrite( );
        gl::enableDepthRead( );

        //gl::enable( GL_NORMALIZE );
        //gl::enable( GL_LIGHTING );
        //glLightModelfv( GL_LIGHT_MODEL_AMBIENT, Color::black( ) );
        //glLightModeli( GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR );

        gl::setViewport( env.getWindowBounds( ) );
    }
    void SceneGame::drawMain( )
    {
        gl::setMatrices( camera );

    #ifdef _DEBUG
        gl::drawCoordinateFrame( );
    #endif // _DEBUG

        fieldManager->Draw( );

        enemyManager->draw( );
        enemyBulletManager->draw( );
    }
    void SceneGame::endDrawMain( )
    {
        gl::popMatrices( );
    }
    void SceneGame::beginDrawUI( )
    {
        gl::pushMatrices( );
        gl::enable( GL_TEXTURE_2D );
        gl::disable( GL_CULL_FACE );
        gl::disable( GL_NORMALIZE );
        gl::disable( GL_LIGHTING );
        gl::enableAlphaBlending( );
        gl::disableDepthRead( );
        gl::disableDepthWrite( );

        gl::setViewport( env.getWindowBounds( ) );
        gl::setMatricesWindow( env.getWindowSize( ) );
        gl::color( Color::white( ) );
    }
    void SceneGame::drawUI( )
    {
        // �G��A�o���b�g�̓����蔻��̈��`�悵�܂��B
    #ifdef _DEBUG
        enemyManager->DrawCollisionCircle( camera );
        enemyBulletManager->DrawCollisionCircle( camera );
    #endif

        // �`�揇�� �v���C���[ �G�l�~�[ UI �X�y�V�����̏��ɂ��܂��B
        player.Draw( );

        enemyManager->drawUI( );

        enemyBulletManager->DrawBulletCircle( camera );

        UI->draw( player.NormalizedMp( ), player.NormalizedHp( ) );

        special.draw( );

        gl::color( damageColor );
        gl::drawSolidRect( Rectf( Vec2f::zero( ), env.getWindowSize( ) ) );

        // ��ʂ𔖂����œh��Ԃ��B
        if ( !inputzkoo.IsActive( ) )
        {
            gl::color( ColorA( 0, 0, 0, 0.5 ) );
            gl::drawSolidRect( Rectf( Vec2f::zero( ), env.getWindowSize( ) ) );
            gl::color( Color( 1, 1, 1 ) );
            gl::drawSolidRect( Rectf( Vec2f( 200, 400 ), env.getWindowSize( ) ) );
        }

        // ZKOO�̕\��
        auto hand = inputzkoo.hand( );
        for ( auto& i : inputzkoo.GetHandleIDs( ) )
        {
            if ( inputzkoo.isRecognition( i, hand ) )
            {
                gl::color( Color( 1, 1, 0 ) );
                gl::drawSolidCircle( hand.Position( ), 100, 50 );
            }
            if ( inputzkoo.isPress( i, hand ) )
            {
                gl::color( Color( 1, 1, 1 ) );
                gl::drawSolidCircle( hand.Position( ), 50, 50 );
            }
            if ( inputzkoo.isPush( i, hand ) )
            {
                if ( !inputzkoo.IsActive( ) )
                {
                    if ( inputzkoo.IsHandsActive( ) ) inputzkoo.Resumption( );
                }
            }
        }
}
    void SceneGame::endDrawUI( )
    {
        gl::popMatrices( );
    }
}