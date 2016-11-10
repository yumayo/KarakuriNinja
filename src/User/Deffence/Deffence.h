#pragma once
#include "cinder/Vector.h"
#include "Framework.hpp"
#include "../Utilitys/Nomoto.h"
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
const int GUARD_LINE_DEF = 500;

class Deffence
{
public:
	Deffence() : isActive(false)
	{
		tex = loadImage(cinder::app::loadAsset("Textures/gurd.png"));
	}

	void Update(ci::Vec2f rightPos,ci::Vec2f leftPos);

	void MakeGuardLine()
	{
		ci::Vec2f centerPoint = ci::Vec2f((guardLine.endPos.x - guardLine.startPos.x) / 2, (guardLine.startPos.y - guardLine.endPos.y) / 2);
		float angle = atan2f((guardLine.startPos.x - guardLine.endPos.x), (guardLine.startPos.y - guardLine.endPos.y));
		float x = (GUARD_LINE_DEF) * std::sin(angle);
		float y = (GUARD_LINE_DEF) * std::cos(angle);
		guardLine.startPos = effect.pos +  ci::Vec2f(x,y);
		guardLine.endPos = effect.pos - ci::Vec2f(x, y);

		guardLineSegment = MakeLine(guardLine);
	}

	void Draw();

	Line GuardLine() 
	{
		return guardLine;
	}

	bool IsActive() { return isActive; }

	void ShiftIsActive() { isActive = !isActive; }

private:

	bool isActive;
	Line guardLine;
	LineSegment guardLineSegment;
	Effect effect;
	cinder::gl::Texture tex;
};