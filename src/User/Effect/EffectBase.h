# pragma once

# include "EffectActor.hpp"

# include <memory>

# include "cinder/gl/Texture.h"
#include"../Utilitys/Hirasawa.h"
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
    private:
        cinder::gl::Texture* texture;
        int frame;
        int speed;
        cinder::Vec2f position;
        cinder::Vec2f cutSize;
        cinder::Vec2f size_;
        int maxIndex;
        Mode mode;
        bool alfamode;
    private:
        bool isActive;
    public:
        EffectBase( std::string const& path, cinder::Vec2f position, cinder::Vec2f size, cinder::Vec2f cutSize, Mode mode = Mode::LEFTUP, bool alfa_m = false );
    public:
        virtual void Update( ) override;
        virtual void Draw( ) override;
    public:
        void Position( cinder::Vec2f position );
        void CutSize( cinder::Vec2f cutSize );
    public:
        // �e�������Ă������̊m�F
        bool IsActive( );
    protected:
        int NowIndex( );
        cinder::Vec2f TextureCutPosition( int index );
        void DrawCutTexture( cinder::Vec2f cutPosition );
    protected:
        void Erase( );
    };

    using EffectBaseRef = std::shared_ptr<EffectBase>;
}