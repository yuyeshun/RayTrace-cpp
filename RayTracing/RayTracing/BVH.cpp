#include "BVH.h"
#include "AABB.h"
#include "Util.h"
#include"HitableList.h"

#include <algorithm>

bool cmpX(const Hitable* a, const Hitable* b)
{
	AABB left, right;
	if (!a->BoundingBox(0, 0, left) || !b->BoundingBox(0, 0, right))
	{
		std::cerr << "no bounding box in BVH_Node construction." << std::endl;
	}
	return (left.Min().X() - right.Min().X() < 0);
}

bool cmpY(const Hitable* a, const Hitable* b)
{
	AABB left, right;
	if (!a->BoundingBox(0, 0, left) || !b->BoundingBox(0, 0, right))
	{
		std::cerr << "no bounding box in BVH_Node construction." << std::endl;
	}
	return (left.Min().Y() - right.Min().Y() < 0);
}

bool cmpZ(const Hitable* a, const Hitable* b)
{
	AABB left, right;
	if (!a->BoundingBox(0, 0, left) || !b->BoundingBox(0, 0, right))
	{
		std::cerr << "no bounding box in BVH_Node construction." << std::endl;
	}
	return (left.Min().Z() - right.Min().Z() < 0);
}

BVH_Node::BVH_Node(std::vector<Hitable*>::iterator start, int n, float time0, float time1)
{
	int axis = int(3 * Random());
	if (axis == 0)
	{
		sort(start, start + n, cmpX);
	}
	else if (axis == 1)
	{
		sort(start, start + n, cmpY);
	}
	else
	{
		sort(start, start + n, cmpZ);
	}

	if (n == 1)
	{
		left = right = *start;
	}
	else if (n == 2)
	{
		left = *start;
		right = *(start + 1);
	}
	else
	{
		left = new BVH_Node(start, n / 2, time0, time1);
		right = new BVH_Node(start + n / 2, n - n / 2, time0, time1);
	}
	AABB leftBox, rightBox;
	if (!left->BoundingBox(time0, time1, leftBox) || !right->BoundingBox(time0, time1, rightBox))
	{
		std::cerr << "no bounding box in BVH_Node construction." << std::endl;
	}
	box = SurroundingBox(leftBox, rightBox);
}

bool BVH_Node::Hit(const Ray& r, float tmin, float tmax, HitRecord& rec) const
{
	if (box.Hit(r, tmin, tmax))
	{
		HitRecord leftRec, rightRec;
		bool hitLeft = left->Hit(r, tmin, tmax, leftRec);
		bool hitRight = right->Hit(r, tmin, tmax, rightRec);
		if (hitLeft && hitRight)
		{
			rec = (leftRec.t < rightRec.t) ? leftRec : rightRec;			
			return true;
		}
		else if (hitLeft)
		{
			rec = leftRec;
			return true;
		}
		else if (hitRight)
		{
			rec = rightRec;
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}	
}

bool BVH_Node::BoundingBox(float t0, float t1, AABB& box) const
{
	box = this->box;
	return true;
}
