# pragma once

# include "EffectActor.hpp"

# include <memory>

# include "cinder/gl/Texture.h"

namespace User
{
    // Effectは画像の左上を基点として表示されます。
    class EffectBase : public EffectActor
    {
    public:
        enum class Mode
        {
            LEFTUP, // 左上を起点として描画（通常の描画位置）
            CENTERCENTER // xyそれぞれの中心を基点として描画
        };
    private:
        cinder::gl::Texture* texture;
        int frame;
        int speed;
        cinder::Vec2f position;
        cinder::Vec2f cutSize;
        int maxIndex;
        Mode mode;
    private:
        bool isActive;
    public:
        EffectBase( std::string const& path, cinder::Vec2f position, cinder::Vec2f cutSize, Mode mode = Mode::LEFTUP );
    public:
        virtual void Update( ) override;
        virtual void Draw( cinder::CameraPersp const& camera ) override;
    public:
        void Position( cinder::Vec2f position );
        void CutSize( cinder::Vec2f cutSize );
    public:
        // 弾を消していいかの確認
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