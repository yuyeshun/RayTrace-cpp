#include "HitableList.h"

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