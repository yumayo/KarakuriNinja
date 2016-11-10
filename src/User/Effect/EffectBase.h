# pragma once

# include "EffectActor.hpp"

# include <memory>

# include "cinder/gl/Texture.h"
#include"../Utilitys/Hirasawa.h"

// �G�t�F�N�g���G�t�F�N�g�̒��Ń\�[�g���ĕ\������悤�ɂ��܂����B
// ����ɂ��������Ĉ����� depth �������Ă��܂��B
// 2016.09.08

namespace User
{
    // Effect�͉摜�̍������_�Ƃ��ĕ\������܂��B
    class EffectBase : public EffectActor
    {
    public:
        enum class Mode
        {
            LEFTUP, // ������N�_�Ƃ��ĕ`��i�ʏ�̕`��ʒu�j
            CENTERCENTER // xy���ꂼ��̒��S����_�Ƃ��ĕ`��
        };
    protected:
        cinder::gl::Texture* texture;
        int frame;
        int speed;
        cinder::Vec2f position;
        float depth;
        cinder::Vec2f cutSize;
        cinder::Vec2f size;
        int maxIndex;
        Mode mode;
    protected:
        bool isActive;
    public:
        EffectBase( std::string const& path, cinder::Vec2f position, float depth, cinder::Vec2f size, cinder::Vec2f cutSize, Mode mode = Mode::LEFTUP );
    public:
        virtual void Update( ) override;
        virtual void Draw( ) override;
    public:
        void Position( cinder::Vec2f position );
        void CutSize( cinder::Vec2f cutSize );
    public:
        float GetDepth( ) { return depth; }
        // �e�������Ă������̊m�F
        bool IsActive( );
    protected:
        virtual int NowIndex( );
        virtual void DrawCutTexture( cinder::Vec2f cutPosition );
        cinder::Vec2f TextureCutPosition( int index );
    protected:
        void Erase( );
    };

    using EffectBaseRef = std::shared_ptr<EffectBase>;
}