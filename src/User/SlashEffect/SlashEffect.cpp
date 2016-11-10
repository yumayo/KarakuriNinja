#include"SlashEffect.h" 

void Slash::Update() {
	++activeTime;
	if (activeTime == maxactiveTime)
		isActive = false;

}
void Slash::Draw() {
	ci::gl::pushModelView();
	ci::gl::translate(pos.x, pos.y);
	ci::gl::rotate(-angle * (180.0f / M_PI) + 90);
	if (isActive) {
		ci::gl::drawSolidRect(ci::Rectf(ci::Vec2f(-size.x / 2, -size.y / 2), ci::Vec2f(size.x / 2, size.y / 2))); 
	}
	ci::gl::rotate(angle * (180.0f / M_PI) + 90);
	ci::gl::popModelView();

	ci::gl::pushModelView();
	ci::gl::translate(pos.x, pos.y);
	if (isActive) {
		float slashspeed = 4.0f;
		int cutsize = int((std::min(float(slashspeed*activeTime) / float(maxactiveTime), 1.f)) * 1024);
		textureDraw(tex, cinder::Vec2f(0, 0), cinder::Area(0, 0, cutsize, 128), 1);
	}
	ci::gl::rotate(angle * (180.0f / M_PI) + 90);
	ci::gl::popModelView();


}

void Slash::textureDraw(cinder::gl::Texture const & texture, cinder::Vec2f position)
{
	/*cinder::gl::pushModelView();
	cinder::gl::translate(-texture.getSize() / 2.0F + position);
	cinder::gl::draw(texture);
	texture.unbind();
	cinder::gl::popModelView();*/
}


void Slash::textureDraw(cinder::gl::Texture const & texture, cinder::Vec2f position, cinder::Area area, float size)
{
	cinder::gl::pushModelView();
	ci::gl::rotate(-angle * (180.0f / M_PI) + 90);
	ci::gl::scale(ci::Vec2f(2.5f,0.7f));
	ci::gl::translate( -texture.getSize().x / 2, -texture.getSize().y / 2);
	float alfa = 1.f;
	float deletetime = (maxactiveTime / 1.5);
	if (activeTime > deletetime) {
		delete_t += 1.0f / float((maxactiveTime - deletetime));
		alfa = EasingLinear(delete_t, 1.f, 0.f);

	}

	cinder::gl::color(cinder::ColorA(1, 1, 1, alfa));
	cinder::gl::draw(texture,
		cinder::Area(area.x1, area.y1, area.x1 + area.x2 * size, area.y1 + area.y2),
		cinder::Rectf(area.x1, area.y1, area.x1 + area.x2 * size, area.y1 + area.y2));
	cinder::gl::color(cinder::ColorA(1, 1, 1, 1));
	texture.unbind();
	cinder::gl::popModelView();
}
