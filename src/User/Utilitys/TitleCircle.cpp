#include"TitleCircle.h"

using namespace ci;

TitleCircle::TitleCircle()
{

}

TitleCircle::TitleCircle(cinder::Vec2f pos, cinder::Vec2f size, cinder::gl::Texture * tex)
{
	pos_ = pos;
	size_ = size;
	tex_ = tex;
	rotate = 0.0f;
}

void TitleCircle::draw(float posx)
{
	if (pos_.y >-400 && pos_.y<(env.getWindowHeight() + 400)) {
		gl::pushModelView();
		gl::translate(Vec2f(pos_.x + posx, pos_.y));
		gl::rotate(rotate);
		tex_->enableAndBind();
		gl::drawSolidRect(Rectf(-size_ / 2, size_ / 2));
		tex_->disable();
		gl::popModelView();
	}

}

void TitleCircle::update(float y_speed)
{
	pos_.y += y_speed;
	rotate += 12;
}

