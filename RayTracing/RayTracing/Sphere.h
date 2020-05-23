#pragma once
#include "Hitable.h"

class Material;

class Sphere : public Hitable {
public:
	Sphere() {}
	Sphere(Vec3 center_, float radius_, Material* mat) : center(center_), radius(radius_), pMat(mat) {}
	
	virtual bool Hit(const Ray &r, float tmin, float tmax, HitRecord &rec) const;

	Vec3 center;
	float radius;
	Material *pMat;
};