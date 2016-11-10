# pragma once

# include "EffectBase.h"

namespace User
{
    // Effect‚Í‰æ‘œ‚Ì¶ã‚ğŠî“_‚Æ‚µ‚Ä•\¦‚³‚ê‚Ü‚·B
    class EffectAlpha: public EffectBase
	{
    public:
		EffectAlpha( std::string const& path, cinder::Vec2f position, cinder::Vec2f size, cinder::Vec2f cutSize, Mode mode = Mode::LEFTUP);
	protected:
        virtual void DrawCutTexture( cinder::Vec2f cutPosition ) override;
    };
}