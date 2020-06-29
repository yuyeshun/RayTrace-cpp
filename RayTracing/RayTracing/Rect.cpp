#include "Rect.h"
#include "AABB.h"

bool Rect::Hit(const Ray& r, float tmin, float tmax, HitRecord& rec) const
{
	float t, a, b;
	if (type == RectType::XY)
	{
		t = (k - r.Origin().Z()) / r.Direction().Z();		
		a = r.Origin().X() + t * r.Direction().X();
		b = r.Origin().Y() + t * r.Direction().Y();
				
		rec.normal = Vec3(0, 0, 1);		
	}
	else if (type == RectType::XZ)
	{
		t = (k - r.Origin().Y()) / r.Direction().Y();		
		a = r.Origin().X() + t * r.Direction().X();
		b = r.Origin().Z() + t * r.Direction().Z();		
		
		rec.normal = Vec3(0, 1, 0);		
	}
	else if (type == RectType::YZ)
	{
		t = (k - r.Origin().X()) / r.Direction().X();		
		a = r.Origin().Y() + t * r.Direction().Y();
		b = r.Origin().Z() + t * r.Direction().Z();		
		
		rec.normal = Vec3(1, 0, 0);		
	}	

	if (type == RectType::XY || type == RectType::XZ || type == RectType::YZ)
	{
		if (t < tmin || t > tmax)
		{
			return false;
		}
		if (a < a0 || a > a1 || b < b0 || b > b1)
		{
			return false;
		}
		rec.u = (a - a0) / (a1 - a0);
		rec.v = (b - b0) / (b1 - b0);
		rec.t = t;
		rec.pMat = pMat;
		rec.p = r.PointAtParameter(t);
		return true;
	}
    return false;
}

bool Rect::BoundingBox(float t0, float t1, AABB& box) const
{
	switch (type)
	{
	case RectType::XY:
		box = AABB(Vec3(a0, b0, k - 0.0001f), Vec3(a1, b1, k + 0.0001f));
		break;
	case RectType::XZ:
		box = AABB(Vec3(a0, k - 0.0001f, b0), Vec3(a1, k + 0.0001f, b1));
		break;
	case RectType::YZ:
		box = AABB(Vec3(k - 0.0001f, a0, b0), Vec3(k + 0.0001f, a1, b1));
		break;
	default:
		return false;
	}    
    return true;
}
