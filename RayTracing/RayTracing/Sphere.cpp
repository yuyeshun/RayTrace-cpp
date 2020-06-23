#include "Sphere.h"

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
			return true;
		}
		temp = (-b + sqrt(b * b - a * c)) / a;
		if (temp < tmax && temp > tmin) {
			rec.t = temp;
			rec.p = r.PointAtParameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.pMat = pMat;
			return true;
		}
	}
	return false;
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

Vec3 MovingSphere::center(float time) const
{
	return center0 + ((time - time0) / (time1 - time)) * (center1 - center0);
}
