#pragma once

#include "Hitable.h"
#include "Util.h"
#include "Texture.h"

class Material {
public:
	virtual bool Scatter(const Ray &InRay, const HitRecord &rec, Vec3 &attenuation, Ray &scattered) const = 0;
};

class Lambertian : public Material {
public:
	Lambertian(Texture* t) : albedo(t) {}

	virtual bool Scatter(const Ray &InRay, const HitRecord &rec, Vec3 &attenuation, Ray &scattered) const {
		Vec3 target = rec.p + rec.normal + RandomInUnitSphere();
		scattered = Ray(rec.p, target - rec.p, InRay.Time());
		attenuation = albedo->Value(0, 0, rec.p);
		return true;
	}

	Texture* albedo;
};

class Metal : public Material {
public:
	Metal(const Vec3 &a, float f = 0) : albedo(a) { if (f < 1) fuzz = f; else fuzz = 1; }

	virtual bool Scatter(const Ray &InRay, const HitRecord &rec, Vec3 &attenuation, Ray &scattered) const {
		Vec3 reflected = Reflect(UnitVector(InRay.Direction()), rec.normal);
		scattered = Ray(rec.p, reflected + fuzz * RandomInUnitSphere());
		attenuation = albedo;
		return (Dot(scattered.Direction(), rec.normal) > 0);
	}

	Vec3 albedo;
	float fuzz;
};


class Dielectric : public Material {

public:
	Dielectric(float ri) : RefIdx(ri) {}

	virtual bool Scatter(const Ray &InRay, const HitRecord &rec, Vec3 &attenuation, Ray &scattered) const {
		Vec3 outwardNormal;
		Vec3 reflected = Reflect(InRay.Direction(), rec.normal);
		float NiOverNt;
		attenuation = Vec3(1.0f, 1.0f, 1.0f);
		Vec3 refracted;
		float reflectProb;
		float cosine;
		if (Dot(InRay.Direction(), rec.normal) > 0) {
			outwardNormal = -rec.normal;
			NiOverNt = RefIdx;
			cosine = RefIdx * Dot(InRay.Direction(), rec.normal) / InRay.Direction().Length();
		}
		else {
			outwardNormal = rec.normal;
			NiOverNt = 1.0f / RefIdx;
			cosine = -Dot(InRay.Direction(), rec.normal) / InRay.Direction().Length();
		}

		if (Refract(InRay.Direction(), outwardNormal, NiOverNt, refracted)) {
			reflectProb = Schlick(cosine, RefIdx);
		}
		else {			
			reflectProb = 1.0f;
		}

		if (Random(0, 1) < reflectProb) {
			scattered = Ray(rec.p, reflected);
		}
		else {
			scattered = Ray(rec.p, refracted);
		}
		return true;
	}

	float RefIdx;
};