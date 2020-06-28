#pragma once

#include "Vec3.h"
#include "Util.h"

class Perlin {
public:

	float Noise(const Vec3& p) const;

	float Turb(const Vec3& p, int depth =  7) const ;

	static Vec3* ranVec;
	static int* perm_x;
	static int* perm_y;
	static int* perm_z;
};

inline float PerlinInterp(Vec3 c[2][2][2], float u, float v, float w)
{
	float uu = u * u * (3 - 2 * u);
	float vv = v * v * (3 - 2 * v);
	float ww = w * w * (3 - 2 * w);
	float accum = 0;
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			for (int k = 0; k < 2; ++k)
			{
				Vec3 weight(u - i, v - j, w - k);
				accum += 
					(i * uu + (1 - i) * (1 - uu)) *
					(j * vv + (1 - j) * (1 - vv)) *
					(k * ww + (1 - k) * (1 - ww)) * Dot(c[i][j][k], weight);
			}
		}
	}
	return accum;
}