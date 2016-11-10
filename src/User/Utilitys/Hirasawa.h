#pragma once
#include<iostream>
#define M_PI 3.14159265359
static float EasingLinear(float t, float b, float e) {
	return (e - b) * t + b;
}


static float EasingBackIn(float t, float b, float e) {
	float s = 1.70158f;
	return (e - b) * t * t * ((s + 1) * t - s) + b;
}

static float EasingBackOut(float t, float b, float e) {
	float s = 1.70158f;
	t -= 1.0f;
	return (e - b) * (t * t * ((s + 1) * t + s) + 1) + b;
}

static float EasingBackInOut(float t, float b, float e) {
	float s = 1.70158f * 1.525f;
	if ((t /= 0.5f) < 1.0f) return (e - b) / 2.0f * (t * t * ((s + 1.0f) * t - s)) + b;
	t -= 2;
	return (e - b) / 2.f * (t * t * ((s + 1.f) * t + s) + 2.f) + b;
}


static float EasingBounceOut(float t, float b, float e) {
	if (t < (1.f / 2.75f)) {
		return (e - b) * (7.5625 * t * t) + b;
	}
	else if (t < (2.f / 2.75f)) {
		t -= (1.5f / 2.75f);
		return (e - b) * (7.5625 * t * t + 0.75) + b;
	}
	else if (t < (2.5f / 2.75f)) {
		t -= (2.25f / 2.75f);
		return (e - b) * (7.5625 * t * t + 0.9375) + b;
	}
	else {
		t -= (2.625f / 2.75f);
		return (e - b) * (7.5625f * t * t + 0.984375f) + b;
	}
}

static float EasingBounceIn(float t, float b, float e) {
	return (e - b) - EasingBounceOut(1.0f - t, 0.f, e - b) + b;
}

static float EasingBounceInOut(float t, float b, float e) {
	if (t < 0.5f) return EasingBounceIn(t * 2.f, 0, e - b) * 0.5f + b;
	else         return EasingBounceOut(t * 2.f - 1.0f, 0, e - b) * 0.5f + (e - b) * 0.5f + b;
}


static float EasingCircIn(float t, float b, float e) {
	return -(e - b) * (std::sqrt(1.f - t * t) - 1.f) + b;
}

static float EasingCircOut(float t, float b, float e) {
	t -= 1.0f;
	return (e - b) * std::sqrt(1.f - t * t) + b;
}

static float EasingCircInOut(float t, float b, float e) {
	if ((t /= 0.5f) < 1.f) return -(e - b) / 2.f  * (std::sqrt(1.f - t * t) - 1.f) + b;
	t -= 2.f;
	return (e - b) / 2.f  * (std::sqrt(1.f - t * t) + 1.f) + b;
}


static float EasingCubicIn(float t, float b, float e) {
	return (e - b) * t * t * t + b;
}

static float EasingCubicOut(float t, float b, float e) {
	t -= 1.0f;
	return (e - b) * (t * t * t + 1.f) + b;
}

static float EasingCubicInOut(float t, float b, float e) {
	if ((t /= 0.5f) < 1.f) return (e - b) / 2.f  * t * t * t + b;
	t -= 2.f;
	return (e - b) / 2.f  * (t * t * t + 2.f) + b;
}


static float EasingElasticIn(float t, float b, float e) {
	if (t == 0.f) return b;
	if (t == 1.f) return e;

	float p = 0.3f;
	float a = e - b;
	float s = p / 4.f;
	t -= 1.f;
	return -(a * std::pow(2.f, 10.f  * t) * std::sin((t - s) * (2.f  * M_PI) / p)) + b;
}

static float EasingElasticOut(float t, float b, float e) {
	if (t == 0) return b;
	if (t == 1) return e;

	float p = 0.3f;
	float a = e - b;
	float s = p / 4.f;
	return (a * std::pow(2.f, -10.f  * t) * std::sin((t - s) * (2.f  * M_PI) / p) + a + b);
}

static float EasingElasticInOut(float t, float b, float e) {
	if (t == 0) return b;
	if ((t /= 0.5f) == 2) return e;

	float p = 0.3f * 1.5f;
	float a = e - b;
	float s = p / 4.f;
	if (t < 1) {
		t -= 1;
		return -0.5f * (a * std::pow(2.f, 10.f  * t) * std::sin((t - s) * (2.f  * M_PI) / p)) + b;
	}
	t -= 1.f;
	return a * std::pow(2.f, -10.f  * t) * std::sin((t - s) * (2.f  * M_PI) / p) * 0.5f + a + b;
}


static float EasingExpoIn(float t, float b, float e) {
	return (t == 0) ? b : (e - b) * std::pow(2.f, 10.f  * (t - 1)) + b;
}

static float EasingExpoOut(float t, float b, float e) {
	return (t == 1) ? e : (e - b) * (-std::pow(2.f, -10.f  * t) + 1) + b;
}

static float EasingExpoInOut(float t, float b, float e) {
	if (t == 0) return b;
	if (t == 1) return e;
	if ((t /= 0.5f) < 1) return (e - b) / 2.f * std::pow(2.f, 10.f * (t - 1)) + b;
	return (e - b) / 2.f * (-std::pow(2.f, -10.f * --t) + 2.f) + b;
}


static float EasingQuadIn(float t, float b, float e) {
	return (e - b) * t * t + b;
}

static float EasingQuadOut(float t, float b, float e) {
	return -(e - b) * t * (t - 2.f) + b;
}

static float EasingQuadInOut(float t, float b, float e) {
	if ((t /= 0.5f) < 1) return (e - b) / 2.f * t * t + b;
	--t;
	return -(e - b) / 2.f * (t * (t - 2.f) - 1.f) + b;
}


static float EasingQuartIn(float t, float b, float e) {
	return (e - b) * t * t * t * t + b;
}

static float EasingQuartOut(float t, float b, float e) {
	t -= 1.0f;
	return -(e - b) * (t * t * t * t - 1.f) + b;
}

static float EasingQuartInOut(float t, float b, float e) {
	if ((t /= 0.5f) < 1.f) return (e - b) / 2.f * t * t * t * t + b;
	t -= 2;
	return -(e - b) / 2.f * (t * t * t * t - 2.f) + b;
}


static float EasingQuintIn(float t, float b, float e) {
	return (e - b) * t * t * t * t * t + b;
}

static float EasingQuintOut(float t, float b, float e) {
	t -= 1.0f;
	return (e - b) * (t * t * t * t * t + 1.f) + b;
}

static float EasingQuintInOut(float t, float b, float e) {
	if ((t /= 0.5f) < 1.f) return (e - b) / 2.f * t * t * t * t * t + b;
	t -= 2.f;
	return (e - b) / 2.f * (t * t * t * t * t + 2.f) + b;
}


static float EasingSineIn(float t, float b, float e) {
	return -(e - b) * std::cos(t * (M_PI / 2.f)) + (e - b) + b;
}

static float EasingSineOut(float t, float b, float e) {
	return (e - b) * std::sin(t * (M_PI / 2.f)) + b;
}

static float EasingSineInOut(float t, float b, float e) {
	return -(e - b) / 2.f * (std::cos(M_PI * t) - 1.f) + b;
}