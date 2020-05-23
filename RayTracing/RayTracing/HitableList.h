#pragma once

#include "Hitable.h"
#include <vector>

class HitableList : public Hitable {
public:
	HitableList() {}

	virtual bool Hit(const Ray &r, float tmin, float tmax, HitRecord &rec) const;

	void AddObject(Hitable* obj) {
		list.push_back(obj);
	}

	vector<Hitable *> list;
};
