#pragma once

#include "cinder/Vector.h"
#include "Framework.hpp"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"

const int START_ATTACK_TIME = 3;
const int END_ATTACK_TIME = 10;
const int DEFAULT_LINE_VALUE = 100;
const int ATTACK_LINE_DEF = 2000;

struct Line
{
	cinder::Vec2f startPos;
	cinder::Vec2f endPos;
};

struct LineSegment
{
	//傾き
	float slope;
	//高さ (  y= ax + b のx = 0のときの値)
	float heightVal;

	cinder::Vec2f startPos;
	cinder::Vec2f endPos;
};

struct Effect
{
	cinder::Vec2f pos;
	cinder::Vec2f size;
	float angle;
};

cinder::Vec2i ChangeMousePos(ci::Vec2i mouseEventPos_);

LineSegment MakeLine(Line& line_);

LineSegment MakeLine(cinder::Vec2f& startPos_, cinder::Vec2f& endPos_);

ci::Vec2f CrossLine(LineSegment& line1, LineSegment& line2);

bool CrossedLine(LineSegment& hitLine, LineSegment& crossLine);

Effect MakeEffect(cinder::Vec2f startPos, cinder::Vec2f endPos);

Effect MakeAttackEffect(cinder::Vec2f startPos, cinder::Vec2f endPos);


float CheckDefLineOfCircle(Line line_, cinder::Vec2f pos_, float size_);

enum CommandType
{
	NONE,
	ATTACK,
	GUARD,
	SKILL_SELECT
};

//! @brief LineとEnemyの当たり判定をしています
//! @note 
//!     　線と円の当たり判定です
//!       true-> hit false->non Hit

float CheckDefLineOfCircle(Line line_, cinder::Vec2f pos_, float size_);

//形成したLineをさらに長くします
Line MakeLongLine(Line& line_);

//形成したラインの絶対距離を測ります
bool CheckLineDef(Line line_);

//値の最大値と最小値を超えないようにします
template <typename T>
static T Clamp(T value, T low, T high)
{
	assert(low <= high);
	return std::min(std::max(value, low), high);
}

//ラップアラウンド関数
int Wrap(int value, int low, int high);

//指定したAsstes内部のファイルまでのPathをわたします
std::string GetFileFullPath(const char* file);

//Texture描画
void drawTexture(ci::Vec2f pos, ci::Vec2f size, ci::gl::Texture& texture, ci::ColorA color);