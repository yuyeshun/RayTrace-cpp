#pragma once

#include "Vec3.h"
#include "Ray.h"

inline float ffmin(float a, float b) { return a > b ? b : a; }
inline float ffmax(float a, float b) { return a > b ? a : b; }

class AABB
{
public:
	AABB() {}
	AABB(const Vec3& a, const Vec3& b) { _Min = a; _Max = b; }

	Vec3 Min() const { return _Min; }
	Vec3 Max() const { return _Max; }

	inline bool Hit(const Ray& r, float tmin, float tmax) const;

private:
	Vec3 _Min;
	Vec3 _Max;
};

inline bool AABB::Hit(const Ray& r, float tmin, float tmax) const
{
	for (int a = 0; a < 3; ++a)
	{
		float invD = 1.0f / r.Direction()[a];
		float t0 = (Min()[a] - r.Origin()[a]) * invD;
		float t1 = (Max()[a] - r.Origin()[a]) * invD;
		if (invD < 0)
		{
			std::swap(t0, t1);
		}
		tmin = t0 > tmin ? t0 : tmin;
		tmax = t1 < tmax ? t1 : tmax;
		if (tmax <= tmin)
		{
			return false;
		}
	}
	return true;
}