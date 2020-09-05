#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <fstream>

#include "Scene.h"

using namespace std;

int TempGenerateCount = 0;

void DisplayProgress(RayTracing *rt, float (RayTracing::*pFun)(), bool bNeedGenerate = false) {
	float percentage = (rt->*pFun)();
	while (percentage < 100.0f) {
		cout << "\b\b\b\b\b\b" << setw(5) << setiosflags(ios::fixed) << setprecision(2) << percentage << "%";
		Sleep(500);
		percentage = (rt->*pFun)();
		if (bNeedGenerate)
		{
			if (TempGenerateCount == 2)
			{
				rt->GenerateImage("TempImage", ImgType::BMP);
				TempGenerateCount = 0;
			}			
			else
			{
				++TempGenerateCount;
			}
		}		
	}
}

int main(int argc, char** argv)
{		
	if (argc <= 1)
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
	}
	
	int quality = 1;
	if (argc <= 1)
	{
		cout << "choose quality(1-Low, 2-Medium, 3-High, 4-SuperHigh)" << endl;
		cin >> quality;
	}
	else
	{
		quality = (int)*argv[1];
	}
	quality = quality <= 0 ? 1 : quality;
	int nx = 1024 * (quality > 2 ? 4 : quality);
	int ny = 540 * (quality > 2 ? 4 : quality);
	int ns = 100 * (quality > 2 ? (quality > 3 ? 100 : 4) : quality);
	int MaxDepth = 25 * (quality > 2 ? 4 : quality);

	int imageType = 1;
	if (argc <= 2)
	{
		cout << "choose image type(1-BMP, 2-PPM)" << endl;
		cin >> imageType;
	}	
	else
	{
		imageType = (int)*argv[2];
	}
	srand(unsigned(time(NULL)));

	Vec3 lookFrom(278, 278, -800);
	Vec3 lookAt(278, 278, 0);
	Vec3 vUp(0, 1, 0);
	float distToFocus = 10.0f;
	float aperture = 0.0f;
	float vFov = 40.f;
	Camera *camera = new Camera(lookFrom, lookAt, vUp, vFov, float(nx) / float(ny), aperture, distToFocus, 0.0f, 1.f);

	//HitableList *world = RandomScene();
	//HitableList* world = TwoPerlinSphere();
	//HitableList* world = SampleLight();
	HitableList* world = CornellBox();
	//HitableList* world = FinalScene();
	
	RayTracing *rayTracing = new RayTracing(nx, ny, ns, world);
	rayTracing->SetDepth(MaxDepth);
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
	rayTracing->GenerateImage("Image", imageType == 1 ? ImgType::BMP : ImgType::PPM);
	
	DisplayProgress(rayTracing, &RayTracing::GetGeneraatePercentage);
	cout << "\b\b\b\b\b\b" << setw(5) << setiosflags(ios::fixed) << setprecision(2) << rayTracing->GetGeneraatePercentage() << "%";
	cout << "\nGenerate finish!" << endl;
	cout << "\aTotal generate time: " << time(NULL) - beginTime << "(s)." << endl;

	system("pause");

	return 0;
}