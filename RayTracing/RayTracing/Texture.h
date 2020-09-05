#pragma once

#include "Vec3.h"
#include "Perlin.h"

class Texture {
public:
	virtual Vec3 Value(float u, float v, const Vec3& p) const = 0;
};

class ConstantTexture : public Texture
{
public:
	ConstantTexture() {}
	ConstantTexture(Vec3 c) : color(c) {}

	virtual Vec3 Value(float u, float v, const Vec3& p) const
	{
		return color;
	}

	Vec3 color;
};

class CheckerTexture : public Texture
{
public:
	CheckerTexture() {}
	CheckerTexture(Texture* t0, Texture* t1) : odd(t0), even(t1){}

	virtual Vec3 Value(float u, float v, const Vec3& p) const
	{
		float sines = sin(1000 * p.X()) * sin(1000 * p.Y()) * sin(1000 * p.Z());

		if (sines < 0)
		{
			return odd->Value(u, v, p);
		}
		else
		{
			return even->Value(u, v, p);
		}		
	}

	Texture* odd;
	Texture* even;
};

class NoiseTexture : public Texture
{
public:
	NoiseTexture(){}
	NoiseTexture(float sc) : scale(sc) {}
	virtual Vec3 Value(float u, float v, const Vec3& p) const
	{
		return Vec3(1,1,1) * 0.5f * (1 + sin(scale * p.Z() + 10 * noise.Turb(p)));
	}

	Perlin noise;
	float scale;
};

class ImageTexture : public Texture
{
public:
	ImageTexture() {}
	ImageTexture(unsigned char* pixel, int A, int B) : data(pixel), nx(A), ny(B) {}

	virtual Vec3 Value(float u, float v, const Vec3& p) const;

	unsigned char* data;
	int nx, ny;
};
