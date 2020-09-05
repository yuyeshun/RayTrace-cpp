#pragma once

#include "RayTracing.h"
#include "Box.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION 
#include "stb_image.h"
#include "BVH.h"

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
	Material* Mirror = new Metal(Vec3(0.7f, 0.6f, 0.5f));
	
	Texture* Checker = new CheckerTexture(new ConstantTexture(Vec3(0.2f, 0.3f, 0.1f)), new ConstantTexture(Vec3(0.9f, 0.9f, 0.9f)));

	world->AddObject(new FlipNormal(new Rect(0, 555, 0, 555, 555, Mirror, RectType::YZ)));
	world->AddObject(new Rect(0, 555, 0, 555, 0, Red, RectType::YZ));
	world->AddObject(new Rect(213, 343, 227, 322, 554, Light, RectType::XZ));
	world->AddObject(new FlipNormal(new Rect(0, 555, 0, 555, 555, White, RectType::XZ)));
	world->AddObject(new Rect(0, 555, 0, 555, 0, White, RectType::XZ));
	world->AddObject(new FlipNormal(new Rect(0, 555, 0, 555, 555, new Lambertian(Checker), RectType::XY)));

	auto box1 = new Translate(new RotateY(new Box(Vec3(0, 0, 0), Vec3(165, 165, 165), White), -18), Vec3(130, 0, 65));
	auto box2 = new Translate(new RotateY(new Box(Vec3(0, 0, 0), Vec3(165, 330, 165), White), 15), Vec3(265, 0, 295));

	world->AddObject(box1);
	world->AddObject(box2);

	int nx, ny, nn;
	unsigned char* textureData = stbi_load("earth.jpg", &nx, &ny, &nn, 0);
	Texture* mapTexture = new ImageTexture(textureData, nx, ny);
	world->AddObject(new Sphere(Vec3(175, 235, 185), 60.0, new Lambertian(mapTexture)));

	world->AddObject(new Sphere(Vec3(280, 40, 45), 40, new Dielectric(1.5f)));
	world->AddObject(new Sphere(Vec3(325, 40, 100), 40, new Metal(Vec3(0.7f, 0.7f, 0.9f), 1)));

	Vec3 center(400, 400, 200);
	world->AddObject(new MovingSphere(center, center + Vec3(15, 0, 0), 0, 1, 40, new Lambertian(new ConstantTexture(Vec3(0.7f, 0.3f, 0.1f)))));	


	/*Texture* PerlinTexture = new NoiseTexture(0.1f);
	world->AddObject(new Sphere(Vec3(220, 280, 300), 40.0, new Lambertian(PerlinTexture)));*/

	return world;
}

HitableList* FinalScene()
{
	int nb = 20;
	HitableList* world = new HitableList();
	HitableList* Boxs = new HitableList();
	HitableList* Boxs2 = new HitableList();
	Material* White = new Lambertian(new ConstantTexture(Vec3(0.73f, 0.73f, 0.73f)));
	Material* Ground = new Lambertian(new ConstantTexture(Vec3(0.48f, 0.83f, 0.53f)));
	
	for (int i = 0; i < nb; ++i)
	{
		for (int j = 0; j < nb; ++j)
		{
			float w = 100;
			float x0 = -1000 + i * w;
			float z0 = -1000 + j * w;
			float y0 = 0;
			float x1 = x0 + w;
			float y1 = 100 * (Random() + 0.01f);
			float z1 = z0 + w;
			Boxs->AddObject(new Box(Vec3(x0, y0, z0), Vec3(x1, y1, z1), Ground));
		}
	}
	world->AddObject(new BVH_Node(Boxs->list.begin(), (int)Boxs->list.size(), 0, 1));
	//¶¥²¿¹âÔ´
	Material* light = new DiffuseLight(new ConstantTexture(Vec3(17, 17, 17)));
	world->AddObject(new Rect(123, 423, 147, 412, 554, light, RectType::XZ));
	

	Hitable* boundary = new Sphere(Vec3(360, 150, 145), 70, new Dielectric(1.5f));
	world->AddObject(boundary);
	world->AddObject(new ConstantMedium(boundary, 0.2f, new ConstantTexture(Vec3(0.2f, 0.4f, 0.9f))));
	boundary = new Sphere(Vec3(0, 0, 0), 5000, new Dielectric(1.5f));
	world->AddObject(new ConstantMedium(boundary, 0.0001f, new ConstantTexture(Vec3(1, 1, 1))));

	

	int ns = 1000;
	for (int j = 0; j < ns; ++j)
	{
		Boxs2->AddObject(new Sphere(Vec3(165 * Random(), 165 * Random(), 165 * Random()), 10, White));
	}

	world->AddObject(new Translate(new RotateY(new BVH_Node(Boxs2->list.begin(), ns, 0, 1), 15), Vec3(-100, 270, 395)));

	return world;
}