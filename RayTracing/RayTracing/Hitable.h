#pragma once
#include "Ray.h"

class Material;

struct HitRecord {
	float t;
	Vec3 p;
	Vec3 normal;
	Material *pMat;
};

class Hitable {
public:
	virtual bool Hit(const Ray &r, float tmin, float tmax, HitRecord &rec) const = 0;
};