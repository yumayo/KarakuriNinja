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
	//�X��
	float slope;
	//���� (  y= ax + b ��x = 0�̂Ƃ��̒l)
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

//! @brief Line��Enemy�̓����蔻������Ă��܂�
//! @note 
//!     �@���Ɖ~�̓����蔻��ł�
//!       true-> hit false->non Hit

float CheckDefLineOfCircle(Line line_, cinder::Vec2f pos_, float size_);

//�`������Line������ɒ������܂�
Line MakeLongLine(Line& line_);

//�`���������C���̐�΋����𑪂�܂�
bool CheckLineDef(Line line_);

//�l�̍ő�l�ƍŏ��l�𒴂��Ȃ��悤�ɂ��܂�
template <typename T>
static T Clamp(T value, T low, T high)
{
	assert(low <= high);
	return std::min(std::max(value, low), high);
}

//���b�v�A���E���h�֐�
int Wrap(int value, int low, int high);

//�w�肵��Asstes�����̃t�@�C���܂ł�Path���킽���܂�
std::string GetFileFullPath(const char* file);

//Texture�`��
void drawTexture(ci::Vec2f pos, ci::Vec2f size, ci::gl::Texture& texture, ci::ColorA color);