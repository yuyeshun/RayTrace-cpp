#include "Texture.h"

Vec3 ImageTexture::Value(float u, float v, const Vec3& p) const
{
	int i = (int)(u * nx);
	int j = (int)((1 - v) * ny - 0.001f);
	i = i < 0 ? 0 : (i > nx - 1 ? nx - 1 : i);
	j = j < 0 ? 0 : (j > ny - 1 ? ny - 1 : j);

	float r = int(data[3 * i + 3 * nx * j]) / 255.0f;
	float g = int(data[3 * i + 3 * nx * j + 1]) / 255.0f;
	float b = int(data[3 * i + 3 * nx * j + 2]) / 255.0f;

	return Vec3(r, g, b);
}