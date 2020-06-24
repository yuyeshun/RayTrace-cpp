#pragma once
#include "Hitable.h"
#include "AABB.h"
#include <vector>

class HitableList;

class BVH_Node : public Hitable
{
public:
	BVH_Node() {}
	BVH_Node(vector<Hitable *>::iterator start, int n, float time0, float time1);

	virtual bool Hit(const Ray& r, float tmin, float tmax, HitRecord& rec) const;
	virtual bool BoundingBox(float t0, float t1, AABB& box) const;

	Hitable* left;
	Hitable* right;
	AABB box;
};