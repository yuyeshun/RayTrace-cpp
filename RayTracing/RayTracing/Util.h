#pragma once
#include <stdlib.h>
#include <time.h>
#include "Vec3.h"

const double PI = 3.14159265358979323846;

enum class ImgType
{
	PPM,
	BMP
};

static float Random(float begin = 0, float end = 1) {
	return begin + (end - begin) * (float)(rand() / (RAND_MAX + 1.0));
}

static Vec3 RandomInUnitSphere() {
	Vec3 p;
	do
	{
		p = 2.0f * Vec3(Random(), Random(), Random()) - Vec3(1, 1, 1);
	} while (p.SquaredLength() >= 1.0f);
	return p;
}

static Vec3 RandomInUintDisk() {
	Vec3 p;
	do {
		p = 2.0f * Vec3(Random(), Random(), 0) - Vec3(1, 1, 0);
	} while (Dot(p, p) >= 1.0f);

	return p;
}

static Vec3 Reflect(const Vec3 v, const Vec3 &n) {
	return v - 2 * Dot(v, n) * n;
}

static bool Refract(const Vec3 &v, const Vec3 &n, float NiOverNt, Vec3 &refracted) {
	Vec3 uv = UnitVector(v);
	float dt = Dot(uv, n);
	float discriminant = 1.0f - NiOverNt * NiOverNt * (1 - dt * dt);
	if (discriminant > 0) {
		refracted = NiOverNt * (uv - n * dt) - n * sqrt(discriminant);
		return true;
	}
	else {
		return false;
	}
}

static float Schlick(float cosine, float refIdx) {
	float r0 = (1 - refIdx) / (1 + refIdx);
	r0 = r0 * r0;
	return r0 + (1 - r0) * pow((1 - cosine), 5);
}

struct Pixel {
public:
	Pixel() : R(0), G(0), B(0) {}
	Pixel(int r_, int g_, int b_) : R(r_), G(g_), B(b_) {}

	int R;
	int G;
	int B;
};