#pragma once

#include "Util.h"
#include "Hitable.h"

class Material;

class Rect : public Hitable
{
public:

	Rect() {}
	Rect(float a0, float a1, float b0, float b1, float k, Material* mat, RectType rectType) 
	: a0(a0), a1(a1), b0(b0), b1(b1), k(k), pMat(mat), type(rectType) {}

	virtual bool Hit(const Ray& r, float tmin, float tmax, HitRecord& rec) const;
	virtual bool BoundingBox(float t0, float t1, AABB& box) const;

	Material* pMat;
	float a0, a1, b0, b1, k;
	RectType type;
};