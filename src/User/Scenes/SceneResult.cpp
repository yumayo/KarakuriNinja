# include "SceneResult.h"
# include "SceneTitle.h"

# include "cinder/Rand.h"
# include "GlobalData.hpp"

using namespace ci;
const int TABLEFONTSIZE = 90;
namespace User
{
    Fusuma::Fusuma( )
        : lefthusuma( &GData::FindTexture( "Textures/husuma0.png" ) )
        , righthusuma( &GData::FindTexture( "Textures/husuma1.png" ) )
        , husuma_t( 0.0f )
        , delaycount( 0 )
    {
    }

    void Fusuma::openFusuma( )
    {
        delaycount++;
        if ( delaycount > 90 )
            Easing::tCount( husuma_t, 1.f );
        lefthusumapos.x = Easing::getEasing[Easing::QuadIn]( husuma_t, env.getWindowWidth( ) / 4.f, -env.getWindowWidth( ) / 2.f );
        lefthusumapos.y = env.getWindowHeight( ) / 2.f;
        righthusumapos.x = Easing::getEasing[Easing::QuadIn]( husuma_t, env.getWindowWidth( )*( 3.f / 4.f ), 1.5f*env.getWindowWidth( ) );
        righthusumapos.y = env.getWindowHeight( ) / 2.f;
        husumasize = Vec2f( env.getWindowWidth( ) / 2.f, env.getWindowHeight( ) );
    }

    void Fusuma::drawFusuma( )
    {
        gl::pushModelView( );
        gl::translate( lefthusumapos );
        gl::color( Color::white( ) );
        lefthusuma->enableAndBind( );
        gl::drawSolidRect( Rectf( -husumasize / 2, husumasize / 2 ) );
        lefthusuma->disable( );
        gl::popModelView( );

        gl::pushModelView( );
        gl::translate( righthusumapos );
        gl::color( Color::white( ) );
        righthusuma->enableAndBind( );
        gl::drawSolidRect( Rectf( -husumasize / 2, husumasize / 2 ) );
        righthusuma->disable( );
        gl::popModelView( );
    }

    void Fusuma::closeFusuma( )
    {
        delaycount++;
        if ( delaycount > 90 )
            Easing::tCount( husuma_t, 1.f );
        lefthusumapos.x = Easing::getEasing[Easing::QuadIn]( husuma_t, -env.getWindowWidth( ) / 2.f, env.getWindowWidth( ) / 4.f );
        lefthusumapos.y = env.getWindowHeight( ) / 2.f;
        righthusumapos.x = Easing::getEasing[Easing::QuadIn]( husuma_t, 1.5f*env.getWindowWidth( ), env.getWindowWidth( )*( 3.f / 4.f ) );
        righthusumapos.y = env.getWindowHeight( ) / 2.f;
        husumasize = Vec2f( env.getWindowWidth( ) / 2.f, env.getWindowHeight( ) );


    }
    SceneResult::SceneResult( int _score, int maxcombo, int nowhp, int time )
        : score( _score )
		,maxcombo_(maxcombo)
		,nowhp_(nowhp)
		,time_(time)
        , font( u8"HG行書体", 120 )
        , tables( u8"HG行書体", TABLEFONTSIZE )
        , audio( &GData::FindAudio( "SE/result.wav" ) )
        , isEnd( false )
        , logoAlpha( 0.0f )
        , logoAlphaSpeed( 0.0125f )
        , texture( &GData::FindTexture( "title.png" ) )

        , background( &GData::FindTexture( "Textures/resultback.png" ) )
        , ninja( &GData::FindTexture( "Textures/resultninja.png" ) )
        , makimonotex( &GData::FindTexture( "Textures/makimono.png" ) )
        , makimonotex2( &GData::FindTexture( "Textures/makimono2.png" ) )
        , fire( &GData::FindTexture( "Textures/Effect/effect0.png" ) )
        , window( &GData::FindTexture( "Textures/window2.png" ) )
        , taiko1( &GData::FindAudio( "SE/taiko.wav" ) )
        , taiko2( &GData::FindAudio( "SE/taiko2.wav" ) )

        , flash_t( 0.0f )
        , flash_alfa( 1.0f )

        , rankin( false )
        , camera_t( 0.0f )
        , delay_t( 0.0f )
        , move_t( 0.0f )
        , myrank_camera_t( 0.0f )
    {
        //audio->Play( );
        for ( int i = 0; i < TS::MAX; i++ ) {
            t_[i] = 0.0f;
        }
        for ( int i = 0; i < SPE::E_MAX; i++ ) {
            s_end[i] = false;
        }
		for (int i = 0; i < 3; i++) {
			endt_[i] = false;
		}
		bgm = &GData::FindAudio("SE/resultbgm.wav");
		bgm->Gain(0.5f);
		bgm->Play();
        endscore = calcurateScore(_score,maxcombo,nowhp,time);
        player_scores.push_back( endscore );
        roadScores( );
        sortScores( );
        ranking = calcurateRanking( );
        datanum = player_scores.size( );
        writeScores( );
        pushIcon( );
    }

    SceneResult::~SceneResult( )
    {
        bgm->Stop( );
    }

    void SceneResult::resize( )
    {

    }

    void SceneResult::update( )
    {
        fusuma.openFusuma( );
        updateflash( );
        updateNinja( );
        updatecamera( );
        slashInput.Begin( );

        UpdateLogoAlpha( );

        // スラッシュとの当たり判定を取るには、円の中心ポジションと半径を入れます。
        if ( endt_[2] >= 1.0f ) isEnd = true;

        slashInput.End( );
    }
    void SceneResult::draw( )
    {
        beginDrawUI( );
        drawUI( );
        endDrawUI( );
    }

    void SceneResult::select( )
    {
		if (isEnd) {
			create(new SceneTitle());
		}

        if ( inputs.isPressKey( Key::KEY_LCTRL ) && inputs.isPushKey( Key::KEY_t ) )
        {
            create( new SceneTitle( ) );
            return;
        }
    }

    void SceneResult::UpdateLogoAlpha( )
    {
        logoAlpha += logoAlphaSpeed;

        if ( logoAlpha <= 0 || logoAlpha >= 1 )
            logoAlphaSpeed *= -1;
    }

    void SceneResult::beginDrawUI( )
    {
        ci::gl::enable( GL_TEXTURE_2D );
        ci::gl::disable( GL_CULL_FACE );
        ci::gl::disable( GL_NORMALIZE );
        ci::gl::disable( GL_LIGHTING );
        ci::gl::enableAlphaBlending( );
        ci::gl::disableDepthRead( );
        ci::gl::disableDepthWrite( );

        ci::gl::setViewport( env.getWindowBounds( ) );
        ci::gl::setMatricesWindow( env.getWindowSize( ) );
        ci::gl::color( Color::white( ) );
    }
    void SceneResult::drawUI( )
    {
        drawBackground( );
        drawTitleBase( );
        drawTitle( );
        drawFire( );
        drawNinja( );
        if ( t_[TS::RANKING] < 1 ) {
            drawWindow( );
            drawTables( );
            drawBeginScore( );
        }
        else {
            drawIcon( );
            drawMyIcon( );
        }
        drawflash( );
        fusuma.drawFusuma( );
		drawEnd();
        //slashInput.Draw( );
    }
    void SceneResult::endDrawUI( )
    {
    }

    void SceneResult::drawBackground( )
    {
        gl::pushModelView( );
        gl::translate( env.getWindowSize( ) / 2 );
        background->enableAndBind( );
        gl::color( ColorA( 1, 1, 1, 1 ) );
        gl::drawSolidRect( Rectf( -env.getWindowSize( ) / 2, env.getWindowSize( ) / 2 ) );
        background->disable( );
        gl::popModelView( );
    }

    void SceneResult::updateflash( )
    {
        if ( !fusuma.IsMoveFinished( ) ) return;
        Easing::tCount( flash_t, 1.f );
        flash_alfa = Easing::getEasing[Easing::QuadOut]( flash_t, 1.f, 0.f );
    }
    void SceneResult::drawflash( )
    {
        gl::pushModelView( );
        gl::translate( env.getWindowSize( ) / 2 );
        gl::color( ColorA( 1, 1, 1, flash_alfa ) );
        gl::drawSolidRect( Rectf( -env.getWindowSize( ) / 2, env.getWindowSize( ) / 2 ) );
        gl::popModelView( );
    }
    void SceneResult::drawNinja( )
    {
        if ( !fusuma.IsMoveFinished( ) ) return;
        gl::pushModelView( );
        gl::translate( ninjapos );
        ninja->enableAndBind( );
        gl::color( ColorA( 1, 1, 1, 1 ) );
        gl::drawSolidRect( Rectf( -ninjasize / 2, ninjasize / 2 ) );
        ninja->disable( );
        gl::popModelView( );
    }
    void SceneResult::drawWindow( )
    {
        if ( !fusuma.IsMoveFinished( ) ) return;
        gl::pushModelView( );
        Vec2f pos;
        Vec2f size = Vec2f( env.getWindowWidth( ) / 2.3f, env.getWindowHeight( ) / 1.4f );
        pos.x = Easing::getEasing[Easing::SineOut]( t_[TS::TABLE], env.getWindowWidth( ) *( 3 / 2.f ), env.getWindowWidth( )*( 3.f / 4.f ) );
        pos.y = env.getWindowHeight( ) - size.y / 2 * 1.1f;
        gl::translate( pos );
        window->enableAndBind( );
        gl::color( ColorA( 1, 1, 1, 1 ) );
        gl::drawSolidRect( Rectf( -size / 2, size / 2 ) );
        window->disable( );
        gl::popModelView( );
    }
    void SceneResult::updateNinja( )
    {
        if ( t_[TS::HONOO] >= 1.0f )
            Easing::tCount( t_[TS::NINJA], 0.5f );
		ninjasize.x = env.getWindowWidth() / 2.5f;
		ninjasize.y = ninjasize.x*(ninja->getSize().y / ninja->getSize().x);
        ninjapos.x = Easing::getEasing[Easing::SineOut]( t_[TS::NINJA], -env.getWindowWidth( ) / 2.f, env.getWindowWidth( ) / 4.f );
        ninjapos.y = env.getWindowHeight( ) - ninjasize.y / 2 * 1.1f;

    }
    void SceneResult::drawTitleBase( )
    {
        if ( flash_t >= 1.0f )
            Easing::tCount( t_[TS::MAKIMONO], 0.5f );
        Vec2f size = Vec2f( env.getWindowWidth( ) / 2.0f, env.getWindowHeight( ) / 3.5f );
        Vec2f pos = Vec2f( env.getWindowWidth( ) / 4, env.getWindowHeight( ) / 10.f );
        int cutsize = Easing::getEasing[Easing::Linear]( t_[TS::MAKIMONO], 0, makimonotex->getSize( ).x );
        if ( flash_t >= 1.0f )
            textureDraw( *makimonotex, pos, size, cinder::Area( 0, 0, cutsize, makimonotex->getSize( ).y ), 1 );

    }
    void SceneResult::drawFire( )
    {
        if ( t_[TS::MAKIMONO] >= 1.0f )
            Easing::tCount( t_[HONOO], 0.2f );
        int count = Easing::getEasing[Easing::Linear]( t_[TS::HONOO], 0, 8 );
        Vec2f size = Vec2f( env.getWindowWidth( ) / 2.5f, env.getWindowHeight( ) / 4.0f );
        Vec2f pos = Vec2f( env.getWindowWidth( ) / 4.f, env.getWindowHeight( ) / 10.f );
        gl::pushModelView( );
        gl::translate( pos );
        gl::scale( Vec2f( size.x / fire->getSize( ).x, size.y / ( fire->getSize( ).y / 8.f ) ) );
        cinder::gl::color( cinder::ColorA( 1, 1, 1, 1 ) );
        int countspeed = 2;
        int index = ( count / countspeed ) % 8;
        float start_y = count*( fire->getSize( ).y / 8.f );
        if ( t_[TS::MAKIMONO] >= 1.0f )
            cinder::gl::draw( *fire,
                              cinder::Area( 0, start_y, fire->getSize( ).x, start_y + int( fire->getSize( ).y / 8 ) ),
                              cinder::Rectf( -size / 2, size / 2 ) );
        gl::popModelView( );
    }
    void SceneResult::drawTitle( )
    {
        if ( t_[HONOO] < 0.5f )return;
        if ( t_[TS::HONOO] >= 1.0f )
            Easing::tCount( t_[TITLE], 0.4f );
        float redcolor = Easing::getEasing[Easing::CircIn]( t_[TS::TITLE], 1, 0 );
        Vec2f pos = Vec2f( env.getWindowWidth( ) / 4.f, env.getWindowHeight( ) / 20 );
        font.Draw( u8"結果発表！", pos + Vec2f( 30,15 ), ColorA( redcolor, 0, 0, 1 ), User::Fonts::Mode::CENTERUP );

    }
    void SceneResult::drawTables( )
    {
        if ( t_[TS::NINJA] < 1.0f )return;
        Easing::tCount( t_[TS::TABLE], 0.4f );
        Vec2f scorepos;
        float rate = 1.17f;
        float endpos_x = env.getWindowWidth( ) / 2.f + 120;
        scorepos.x = Easing::getEasing[Easing::SineOut]( t_[TS::TABLE], env.getWindowWidth( )*( 3.f / 2.f ), endpos_x );
        scorepos.y = env.getWindowHeight( ) / 2.7f - TABLEFONTSIZE / 2;
        tables.Draw( u8"スコア", scorepos, ColorA( 0, 0, 0, 1 ), User::Fonts::Mode::LEFTUP );

        Vec2f combopos;
        combopos.x = Easing::getEasing[Easing::SineOut]( t_[TS::TABLE], env.getWindowWidth( )*( 3.f / 2.f ), endpos_x );
        combopos.y = scorepos.y + TABLEFONTSIZE*rate;
        tables.Draw( u8"最高コンボ", combopos, ColorA( 0, 0, 0, 1 ), User::Fonts::Mode::LEFTUP );

        Vec2f hppos;
        hppos.x = Easing::getEasing[Easing::SineOut]( t_[TS::TABLE], env.getWindowWidth( )*( 3.f / 2.f ), endpos_x );
        hppos.y = scorepos.y + TABLEFONTSIZE*rate * 2;
        tables.Draw( u8"残り体力", hppos, ColorA( 0, 0, 0, 1 ), User::Fonts::Mode::LEFTUP );

        Vec2f timepos;
        timepos.x = Easing::getEasing[Easing::SineOut]( t_[TS::TABLE], env.getWindowWidth( )*( 3.f / 2.f ), endpos_x );
        timepos.y = scorepos.y + TABLEFONTSIZE*rate * 3;
        tables.Draw( u8"タイム", timepos, ColorA( 0, 0, 0, 1 ), User::Fonts::Mode::LEFTUP );

        Vec2f endscorepos;
        endscorepos.x = Easing::getEasing[Easing::SineOut]( t_[TS::TABLE], env.getWindowWidth( )*( 3.f / 2.f ), endpos_x );
        endscorepos.y = scorepos.y + TABLEFONTSIZE*rate * 4;
        tables.Draw( u8"最終スコア", endscorepos, ColorA( 0, 0, 0, 1 ), User::Fonts::Mode::LEFTUP );

        Vec2f rankingpos;
        rankingpos.x = Easing::getEasing[Easing::SineOut]( t_[TS::TABLE], env.getWindowWidth( )*( 3.f / 2.f ), endpos_x );
        rankingpos.y = scorepos.y + TABLEFONTSIZE*rate * 5;
        tables.Draw( u8"順位", rankingpos, ColorA( 0, 0, 0, 1 ), User::Fonts::Mode::LEFTUP );
    }
    void SceneResult::drawBeginScore( )
    {
        float rate = 1.17f;
        float time = 0.3f;
        if ( t_[TS::TABLE] < 1.0f )return;
        soundplay( taiko1, s_end[SPE::E_SCORE] );
        Easing::tCount( t_[TS::SCORE], time );
        Vec2f scorepos = Vec2f( env.getWindowWidth( ) - 100, env.getWindowHeight( ) / 2.7f - TABLEFONTSIZE / 2 );
        tables.Draw( std::to_string( score ), scorepos, ColorA( 0, 0, 0, 1 ), User::Fonts::Mode::RIGHTUP );

        if ( t_[TS::SCORE] < 1.0f )return;
        soundplay( taiko1, s_end[SPE::E_COMBO] );
        Easing::tCount( t_[TS::COMBO], time );
        Vec2f combopos = Vec2f( scorepos.x, scorepos.y + TABLEFONTSIZE*rate * 1 );
        tables.Draw( std::to_string( maxcombo_ ), combopos, ColorA( 0, 0, 0, 1 ), User::Fonts::Mode::RIGHTUP );

        if ( t_[TS::COMBO] < 1.0f )return;
        soundplay( taiko1, s_end[SPE::E_HP] );
        Easing::tCount( t_[TS::HP], time );
        Vec2f  hppos = Vec2f( scorepos.x, scorepos.y + TABLEFONTSIZE*rate * 2 );
        tables.Draw( std::to_string( nowhp_), hppos, ColorA( 0, 0, 0, 1 ), User::Fonts::Mode::RIGHTUP );


        if ( t_[TS::HP] < 1.0f )return;
        soundplay( taiko1, s_end[SPE::E_TIME] );
        Easing::tCount( t_[TS::TIME], time );
        Vec2f timepos = Vec2f( scorepos.x, scorepos.y + TABLEFONTSIZE*rate * 3 );
        tables.Draw( std::to_string(time_), timepos, ColorA( 0, 0, 0, 1 ), User::Fonts::Mode::RIGHTUP );


        if ( t_[TS::TIME] < 1.0f )return;
        soundplay( taiko1, s_end[SPE::E_LASTSCORE] );
        Easing::tCount( t_[TS::LASTSCORE], time*2.5f );
        Vec2f endscorepos = Vec2f( scorepos.x, scorepos.y + TABLEFONTSIZE*rate * 4 );
        tables.Draw( std::to_string( endscore ), endscorepos, ColorA( 0, 0, 0, 1 ), User::Fonts::Mode::RIGHTUP );


        if ( t_[TS::LASTSCORE] < 1.0f )return;
        soundplay( taiko2, s_end[SPE::E_RANKING] );
        Easing::tCount( t_[TS::RANKING], 1.5f );
        Vec2f rankingpos = Vec2f( scorepos.x, scorepos.y + TABLEFONTSIZE*rate * 5 );
        tables.Draw( std::to_string( ranking ) + u8"位/" + std::to_string( datanum ) + u8"人", rankingpos, ColorA( 0, 0, 0, 1 ), User::Fonts::Mode::RIGHTUP );
    }

    void SceneResult::textureDraw( cinder::gl::Texture const & texture, cinder::Vec2f position, cinder::Vec2f realsize, cinder::Area area, float size )
    {
        ci::gl::pushModelView( );
        ci::gl::translate( position.x, position.y );
        cinder::gl::pushModelView( );
        ci::gl::scale( realsize / texture.getSize( ) );
        ci::gl::translate( -texture.getSize( ).x / 2, -texture.getSize( ).y / 2 );
        float alfa = 1.f;
        cinder::gl::color( cinder::ColorA( 1, 1, 1, alfa ) );
        cinder::gl::draw( texture,
                          cinder::Area( area.x1, area.y1, area.x1 + area.x2 * size, area.y1 + area.y2 ),
                          cinder::Rectf( area.x1, area.y1, area.x1 + area.x2 * size, area.y1 + area.y2 ) );
        cinder::gl::color( cinder::ColorA( 1, 1, 1, 1 ) );
        texture.unbind( );
        cinder::gl::popModelView( );
        ci::gl::popModelView( );
    }
    void SceneResult::soundplay( Audio * audio, bool & flag )
    {
        if ( !flag ) {
            audio->Play( );
            flag = true;
        }
    }
    void SceneResult::pushIcon( )
    {
        int scorebuf;
        int bufranking;
        for ( int i = 0; i < 100; i++ ) {
            float rate = 1.15f;
            Icon icon;
            icon.size = Vec2f( 700, 120 );
            icon.pos = Vec2f( env.getWindowWidth( )*( 3.f / 4.f ), 120 + i*icon.size.y*1.15f );
            if ( i < player_scores.size( ) ) {
                icon.isscoredraw = true;
                icon.score = player_scores[i];
                if ( i == 0 ) {
                    scorebuf = player_scores[i];
                    bufranking = 1;
                    icon.rank = bufranking;
                }
                else {
                    if ( scorebuf == icon.score ) {
                        icon.rank = bufranking;
                    }
                    else {
                        icon.rank = i + 1;
                        bufranking = i + 1;
                    }
                }
                if ( !( i == 0 ) ) {
                    scorebuf = player_scores[i];
                }
                if ( ranking == ( i + 1 ) ) {
                    icon.ismy = true;
                    rankin = true;
                    myrankpos_ = icon.pos.y;
                }
                else {
                    icon.ismy = false;
                }
                icons.push_back( icon );
            }
            else {
                icon.rank = 0;
                icon.score = 0;
                icon.isscoredraw = false;
                icons.push_back( icon );
            }
        }
    }
    int SceneResult::calcurateScore(int score, int maxcombo, int nowhp, int time)
    {
		int endscore = score + (50*maxcombo) + (100 * nowhp) - (time * 20);
		return endscore;
    }
    void SceneResult::roadScores( )
    {
        std::ifstream scorefile( env.getAssetPath( "savedata/score.txt" ).string( ) );
        while ( !scorefile.eof( ) ) {
            int scorebuf;
            scorefile >> scorebuf;
            player_scores.push_back( scorebuf );
        }
    }
    void SceneResult::sortScores( )
    {
        std::sort( player_scores.begin( ), player_scores.end( ), std::greater<int>( ) );
    }
    int SceneResult::calcurateRanking( )
    {
        for ( int i = 0; i < player_scores.size( ); i++ ) {
            if ( player_scores[i] == endscore ) {
                return i + 1;
            }
        }
        return player_scores.size( );//絶対ありえないです
    }
    void SceneResult::writeScores( )
    {
        std::ofstream scorefile( env.getAssetPath( "savedata/score.txt" ).string( ) );
        for ( int i = 0; i < player_scores.size( ); i++ ) {
            scorefile << player_scores[i];
            if ( !( i == ( player_scores.size( ) - 1 ) ) )
                scorefile << "\n";
        }
    }
    void SceneResult::drawIcon( )
    {
        for ( int i = 0; i < 100; i++ ) {
            gl::pushModelView( );
            Vec2f pos = icons[i].pos + Vec2f( 0, iconcamera );
            gl::translate( pos );
            if ( !( ranking == ( i + 1 ) ) || delay_t >= 1.0f ) {
                if ( ( pos.y > -200 ) && ( pos.y < env.getWindowHeight( ) + 200 ) ) {
                    gl::color( ColorA( 1, 1, 1, 1 ) );
                    makimonotex2->enableAndBind( );
                    gl::drawSolidRect( Rectf( -icons[i].size / 2, icons[i].size / 2 ) );
                    makimonotex2->disable( );
                }
            }
            gl::popModelView( );
            if ( !( ranking == ( i + 1 ) ) || delay_t >= 1.0f ) {
                if ( ( pos.y > -200 ) && ( pos.y < env.getWindowHeight( ) + 200 ) && icons[i].isscoredraw ) {
                    tables.Draw( std::to_string( icons[i].rank )+u8"位", pos + Vec2f( -icons[i].size.x / 2 + 80, -tables.BoundingBox( std::to_string( icons[i].rank ) ).getHeight( ) / 2.2F ), ColorA( 0, 0, 0, 1 ), User::Fonts::Mode::LEFTUP );
                }
                if ( ( pos.y > -200 ) && ( pos.y < env.getWindowHeight( ) + 200 ) && icons[i].isscoredraw ) {
                    tables.Draw( std::to_string( icons[i].score ), pos + Vec2f( icons[i].size.x / 2 - 70, -tables.BoundingBox( std::to_string( icons[i].rank ) ).getHeight( ) / 2.2F ), ColorA( 0, 0, 0, 1 ), User::Fonts::Mode::RIGHTUP );
                }
            }
        }
    }
    void SceneResult::updatecamera( )
    {
        if ( rankin == false ) {
            float beginpos = -icons[0].size.y*1.15f * ( float( icons.size( ) ) + 0.5f );
            float endpos = 0;
            if ( t_[TS::RANKING] < 1 )return;
            Easing::tCount( camera_t, 2.5f );
            iconcamera = Easing::getEasing[Easing::QuintInOut]( camera_t, beginpos, endpos );
        }
        else {
            float beginpos = -icons[0].size.y*1.15f * ( float( icons.size( ) ) + 0.5f );
            float endpos = -myrankpos_ + env.getWindowHeight( ) / 2;
            if ( t_[TS::RANKING] < 1 )return;
            Easing::tCount( camera_t, 1.5f );
            iconcamera = Easing::getEasing[Easing::CubicInOut]( camera_t, beginpos, endpos );
            if ( ( camera_t >= 1.0f ) && ( delay_t < 1.0f ) ) {
                Easing::tCount( move_t, 0.5f );
                if ( move_t >= 1.0f )
                    Easing::tCount( delay_t, 1.0f );
            }
            if ( delay_t >= 1.0f ) {
                beginpos = -myrankpos_ + env.getWindowHeight( ) / 2;
                endpos = 0;
                iconcamera = Easing::getEasing[Easing::CubicInOut]( myrank_camera_t, beginpos, endpos );
                Easing::tCount( myrank_camera_t, 1.0f );
            }
        }
    }
    void SceneResult::drawMyIcon( )
    {
        if ( rankin == true ) {
            if ( ( camera_t >= 1.0f ) && ( delay_t < 1.0f ) ) {
                Vec2f size = Vec2f( 700, 120 );
                Vec2f pos;
                Vec2f beginpos = Vec2f( -env.getWindowWidth( )*( 3.f / 2.f ), env.getWindowHeight( ) / 2 );
                Vec2f endpos = Vec2f( env.getWindowWidth( )*( 3.f / 4.f ), env.getWindowHeight( ) / 2 );
                pos.x = Easing::getEasing[Easing::CubicOut]( move_t, beginpos.x, endpos.x );
                pos.y = env.getWindowHeight( ) / 2;
                gl::pushModelView( );
                gl::translate( pos );
				gl::color(ColorA(1, 1, 1, 1));
                makimonotex2->enableAndBind( );
                gl::drawSolidRect( Rectf( -size / 2, size / 2 ) );
                makimonotex2->disable( );
                gl::popModelView( );
                tables.Draw( std::to_string( icons[ranking - 1].rank ) + u8"位", pos + Vec2f( -icons[ranking - 1].size.x / 2 + 80, -tables.BoundingBox( std::to_string( icons[ranking - 1].rank ) ).getHeight( ) / 2.2F ),
					ColorA(0.5f+0.5f*sin(delay_t*20.f+(M_PI*(3.f/2.f))), 0, 0, 1), User::Fonts::Mode::LEFTUP );
                tables.Draw( std::to_string( icons[ranking - 1].score ), pos + Vec2f( icons[ranking - 1].size.x / 2 - 70, -tables.BoundingBox( std::to_string( icons[ranking - 1].rank ) ).getHeight( ) / 2.2F ),
					ColorA(0.5f + 0.5f*sin(delay_t*20.f + (M_PI*(3.f / 2.f))), 0, 0, 1), User::Fonts::Mode::RIGHTUP );
            }
        }
    }

	void SceneResult::drawEnd()
	{
		if (rankin) {
			if (myrank_camera_t >= 1) {
				Easing::tCount(endt_[0],0.5f);
			}
		}
		else {
			if (camera_t >= 1) {
				Easing::tCount(endt_[0], 0.5f);
			}
		}
		if (endt_[0] >= 1.0f) {
			Vec2f size = Vec2f(700,160);
			Easing::tCount(endt_[1], 2.f);
			gl::pushModelView();
			gl::translate(env.getWindowSize()/2);
			gl::color(ColorA(1,1,1,1));
			window->enableAndBind();
			gl::drawSolidRect(Rectf(-size / 2, size / 2));
			window->disable();
			gl::popModelView();
			std::string str = u8"おつかれさまでした";
			tables.Draw(str, env.getWindowSize() / 2 +Vec2f(0,-tables.BoundingBox(str).getHeight() / 2.2F),Color::black(),User::Fonts::Mode::CENTERUP);
		}
		if (endt_[1] >= 1.0f) {
			Easing::tCount(endt_[2], 2.f);
			gl::pushModelView();
			gl::translate(env.getWindowSize() / 2);
			gl::color(ColorA(0, 0, 0, endt_[2]));
			gl::drawSolidRect(Rectf(-env.getWindowSize()/2 , env.getWindowSize() / 2));
			gl::popModelView();
			bgm->Gain(0.5f-0.5f*endt_[2]);
		}
	}

}