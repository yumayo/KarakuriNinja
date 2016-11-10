#pragma once

#include "cinder/camera.h"
#include "cinder/cinderMath.h"
#include "Framework.hpp"

class MoveCamera
{
public:
	void Setup()
	{
		_camera = ci::CameraPersp(env.getWindowWidth(), env.getWindowHeight(), 60.0f, 1.0f, 100.0f);
		_nowPos = ci::Vec3f(0, 0, -5.0f);
		_lookPos = ci::Vec3f(0.0f, 0.0f, 0.0f);
		_angle = ci::Vec3f(0, 0, 1);
		_camera.setEyePoint(_nowPos);
		_camera.setCenterOfInterestPoint(_lookPos);
	}

	void ChangeAngle(ci::Vec2f changeValue)
	{
		_angle.x += static_cast<float>(_rotateSpeed * 2.0f * changeValue.x);
		_angle.y += static_cast<float>(_rotateSpeed * 2.0f * changeValue.y);

		_angle.x = std::min(static_cast<float>(M_PI / 2.1f), _angle.x);
		_angle.x = std::max(static_cast<float>(M_PI / -2.1f), _angle.x);
		_angle.y = static_cast<float>(std::fmod(_angle.y, 2 * M_PI));
	}

	void Rotate()
	{
		ci::gl::pushModelView();

		ci::Vec3f r = ci::Vec3f(0, 0, 1);

		_lookPos = ci::Matrix44f::createRotation(ci::Vec3f(_angle.x, _angle.y, 0)) * r;
		_lookPos += _nowPos;

		_camera.setEyePoint(_nowPos);

		_camera.setCenterOfInterestPoint(_lookPos);

		ci::gl::popModelView();
	}


	ci::CameraPersp Get()
	{
		return _camera;
	}
private:

	ci::CameraPersp _camera;

	ci::Vec3f _nowPos;

	ci::Vec3f _lookPos;

	ci::Vec3f _angle;

	double _rotateSpeed = M_PI / 90.0f;


};