#pragma once

#include "RayTracing.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION 
#include "stb_image.h"

HitableList* RandomScene() {
	HitableList* world = new HitableList();
	Texture* Checker = new CheckerTexture(new ConstantTexture(Vec3(0.2f, 0.3f, 0.1f)), new ConstantTexture(Vec3(0.9f, 0.9f, 0.9f)));
	world->AddObject(new Sphere(Vec3(0, -1000, 0), 1000, new Lambertian(Checker)));

	for (int a = -10; a < 10; ++a) {
		for (int b = -10; b < 10; ++b) {
			float chooseMat = Random();
			Vec3 center(a + 0.9f * Random(), 0.2f, b + 0.9f * Random());
			if ((center - Vec3(4, 0.2f, 0)).Length() > 0.9f) {
				if (chooseMat < 0.75f) {
					world->AddObject(new MovingSphere(center, center + Vec3(0.0f, 0.5f * Random(), 0.0f), 0.0f, 1.0f, 0.2f,
						new Lambertian(new ConstantTexture(Vec3(Random() * Random(), Random() * Random(), Random() * Random())))));
				}
				else if (chooseMat < 0.9f) {
					world->AddObject(new Sphere(center, 0.2f, new Metal(Vec3(0.5f * (1 + Random()), 0.5f * (1 + Random()), 0.5f * Random()))));
				}
				else {
					world->AddObject(new Sphere(center, 0.2f, new Dielectric(1.5f)));
				}
			}
		}
	}

	world->AddObject(new Sphere(Vec3(0, 1, 0), 1.0, new Dielectric(1.5f)));
	world->AddObject(new Sphere(Vec3(-4, 1, 0), 1.0, new Lambertian(new ConstantTexture(Vec3(0.4f, 0.2f, 0.1f)))));
	world->AddObject(new Sphere(Vec3(4, 1, 0), 1.0, new Metal(Vec3(0.7f, 0.6f, 0.5f))));

	return world;
}

HitableList* TwoPerlinSphere() {
	HitableList* world = new HitableList();
	Texture* PerlinTexture = new NoiseTexture(1);

	world->AddObject(new Sphere(Vec3(0, -1000, 0), 1000, new Lambertian(PerlinTexture)));
	world->AddObject(new Sphere(Vec3(0, 1, 0), 1.0, new Lambertian(PerlinTexture)));

	int nx, ny, nn;
	unsigned char* textureData = stbi_load("earth.jpg", &nx, &ny, &nn, 0);
	Texture* mapTexture = new ImageTexture(textureData, nx, ny);
	world->AddObject(new Sphere(Vec3(4, 1, 0), 1.0, new Lambertian(mapTexture)));

	return world;
}

HitableList* SampleLight()
{
	HitableList* world = new HitableList();
	Texture* PerlinTexture = new NoiseTexture(4);
	Texture* Constant = new ConstantTexture(Vec3(4, 4, 4));

	world->AddObject(new Sphere(Vec3(0, -1000, 0), 1000, new Lambertian(PerlinTexture)));
	world->AddObject(new Sphere(Vec3(0, 2, 0), 2.0, new Lambertian(PerlinTexture)));

	world->AddObject(new Sphere(Vec3(0, 7, 0), 2.0, new DiffuseLight(Constant)));
	world->AddObject(new Rect(3, 5, 1, 3, -2, new DiffuseLight(Constant), RectType::XY));

	return world;
}


HitableList* CornellBox()
{
	HitableList* world = new HitableList();
	Material* Red = new Lambertian(new ConstantTexture(Vec3(0.65f, 0.05f, 0.05f)));
	Material* White = new Lambertian(new ConstantTexture(Vec3(0.73f, 0.73f, 0.73f)));
	Material* Green = new Lambertian(new ConstantTexture(Vec3(0.12f, 0.45f, 0.15f)));
	Material* Light = new DiffuseLight(new ConstantTexture(Vec3(15, 15, 15)));
	
	world->AddObject(new FlipNormal(new Rect(0, 555, 0, 555, 555, Green, RectType::YZ)));
	world->AddObject(new Rect(0, 555, 0, 555, 0, Red, RectType::YZ));
	world->AddObject(new Rect(213, 343, 227, 322, 554, Light, RectType::XZ));
	world->AddObject(new FlipNormal(new Rect(0, 555, 0, 555, 555, White, RectType::XZ)));
	world->AddObject(new Rect(0, 555, 0, 555, 0, White, RectType::XZ));
	world->AddObject(new FlipNormal(new Rect(0, 555, 0, 555, 555, White, RectType::XY)));

	return world;
}