#pragma once
#include "Ray.h"
#include "AABB.h"
class Material;
class Texture;

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

class FlipNormal : public Hitable
{
public:
	FlipNormal(Hitable* obj) : ptr(obj) {}

	virtual bool Hit(const Ray& r, float tmin, float tmax, HitRecord& rec) const
	{
		if (ptr->Hit(r, tmin, tmax, rec))
		{
			rec.normal = -rec.normal;
			return true;
		}
		else
		{
			return false;
		}
	}

	virtual bool BoundingBox(float t0, float t1, AABB& box) const
	{
		return ptr->BoundingBox(t0, t1, box);
	}

	Hitable* ptr;
};

class Translate : public Hitable
{
public:
	Translate(Hitable* obj, const Vec3& displacement) : ptr(obj), offset(displacement){}

	virtual bool Hit(const Ray& r, float tmin, float tmax, HitRecord& rec) const;
	virtual bool BoundingBox(float t0, float t1, AABB& box) const;

	Hitable* ptr;
	Vec3 offset;
};

class RotateY : public Hitable 
{
public:
	RotateY(Hitable* obj, float angle);

	virtual bool Hit(const Ray& r, float tmin, float tmax, HitRecord& rec) const;
	virtual bool BoundingBox(float t0, float t1, AABB& box) const;

	Hitable* ptr;
	float sinTheta;
	float cosTheta;
	bool hasBox;
	AABB _box;
};

class ConstantMedium : public Hitable
{
public:
	ConstantMedium(Hitable* obj, float d, Texture* a);

	virtual bool Hit(const Ray& r, float tmin, float tmax, HitRecord& rec) const;
	virtual bool BoundingBox(float t0, float t1, AABB& box) const;

	Hitable* ptr;
	float density;
	Material* phaseFunc;
};
