#include"SpecialShuriken.h"

using namespace ci;

SpecialShuriken::SpecialShuriken()
{
}

SpecialShuriken::SpecialShuriken(cinder::Vec2f pos, cinder::Vec2f size, cinder::gl::Texture * tex)
{
	pos_ = pos;
	size_ = size;
	tex_= tex;
	rotate = 0.0f;
}

void SpecialShuriken::draw(cinder::Vec2f trancepos)
{
	if (pos_.x >-800 && pos_.x<(env.getWindowWidth() + 800)) {
		gl::pushModelView();
		gl::translate(pos_ + trancepos);
		gl::rotate(rotate);
		tex_->enableAndBind();
		gl::drawSolidRect(Rectf(-size_ / 2, size_ / 2));
		tex_->disable();
		gl::popModelView();
	}

}

void SpecialShuriken::update(cinder::Vec2f speed)
{
	pos_ += speed;
	rotate += 12;
}
