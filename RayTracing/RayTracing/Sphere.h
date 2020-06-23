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

class MovingSphere : public Hitable {

public:
	MovingSphere() {}

	MovingSphere(Vec3 cen0, Vec3 cen1, float t0, float t1, float r, Material *mat) 
	: center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), pMat(mat) {}

	virtual bool Hit(const Ray& r, float tmin, float tmax, HitRecord& rec) const;

	Vec3 center(float time)const;

	Vec3 center0, center1;
	float time0, time1;
	float radius;
	Material* pMat;
};