# pragma once

# include "EffectBase.h"

namespace User
{
    // Effectは画像の左上を基点として表示されます。
    class EffectAlpha: public EffectBase
	{
    public:
		EffectAlpha( std::string const& path, cinder::Vec2f position, float depth, cinder::Vec2f size, cinder::Vec2f cutSize, Mode mode = Mode::LEFTUP);
	protected:
        virtual void DrawCutTexture( cinder::Vec2f cutPosition ) override;
    };
}