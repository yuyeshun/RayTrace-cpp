#include "Sphere.h"
#include "AABB.h"
#include "Util.h"

void GetSphereUV(const Vec3& p, float& u, float& v)
{
	float phi = atan2(p.Z(), p.X());
	float theta = asin(p.Y());
	u = (float)(1.0f - (phi + PI) / (2.0f * PI));
	v = (float)((theta + PI / 2.0f) / PI);
}

bool Sphere::Hit(const Ray &r, float tmin, float tmax, HitRecord &rec) const {
	Vec3 oc = r.Origin() - center;
	float a = Dot(r.Direction(), r.Direction());
	float b = Dot(oc, r.Direction());
	float c = Dot(oc, oc) - radius * radius;
	float discriminant = b * b - a * c;

	if (discriminant > 0) {
		float temp = (-b - sqrt(b * b - a * c)) / a;
		if (temp < tmax && temp > tmin) {
			rec.t = temp;
			rec.p = r.PointAtParameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.pMat = pMat;
			GetSphereUV((rec.p - center) / radius, rec.u, rec.v);
			return true;
		}
		temp = (-b + sqrt(b * b - a * c)) / a;
		if (temp < tmax && temp > tmin) {
			rec.t = temp;
			rec.p = r.PointAtParameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.pMat = pMat;
			GetSphereUV((rec.p - center) / radius, rec.u, rec.v);
			return true;
		}
	}
	return false;
}

bool Sphere::BoundingBox(float t0, float t1, AABB& box) const
{
	box = AABB(center - Vec3(radius, radius, radius), center + Vec3(radius, radius, radius));
	return true;
}

bool MovingSphere::Hit(const Ray& r, float tmin, float tmax, HitRecord& rec) const
{
	Vec3 oc = r.Origin() - center(r.Time());
	float a = Dot(r.Direction(), r.Direction());
	float b = Dot(oc, r.Direction());
	float c = Dot(oc, oc) - radius * radius;
	float discriminant = b * b - a * c;

	if (discriminant > 0) {
		float temp = (-b - sqrt(b * b - a * c)) / a;
		if (temp < tmax && temp > tmin) {
			rec.t = temp;
			rec.p = r.PointAtParameter(rec.t);
			rec.normal = (rec.p - center(r.Time())) / radius;
			rec.pMat = pMat;
			return true;
		}
		temp = (-b + sqrt(b * b - a * c)) / a;
		if (temp < tmax && temp > tmin) {
			rec.t = temp;
			rec.p = r.PointAtParameter(rec.t);
			rec.normal = (rec.p - center(r.Time())) / radius;
			rec.pMat = pMat;
			return true;
		}
	}
	return false;
}

bool MovingSphere::BoundingBox(float t0, float t1, AABB& box) const
{
	auto box0 = AABB(center(t0) - Vec3(radius, radius, radius), center(t0) + Vec3(radius, radius, radius));
	auto box1 = AABB(center(t1) - Vec3(radius, radius, radius), center(t1) + Vec3(radius, radius, radius));
	box = SurroundingBox(box0, box1);
	return true;
}

Vec3 MovingSphere::center(float time) const
{
	return center0 + ((time - time0) / (time1 - time)) * (center1 - center0);
}
