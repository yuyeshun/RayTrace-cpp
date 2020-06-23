#pragma once
#include "Vec3.h"

class Ray {

public:
	Ray() {}
	Ray(const Vec3& a, const Vec3& b, float time = 0.0f) { A = a; B = b; _Time = time; }
	Vec3 Origin() const { return A; }
	Vec3 Direction() const { return B; }
	float Time() const { return _Time; }
	Vec3 PointAtParameter(float t) const { return A + t * B; }

	Vec3 A;
	Vec3 B;
	float _Time;
};