#include "HitableList.h"
#include "Util.h"
#include "AABB.h"

bool HitableList::Hit(const Ray &r, float tmin, float tmax, HitRecord &rec) const {
	HitRecord tempRec;
	bool HitAnything = false;
	double closestSoFar = tmax;
	int listSize = (int)list.size();
	for (int i = 0; i < listSize; ++i) {
		if (list[i]->Hit(r, tmin, float(closestSoFar), tempRec)) {
			HitAnything = true;
			closestSoFar = tempRec.t;
			rec = tempRec;
		}
	}
	return HitAnything;
}

bool HitableList::BoundingBox(float t0, float t1, AABB& box) const
{
	if (list.size() < 1)
	{
		return false;
	}
	AABB tempBox;
	bool first_true = list.front()->BoundingBox(t0, t1, tempBox);
	if (!first_true)
	{
		return false;
	}
	else
	{
		box = tempBox;
	}
	for (int i = 1; i < list.size(); ++i)
	{
		if (list[i]->BoundingBox(t0, t1, tempBox))
		{
			box = SurroundingBox(box, tempBox);
		}
		else
		{
			return false;
		}
	}
	return true;
}
