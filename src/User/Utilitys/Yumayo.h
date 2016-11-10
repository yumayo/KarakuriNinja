# pragma once

# include "cinder/Json.h"
# include "cinder/Vector.h"
# include "cinder/Color.h"

// �X���b�V���ɕK�v�ȃw�b�_�[�t�@�C���ł��B

# include "../SlashEffect/SlashEffect.h"
# include "../Attack/AttackFactory.h"
# include "cinder/app/TouchEvent.h"
# include "ZKOO.hpp"
//

# include "cinder/Font.h"

namespace User
{
    int getInt( cinder::JsonTree const& params );
    float getFloat( cinder::JsonTree const& params );
    std::string getString( cinder::JsonTree const& params );

    // JSON�̔z�񂩂�Vec3f��
    cinder::Vec3f getVec3f( cinder::JsonTree const& params );
    cinder::Vec2f getVec2f( cinder::JsonTree const& params );

    // JSON�̔z�񂩂�Color��
    cinder::Color getColor( cinder::JsonTree const& params );

    // JSON�̔z�񂩂�ColorA��
    cinder::Color getColorA( cinder::JsonTree const& params );

    class SlashInput
    {
        //============================================
        // ���̂S���X���b�V���𔭓������邽�߂Ɏg���܂��B
        //============================================
        Slash slashEffect;
        AttackFactory attackTask;
        Line line;
        bool isInput;
        //============================================
    public:
        SlashInput( );
    public:
        // �X���b�V���̃G�t�F�N�g��\�����܂��B
        void Draw( );
        // Input��begin��end�ň͂܂ꂽ�����̂݁AInput�̋@�\�������܂��B
        void Begin( );
        // Input��begin��end�ň͂܂ꂽ�����̂݁AInput�̋@�\�������܂��B
        void End( );
        // �X���b�V���Ƃ̓����蔻������I�u�W�F�N�g�������ɓ���Ă��������B
        // CheckDefLineOfCircle �̃��b�v�ł��B
        // Nomoto.h���Q�ƁB
        bool IsHitCircle( cinder::Vec2f pos, float size );
    private:
        // �X�N���[���^�b�`�ł̃A�b�v�f�[�g
        // �ȉ��̃X���b�V�����A�b�v�f�[�g���܂��B
        void UpdateAttackMotionOfTouch( );
        // �ǂ̈ʒu���^�b�`�����̂����o�������܂��B
        void SetAttackMotionOfTouch( uint32_t id, cinder::app::TouchEvent::Touch& touch );
        // �ǂ̈ʒu�Ő��������I�����̂��𔻒f���A�X���b�V���̒l���Z�b�g���܂��B
        void MakeAttackMotionOfTouch( uint32_t id, cinder::app::TouchEvent::Touch& touch );
    };

    class SlashLine
    {
        Slash slashEffect;
        Line line;
        bool isActive;
    public:
        SlashLine( ) { }
        SlashLine( ::Line line, Effect effect, int combo );
        void Draw( ) { slashEffect.Draw( ); }
        void Update( ) { slashEffect.Update( ); }
        bool IsThisErase( ) { return slashEffect.Active( ); }
        Line Line( ) { return line; }
        void UsedLine( ) { isActive = false; }
        bool IsActive( ) { return isActive; }
    };

    class MoveTouch
    {
    public:
        MoveTouch( ) { }
        MoveTouch( cinder::Vec2f pos );
        bool isActive;
        cinder::Vec2f prevPos;
    };

    class MoveInput
    {
        //============================================
        // ���̂S���X���b�V���𔭓������邽�߂Ɏg���܂��B
        //============================================
        std::list<SlashLine> slashEffect;
        std::map<uint32_t, MoveTouch> moveTouch;
        //============================================
    public:
        // �X���b�V���̃G�t�F�N�g��\�����܂��B
        void Draw( );
        // Input��begin��end�ň͂܂ꂽ�����̂݁AInput�̋@�\�������܂��B
        void Begin( int combo = 0 );
        // Input��begin��end�ň͂܂ꂽ�����̂݁AInput�̋@�\�������܂��B
        void End( );
        // �X���b�V���Ƃ̓����蔻������I�u�W�F�N�g�������ɓ���Ă��������B
        // CheckDefLineOfCircle �̃��b�v�ł��B
        // Nomoto.h���Q�ƁB
        bool IsHitCircle( cinder::Vec2f pos, float size );
        // �`������Ă���L���ȃ��C����S�ĕԂ��܂��B
        std::vector<Line> Lines( );
        // �`���r���̃��C����S�Ė����ɂ��܂��B
        void InputInvalidation( );
    private:
        bool IsHandMove( uint32_t id, cinder::Vec2f pos );
    private:
        // �X�N���[���^�b�`�ł̃A�b�v�f�[�g
        // �ȉ��̃X���b�V�����A�b�v�f�[�g���܂��B
        void UpdateAttackMotionOfTouch( int combo );
        // �ǂ̈ʒu���^�b�`�����̂����o�������܂��B
        void SetAttackMotionOfTouch( uint32_t id, cinder::app::TouchEvent::Touch& touch );
        // �ǂ̈ʒu�Ő��������I�����̂��𔻒f���A�X���b�V���̒l���Z�b�g���܂��B
        void MakeAttackMotionOfTouch( uint32_t id, cinder::app::TouchEvent::Touch& touch, int combo );
    };

    class Fonts
    {
        cinder::Font font;
    public:
        enum class Mode
        {
            LEFTUP, CENTERUP, RIGHTUP
        };
        Fonts( std::string const& path = u8"���C���I", float size = 24.0F );
        void SetFont( std::string const& path = u8"���C���I", float size = 24.0F );
        void Draw( std::string const& string, cinder::Vec2f position, cinder::ColorA color = cinder::ColorA::white( ), Mode mode = Mode::LEFTUP );
        cinder::Rectf BoundingBox( std::string const& string );
        cinder::gl::Texture MakeStringTexture( std::string const& string, float& baseLineOffset );
    };

    class Timer
    {
        int frame;
        int elapseFrame;
        bool isCount;
        bool isActive;
    public:
        Timer( );
        Timer( int elapseFrame );
    public:
        void Update( );
    public:
        bool IsCount( );
        bool IsActionOnePrevFrame( );
        bool IsAction( );
    public:
        void Advance( );
        void Advance( int elapseFrame );
    public:
        void On( );
        void Off( );
    public:
        double NomalizedFrame( );
    };
}