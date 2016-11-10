#include "EffectAlpha.h"

#include "Framework.hpp"

#include "GlobalData.hpp"

namespace User
{
	using namespace cinder;

	EffectAlpha::EffectAlpha(std::string const & path, cinder::Vec2f position, cinder::Vec2f size, cinder::Vec2f cutSize, Mode mode)
		: EffectBase(path, position, size, cutSize, mode)
	{

	}
	void EffectAlpha::DrawCutTexture(cinder::Vec2f cutPosition)
	{
		float alfa = Easing::getEasing[Easing::CubicIn](float(frame) / float(speed*(maxIndex - 1)), 1.f, 0.f);
		gl::color(ColorA(1, 1, 1, alfa));

		auto normalized = cutSize.normalized();
		auto normalized_2 = normalized / 2.0F;

		gl::pushModelView();
		gl::translate(position);
		gl::scale(size_);

		switch (mode)
		{
		case User::EffectAlpha::Mode::LEFTUP:
			gl::draw(*texture, Area(cutPosition, cutPosition + cutSize), Rectf(Vec2f::zero(), normalized));
			break;
		case User::EffectAlpha::Mode::CENTERCENTER:
			gl::draw(*texture, Area(cutPosition, cutPosition + cutSize), Rectf(-normalized_2, normalized_2));
			break;
		default:
			break;
		}

		gl::popModelView();
	}
}