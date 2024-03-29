# pragma once

# include "EffectBase.h"

# include <memory>

# include "cinder/gl/Texture.h"
#include"../Utilitys/Hirasawa.h"
namespace User
{
	// Effectは画像の左上を基点として表示されます。
	class EffectCombo : public EffectBase
	{
		cinder::Vec2f startsize_;
		cinder::Vec2f startpos_;
		cinder::Vec2f endpos_;
		float t_;
		float endt_;
		bool easingstart_;
	public:
		EffectCombo(
			std::string const& path, 
			cinder::Vec2f position, 
            float depth,
			cinder::Vec2f size, 
			cinder::Vec2f cutSize, 
			Mode mode,
			cinder::Vec2f endpos);
	public:
		virtual void Update() override;
		virtual void Draw() override;
	protected:
		virtual int NowIndex() override;
		virtual void DrawCutTexture(cinder::Vec2f cutPosition) override;
	};
}