#pragma once

#include "Ray.h"
#include "Util.h"

class Camera {
public:
	Camera(Vec3 lookFrom, Vec3 lookAt, Vec3 vUp) {
		SetFov(lookFrom, lookAt, vUp, 90, 2, 0, (lookFrom - lookAt).Length());
	}

	Camera(Vec3 lookFrom, Vec3 lookAt, Vec3 vUp, float vfov, float aspect, float aperture, float focusDist) {
		SetFov(lookFrom, lookAt, vUp, vfov, aspect, aperture, focusDist);
	}

	Camera(Vec3 lookFrom, Vec3 lookAt, Vec3 vUp, float vfov, float aspect, float aperture, float focusDist, float t0, float t1) {
		time0 = t0;
		time1 = t1;
		SetFov(lookFrom, lookAt, vUp, vfov, aspect, aperture, focusDist);
	}

public:

	void SetFov(Vec3 lookFrom, Vec3 lookAt, Vec3 vUp, float vfov, float aspect, float aperture, float focusDist) {
		lensRadius = aperture / 2;
		float theta = (float)(vfov * PI / 180.0f);
		float halfHeight = tan(theta / 2);
		float halfWidth = aspect * halfHeight;
		Origin = lookFrom;
		w = UnitVector(lookFrom - lookAt);
		u = UnitVector(Cross(vUp, w));
		v = Cross(w, u);
		LowerLeftCorner = Origin - halfWidth * focusDist * u - halfHeight * focusDist * v - focusDist * w;
		Horizontal = 2 * halfWidth * focusDist * u;
		Vertical = 2 * halfHeight * focusDist * v;
	}

	Ray GetRay(float s, float t) { 
		Vec3 rd = lensRadius * RandomInUintDisk();
		Vec3 offset = u * rd.X() + v * rd.Y();
		float time = time0 + Random() * (time1 - time0);
		return Ray(Origin + offset, LowerLeftCorner + s * Horizontal + t * Vertical - Origin - offset, time); 
	}

private:

	Vec3 Origin;
	Vec3 LowerLeftCorner;
	Vec3 Horizontal;
	Vec3 Vertical;
	Vec3 u, v, w;
	float lensRadius;
	float time0 = 0, time1 = 0;
};