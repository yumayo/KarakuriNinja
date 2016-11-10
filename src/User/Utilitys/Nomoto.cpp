#include "Nomoto.h"

LineSegment MakeLine(Line& line_)
{
	LineSegment line;
	line.slope = (line_.startPos.y - line_.endPos.y) / (line_.startPos.x - line_.endPos.x);
	line.heightVal = line_.startPos.y - line.slope * line_.startPos.x;
	line.startPos = line_.startPos;
	line.endPos = line_.endPos;
	return line;
}

LineSegment MakeLine(ci::Vec2f& startPos_, ci::Vec2f& endPos_)
{
	LineSegment line;
	line.slope = (startPos_.y - endPos_.y) / (startPos_.x - endPos_.x);
	//if (abs(startPos_.x) == abs(endPos_.x))line.slope = startPos_.y;
	line.heightVal = startPos_.y - line.slope * startPos_.x;
	if (startPos_.x == endPos_.x)line.heightVal = 0;
	line.startPos = startPos_;
	line.endPos = endPos_;
	return line;
}

ci::Vec2f CrossLine(LineSegment& line1, LineSegment& line2)
{
	if (abs(line1.slope) - abs(line2.slope) == 0.0f)
	{
		line2.slope += 0.01f;
	}

	if (line2.heightVal == line1.heightVal)
	{
		line2.heightVal += 0.01f;
	}

	float x = (line2.heightVal - line1.heightVal) / (line1.slope - line2.slope);
	if (abs(x) < 0.05f)
		x = 0.0f;
	float y = line1.slope * x + line1.heightVal;
	return ci::Vec2f(x, y);
}

//FIXED : Endのほうが小さいとおわる
bool CrossedLine(LineSegment& hitLine, LineSegment& crossLine)
{
	//ci::app::console() << "X : " << CrossLine(hitLine, crossLine).x << "\n"
	//	<< hitLine.startPos.x << "\n"
	//	<< hitLine.endPos.x << std::endl;

	//ci::app::console() << "Y : " << CrossLine(hitLine, crossLine).y << "\n"
	//	<< hitLine.startPos.y << "\n"
	//	<< hitLine.endPos.y << std::endl;
	if (CrossLine(hitLine, crossLine).x >= hitLine.startPos.x && CrossLine(hitLine, crossLine).x <= hitLine.endPos.x)
	{
		if (CrossLine(hitLine, crossLine).y >= hitLine.startPos.y && CrossLine(hitLine, crossLine).y <= hitLine.endPos.y)
		{
			return true;
		}
	}
	return false;
}

Effect MakeEffect(cinder::Vec2f startPos, cinder::Vec2f endPos)
{
	Effect effect;

	effect.pos = ci::Vec2f((endPos.x - startPos.x) / 2, (endPos.y - startPos.y) / 2) + startPos;

	float sizeX = sqrt((endPos.x - startPos.x) * (endPos.x - startPos.x) + (endPos.y - startPos.y) * (endPos.y - startPos.y));


	effect.size = ci::Vec2f(sizeX, 30);
    effect.angle = atan2f((endPos.x - startPos.x), (endPos.y - startPos.y));


	return effect;

}

Effect MakeAttackEffect(cinder::Vec2f startPos, cinder::Vec2f endPos)
{
	Effect effect;

	effect.pos = ci::Vec2f((endPos.x - startPos.x) / 2, (endPos.y - startPos.y) / 2) + startPos;

	effect.angle = atan2f((endPos.x - startPos.x), (endPos.y - startPos.y));
	float x = (ATTACK_LINE_DEF)* std::sin(effect.angle);
	float y = (ATTACK_LINE_DEF)* std::cos(effect.angle);

	float sizeX = sqrt(x * x + y * y);

	effect.size = ci::Vec2f(sizeX, 30);

	return effect;


}

float CheckDefLineOfCircle(Line line_, cinder::Vec2f pos_, float size_)
{
	//引いたLineの線の値
	float slopeValue = (line_.startPos.y - line_.endPos.y) / (line_.startPos.x - line_.endPos.x);
	float plusNum = line_.startPos.y - slopeValue * line_.startPos.x;

	//引いたLineと直角になる線の値
	float minusSlopeValue = -1 / slopeValue;
	float minusNum = pos_.y - minusSlopeValue * pos_.x;

	//交点の値
	float xValue = (minusNum - plusNum) / (slopeValue - minusSlopeValue);
	float yValue = slopeValue * xValue + plusNum;

	float def = (xValue - pos_.x) * (xValue - pos_.x) + (yValue - pos_.y) * (yValue - pos_.y);
	float judgedValue = sqrt(def) / size_;

	return judgedValue;
}

Line MakeLongLine(Line& line_)
{
	Line line;
	float slope = (line_.startPos.y - line_.endPos.y) / (line_.startPos.x - line_.endPos.x);
	float heightVal = line_.startPos.y - slope * line_.startPos.x;

	if (line_.startPos.x <= line_.endPos.x)
	{
		line.startPos = ci::Vec2f(env.getWindowWidth(), (env.getWindowWidth()) * slope + heightVal);
		line.endPos = ci::Vec2f(-100, -100 * slope + heightVal);
	}

	else
	{
		line.startPos = ci::Vec2f(-100, -100 * slope + heightVal);
		line.endPos = ci::Vec2f(env.getWindowWidth() + 100, (env.getWindowWidth() + 100) * slope + heightVal);
	}

	return line;

}

bool CheckLineDef(Line line_)
{
	float def = (line_.startPos.x - line_.endPos.x) * (line_.startPos.x - line_.endPos.x) + (line_.startPos.y - line_.endPos.y) * (line_.startPos.y - line_.endPos.y);

	return sqrt(def) > DEFAULT_LINE_VALUE;
}

ci::Vec2i ChangeMousePos(ci::Vec2i mouseEventPos_)
{
	ci::Vec2i mousePos = mouseEventPos_;
	mousePos.x -= env.getWindowWidth() / 2;
	mousePos.y -= env.getWindowHeight() / 2;
	mousePos.y *= -1;

	return mousePos;
}

int Wrap(int value, int low, int high)
{
	assert(low < high);
	const int n = (value - low) % (high - low);
	return (n >= 0) ? (n + low) : (n + high);
}

std::string GetFileFullPath(const char* file)
{
	//自動でそのPathがある所まで検索する
	ci::fs::path fullPath = ci::app::getAssetPath(file);
	//fs::path → std::string
	return fullPath.string();
}

void drawTexture(ci::Vec2f pos, ci::Vec2f size, ci::gl::Texture& texture, ci::ColorA color)
{
	ci::gl::pushModelView();
	ci::gl::color(color);
	ci::gl::enableAlphaBlending();
	texture.enableAndBind();
	ci::gl::drawSolidRect(ci::Rectf(pos.x, pos.y, pos.x + size.x, pos.y + size.y));
	texture.disable();
	ci::gl::disableAlphaBlending();
	ci::gl::popModelView();
}