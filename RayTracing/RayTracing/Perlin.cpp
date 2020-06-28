#include "Perlin.h"

float Perlin::Noise(const Vec3& p) const
{
	int i = (int)floor(p.X());
	int j = (int)floor(p.Y());
	int k = (int)floor(p.Z());
	float u = p.X() - i;
	float v = p.Y() - j;
	float w = p.Z() - k;	
	Vec3 c[2][2][2];
	for (int di = 0; di < 2; ++di)
	{
		for (int dj = 0; dj < 2; ++dj)
		{
			for (int dk = 0; dk < 2; ++dk)
			{
				c[di][dj][dk] = ranVec[perm_x[(i + di) & 255] ^ perm_y[(j + dj) & 255] ^ perm_z[(k + dk) & 255]];
			}
		}
	}

	return PerlinInterp(c, u, v, w);
}

float Perlin::Turb(const Vec3& p, int depth) const
{
	float accum = 0.0f;
	Vec3 Temp = p;
	float weight = 1.0f;
	for (int i = 0; i < depth; ++i)
	{
		accum += weight * Noise(Temp);
		weight *= 0.5f;
		Temp *= 2;
	}
	return fabs(accum);
}

static Vec3* perlin_generate()
{
	Vec3* p = new Vec3[256];
	for (int i = 0; i < 256; ++i)
	{
		p[i] = UnitVector(Vec3(Random() * 2 - 1, Random() * 2 - 1, Random() * 2 - 1));
	}

	return p;
}

void permute(int* p, int n)
{
	for (int i = n - 1; i > 0; --i)
	{
		int target = int(Random() * (i + 1));
		int temp = p[i];
		p[i] = p[target];
		p[target] = temp;
	}
}

static int* perlin_generate_perm()
{
	int* p = new int[256];
	for (int i = 0; i < 256; ++i)
	{
		p[i] = i;
	}
	permute(p, 256);
	return p;
}

Vec3* Perlin::ranVec = perlin_generate();
int* Perlin::perm_x = perlin_generate_perm();
int* Perlin::perm_y = perlin_generate_perm();
int* Perlin::perm_z = perlin_generate_perm();