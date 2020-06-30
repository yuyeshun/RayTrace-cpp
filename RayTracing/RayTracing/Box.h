#pragma once

#include "Hitable.h"
#include "Rect.h"
#include "HitableList.h"

class Box : public Hitable
{
public:
	Box(){}
	Box(const Vec3& p0, const Vec3& p1, Material* mat);

	virtual bool Hit(const Ray& r, float tmin, float tmax, HitRecord& rec) const;
	virtual bool BoundingBox(float t0, float t1, AABB& box) const;

	Vec3 pMin, pMax;
	HitableList* assemblyBox;
};