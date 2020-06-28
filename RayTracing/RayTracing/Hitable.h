#pragma once
#include "Ray.h"

class Material;
class AABB;

struct HitRecord {
	float t;
	Vec3 p;
	Vec3 normal;
	Material *pMat;

	float u;
	float v;
};

class Hitable {
public:
	virtual bool Hit(const Ray &r, float tmin, float tmax, HitRecord &rec) const = 0;
	virtual bool BoundingBox(float t0, float t1, AABB& box) const = 0;
};