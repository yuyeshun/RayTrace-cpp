#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <fstream>
#include "RayTracing.h"

using namespace std;

void DisplayProgress(RayTracing *rt, float (RayTracing::*pFun)()) {
	float percentage = (rt->*pFun)();
	while (percentage < 100.0f) {
		cout << "\b\b\b\b\b\b" << setw(5) << setiosflags(ios::fixed) << setprecision(2) << percentage << "%";
		Sleep(500);
		percentage = (rt->*pFun)();
	}
}

HitableList *RandomScene() {
	HitableList *world = new HitableList();
	world->AddObject(new Sphere(Vec3(0, -1000, 0), 1000, new Lambertian(Vec3(0.5, 0.5, 0.5))));

	for (int a = -11; a < 11; ++a) {
		for (int b = -11; b < 11; ++b) {
			float chooseMat = Random();
			Vec3 center(a + 0.9f * Random(), 0.2f, b + 0.9f * Random());
			if ((center - Vec3(4, 0.2f, 0)).Length() > 0.9f) {
				if (chooseMat < 0.75f) {
					world->AddObject(new Sphere(center, 0.2f, new Lambertian(Vec3(Random() * Random(), Random() * Random(), Random() * Random()))));
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
	world->AddObject(new Sphere(Vec3(-4, 1, 0), 1.0, new Lambertian(Vec3(0.4f, 0.2f, 0.1f))));
	world->AddObject(new Sphere(Vec3(4, 1, 0), 1.0, new Metal(Vec3(0.7f, 0.6f, 0.5f))));

	return world;
}

int main()
{
	/** cancle quick edit mode */
	HANDLE inputHandle = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode;
	GetConsoleMode(inputHandle, &mode);
	mode &= ~ENABLE_QUICK_EDIT_MODE;
	SetConsoleMode(inputHandle, mode);

	/** hidden cursor*/
	HANDLE outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(outputHandle, &CursorInfo);
	CursorInfo.bVisible = false;
	SetConsoleCursorInfo(outputHandle, &CursorInfo);

	int nx = 4096;
	int ny = 2160;
	int ns = 200;

	srand(unsigned(time(NULL)));

	Vec3 lookFrom(13, 2, 3);
	Vec3 lookAt(0, 0, 0);
	Vec3 vUp(0, 1, 0);
	float distToFocus = 10.0f;
	float aperture = 0.1f;
	Camera *camera = new Camera(lookFrom, lookAt, vUp, 20, float(nx) / float(ny), aperture, distToFocus);

	HitableList *world = RandomScene();

	RayTracing *rayTracing = new RayTracing(nx, ny, ns, world);
	rayTracing->SetCamera(camera);
	//rayTracing->SetForceThreadNum(6);

	rayTracing->BeginMutiThreadCalc();	

	time_t beginTime = time(NULL);
	int concurrency = ((rayTracing->GetForceThreadNum() > 0) ? rayTracing->GetForceThreadNum() : (thread::hardware_concurrency() > 1 ? thread::hardware_concurrency() - 1 : 1) );
	cout << "Begin multi thread(" << concurrency << ") calculate......" << setw(5) << setiosflags(ios::fixed) << setprecision(2) << rayTracing->GetCalculatePercentage() << "%";
	DisplayProgress(rayTracing, &RayTracing::GetCalculatePercentage);
	cout << "\b\b\b\b\b\b" << setw(5) << setiosflags(ios::fixed) << setprecision(2) << rayTracing->GetCalculatePercentage() << "%";
	cout << "\nCalculate finish!" << endl;
	cout << "\aTotal calculate time: " << time(NULL) - beginTime << "(s)." << endl;

	beginTime = time(NULL);
	cout << "Begin generate image......" << endl;
	rayTracing->GenerateImage("Image", ImgType::BMP);
	//rayTracing->GenerateImage("Image", ImgType::PPM);
	DisplayProgress(rayTracing, &RayTracing::GetGeneraatePercentage);
	cout << "\b\b\b\b\b\b" << setw(5) << setiosflags(ios::fixed) << setprecision(2) << rayTracing->GetGeneraatePercentage() << "%";
	cout << "\nGenerate finish!" << endl;
	cout << "\aTotal generate time: " << time(NULL) - beginTime << "(s)." << endl;

	system("pause");

	return 0;
}