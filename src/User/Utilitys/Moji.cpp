#include "Moji.h"

# include "GlobalData.hpp"

# include "Hirasawa.h"

namespace User
{

	using namespace cinder;

	Moji::Data::Data(cinder::JsonTree const & params, std::string const& moji)
	{
		begin = getInt(params["begin"]);
		move = getInt(params["move"]);
		beginPosition = getVec2f(params["beginPosition"]);
		endPosition = getVec2f(params["endPosition"]);
		beginSize = getVec2f(params["beginSize"]);
		endSize = getVec2f(params["endSize"]);
		texture = &GData::FindTexture(getString(params["texture"]));
		this->moji = moji;
	}

	cinder::Vec2f Moji::Data::Position(int frame)
	{
		Vec2f ret;

		ret.x = EasingCubicOut(Frame(frame), beginPosition.x, endPosition.x);
		ret.y = EasingCubicOut(Frame(frame), beginPosition.y, endPosition.y);

		return ret;
	}
	cinder::Vec2f Moji::Data::Size(int frame)
	{
		Vec2f ret;

		ret.x = EasingCubicOut(Frame(frame), beginSize.x, endSize.x);
		ret.y = EasingCubicOut(Frame(frame), beginSize.y, endSize.y);

		return ret;
	}

	float Moji::Data::Frame(int frame)
	{
		auto ret = static_cast<float>(frame - begin) / move;
		if (ret < 0.0F)
		{
			return 0.0F;
		}
		else if (1.0F < ret)
		{
			return 1.0F;
		}
		else
		{
			return ret;
		}
	}

	Moji::Moji(Data data)
		: data(data)
	{
	}
	void Moji::Draw(int frame, Fonts& font)
	{
		auto pos = data.Position(frame);
		auto size = data.Size(frame);

		gl::pushModelView();
		gl::translate(pos);
		gl::color(Color::white());
		gl::draw(*data.Texture(), Rectf(-size / 2, size / 2));

		float temp;
		gl::color(Color::black());
		gl::draw(font.MakeStringTexture(data.GetMoji(), temp), Rectf(-size / 2, size / 2));

		gl::popModelView();
	}
}